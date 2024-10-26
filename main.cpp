#include <accelerator/adjacency_list.h>
#include <accelerator/constraint.h>
#include <accelerator/hash_grid.h>
#include <geometry/circle.h>
#include <geometry/math.h>
#include <index/index.h>

#include <cstdlib>
#include <numbers>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <SDL3/SDL.h>

class Random
{
private:
  using dist_t = std::uniform_real_distribution<float>;
  std::random_device rd;
  std::mt19937 mt;

public:
  Random()
    : rd()
    , mt(rd())
  {
  }

private:
  static inline dist_t dist(float min, float max) noexcept
  {
    constexpr float lo = -std::numeric_limits<float>::max();
    return dist_t{ std::nextafterf(min, lo), max };
  }

public:
  bool random_bool()
  {
    return std::uniform_int_distribution(-1, 1)(mt);
  }

  Vec2F random_point()
  {
    const auto puffer = static_cast<float>(c_5 * constraint::max_extend);
    const auto x = dist(puffer, constraint::world_width - puffer)(mt);
    const auto y = dist(puffer, constraint::world_height - puffer)(mt);
    return { x, y };
  }

  Vec2F random_velocity()
  {
    const auto a = dist(0.0f, 2.0f * std::numbers::pi_v<float>)(mt);
    const auto v = dist(0.0f, static_cast<float>(constraint::max_velocity))(mt);
    return { std::cos(a) * v, std::sin(a) * v };
  }

  Float random_radius()
  {
    return dist(static_cast<float>(0.55f * constraint::min_extend), static_cast<float>(0.45f * constraint::max_extend))(mt);
  }

  SDL_Color random_color()
  {
    auto dist_c = std::uniform_int_distribution<int>(128, 255);
    auto dist_f = std::uniform_int_distribution<int>(0, 4)(mt);
    return { static_cast<std::uint8_t>(dist_c(mt) - (dist_f == 1 ? c0 : c128)),
             static_cast<std::uint8_t>(dist_c(mt) - (dist_f == 2 ? c0 : c128)),
             static_cast<std::uint8_t>(dist_c(mt) - (dist_f == 3 ? c0 : c128)),
             255 };
  }

  void fill_random_circles(std::vector<Dynamic>& circles, Index n)
  {
    circles.reserve(n);

    for (unsigned i = 0; i < n; ++i) {
      while (true) {
        auto c = Circle{ random_point(), random_radius() };

        bool overlap = false;
        for (const auto o : circles) {
          if (Overlap(c, Circle{ o.p, o.r })) {
            overlap = true;
            break;
          }
        }

        if (!overlap) {
          circles.emplace_back(c.p, c.r, random_velocity());
          break;
        }
      }
    }
  }
};

struct Game;
struct Scene;

struct Context
{
  bool quit = false;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  Scene* scene = nullptr;
  Game* game = nullptr;
};

struct Scene
{
  Random* random = nullptr;
  Context* context = nullptr;
  std::vector<Dynamic>* circles = nullptr;
  std::vector<SDL_Color>* colors = nullptr;
};

struct Game
{
  Context* context = nullptr;
  Scene* scene = nullptr;
};

Scene*
CreateScene(Context* context)
{
  auto scene = static_cast<Scene*>(SDL_aligned_alloc(sizeof(Scene), 4096));
  if (scene) {
    scene->random = new Random{};
    scene->circles = new std::vector<Dynamic>{};
    scene->colors = new std::vector<SDL_Color>{};

    scene->context = context;
    scene->random->fill_random_circles(*scene->circles, 512 /*16384*/);

    for (int i = 0; i < scene->circles->size(); ++i) {
      const auto c = scene->random->random_color();
      const auto r = c.r;
      const auto g = c.g;
      const auto b = c.b;
      const auto a = 255;
      scene->colors->emplace_back(r, g, b, a);
    }
  }
  return scene;
}

std::vector<Dynamic>&
Circles(Scene* scene)
{
  return *scene->circles;
}

inline auto
Transform(auto vec)
{
  constexpr auto factor = 3.63f;
  constexpr auto offset = Vec<decltype(constraint::max_extend)>{ constraint::max_extend, constraint::max_extend };
  return factor * offset + (factor * vec);
}

inline void
DrawCircle(SDL_Renderer* renderer, Dynamic circle)
{
  constexpr int n = 15;

  Vec<float> a = Transform(circle.p + Vec2F(circle.r, 0.0f));
  for (int i = 1; i <= n; ++i) {
    auto theta = static_cast<Float>(i) * 2 * cnl::numbers::pi_v<Float> / n;
    Vec<float> b = Transform(circle.p + Vec2F(circle.r * cnl::cos(theta), circle.r * cnl::sin(theta)));
    SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
    a = b;
  }
}

template<typename S>
inline void
DrawAABB(SDL_Renderer* renderer, AABB<S> c)
{
  Vec<float, float> min = Transform(c.min);
  Vec<float, float> max = Transform(c.max);
  SDL_RenderLine(renderer, min.x, min.y, max.x, min.y);
  SDL_RenderLine(renderer, max.x, min.y, max.x, max.y);
  SDL_RenderLine(renderer, max.x, max.y, min.x, max.y);
  SDL_RenderLine(renderer, min.x, max.y, min.x, min.y);
}

void
RenderScene(Scene* scene, SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
  SDL_RenderClear(renderer);

  for (int i = 0; i < scene->circles->size(); ++i) {
    const auto [r, g, b, a] = (*scene->colors)[i];
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    DrawCircle(renderer, (*scene->circles)[i]);
    // DrawAABB(renderer, MakeAABB(scene->circles->at(i)));
  }

  SDL_RenderPresent(renderer);
}

void
SetColor(Scene* scene, Uint32 index, SDL_Color color)
{
  scene->colors->at(index) = color;
}

Random&
GetRandom(Scene* scene)
{
  return *scene->random;
}

Game*
CreateGame(Context* context, Scene* scene)
{
  auto game = static_cast<Game*>(SDL_aligned_alloc(sizeof(Game), 4096));
  if (game) {
    game->context = context;
    game->scene = scene;
  }
  return game;
}

bool
IsBlockingStepping(Dynamic a, Dynamic b, float dt)
{
  const auto ci0 = Circle{ a.p, a.r };
  const auto cj0 = Circle{ b.p, b.r };
  const auto ci5 = Circle{ a.p + dt * a.v, a.r };
  const auto cj5 = Circle{ b.p + dt * b.v, b.r };
  // clang-format off
    return Overlap(ci0, cj0) || Overlap(ci0, cj5)
           || Overlap(ci5, cj0) || Overlap(ci5, cj5);
  // clang-format on
}

void
HandleIsland(Scene* scene, std::vector<Dynamic>& circles, std::vector<Index>& island, std::unordered_multimap<Index, Index>& island_edges)
{
  auto& random = GetRandom(scene);
  const auto color = random.random_color();

  const auto n = island.size();
  std::unordered_set<Index> blocked{};

  for (int _ = 0; _ < 8; ++_) {
    blocked.clear();

    for (auto [i, j] : island_edges) {
      if (IsBlockingStepping(circles[i], circles[j], 0.125f)) {
        blocked.insert(i);
        blocked.insert(j);
      }
    }

    for (int i = 0; i < n; ++i)
      if (!blocked.contains(island[i]))
        circles[island[i]].p += 0.125f * circles[island[i]].v;
  }

  for (int i = 0; i < n; ++i) {
    SetColor(scene, island[i], SDL_Color(color.r, color.g, color.b, 255));
    circles[island[i]].v = Vec2F{};
  }
}

void
Update(Game* game, float)
{
  auto& circles = Circles(game->scene);

  static HashGrid grid{};
  grid.Clear();
  grid.Reserve(circles.size());
  for (int i = 0; i < circles.size(); ++i)
    grid.Push(i, circles[i]);

  static AdjacencyList archipelago{};
  archipelago.Clear();
  grid.Query(circles, [&](auto i, auto j) {
    archipelago.AddEdge(i, j);
  });
  archipelago.QueryIslands([&](auto island, auto edges) {
    HandleIsland(game->scene, circles, island, edges);
  });

  auto& random = GetRandom(game->scene);
  for (auto& circle : circles) {
    circle.p += circle.v;

    if (circle.v == Vec2F{} && random.random_bool() && random.random_bool() && random.random_bool() && random.random_bool())
      circle.v = random.random_velocity();

    if (circle.p.x < c0)
      circle.v.x = random.random_bool() ? constraint::max_velocity_f : 0.0f;
    if (circle.p.x > constraint::world_width)
      circle.v.x = random.random_bool() ? -constraint::max_velocity_f : 0.0f;
    if (circle.p.y < c0)
      circle.v.y = random.random_bool() ? constraint::max_velocity_f : 0.0f;
    if (circle.p.y > constraint::world_height)
      circle.v.y = random.random_bool() ? -constraint::max_velocity_f : 0.0f;
  }
}

void
Init(Context* context)
{
  constexpr auto sdl_components = SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO;

  if (!SDL_Init(sdl_components)) {
    SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
    std::exit(1);
  }

  context->window = SDL_CreateWindow("", 0, 0, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);
  if (!context->window) {
    SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    std::exit(1);
  }

  auto vulkan_driver_found = false;
  for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
    if (!strcmp(SDL_GetRenderDriver(i), "vulkan"))
      vulkan_driver_found = true;

  if (!vulkan_driver_found) {
    SDL_Log("vulkan renderer driver not found\n");
    SDL_DestroyWindow(context->window);
    SDL_Quit();
    std::exit(1);
  }

  context->renderer = SDL_CreateRenderer(context->window, "vulkan");
  if (!context->renderer) {
    SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(context->window);
    SDL_Quit();
    std::exit(1);
  }

  const auto display = SDL_GetDisplayForWindow(context->window);
  const auto display_mode = SDL_GetCurrentDisplayMode(display);

  bool status = true;
  status &= SDL_SetWindowSize(context->window, display_mode->w, display_mode->h);
  status &= SDL_SetWindowFullscreenMode(context->window, display_mode);
  status &= SDL_SetWindowFullscreen(context->window, true);
  status &= SDL_SetWindowBordered(context->window, false);
  status &= SDL_ShowWindow(context->window);

  if (!status) {
    SDL_Log("SDL failed to show window: %s\n", SDL_GetError());
    SDL_DestroyWindow(context->window);
    SDL_Quit();
    std::exit(1);
  }

  context->scene = CreateScene(context);
  if (!context->scene) {
    SDL_Log("CreateScene failed!\n");
    SDL_DestroyWindow(context->window);
    SDL_Quit();
    std::exit(1);
  }

  context->game = CreateGame(context, context->scene);
  if (!context->game) {
    SDL_Log("CreateGame failed!\n");
    delete context->scene->circles;
    delete context->scene->colors;
    SDL_aligned_free(context->scene);
    SDL_DestroyWindow(context->window);
    SDL_Quit();
    std::exit(1);
  }
}

void
Loop(Context* context)
{
  constexpr auto dt = static_cast<float>(20'000'000);
  auto t0 = SDL_GetTicksNS();

  do {
    const auto ti = SDL_GetTicksNS();
    RenderScene(context->scene, context->renderer);

    const auto t_beg = SDL_GetTicksNS();
    Update(context->game, static_cast<float>(ti - t0) / dt);
    const auto t_end = SDL_GetTicksNS();

    SDL_Log("%f fps; update: %lu ns \n", 1000.0 / (static_cast<double>(ti - t0) / 1000000.0), t_end - t_beg);
    SDL_Delay(5); // renderer needs some time

    t0 = ti;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        context->quit = true;
      if ((&event)->type == SDL_EVENT_KEY_DOWN)
        switch ((&event)->key.key) {
          case SDLK_ESCAPE:
            context->quit = true;
          default:
            break;
        }
    }

  } while (!context->quit);
}

void
Quit(Context* context)
{
  delete context->scene->circles;
  delete context->scene->colors;
  SDL_aligned_free(context->scene);
  SDL_aligned_free(context->game);
  context->game = nullptr;

  SDL_DestroyWindow(context->window);
  context->window = nullptr;

  SDL_Quit();
}

int
main(int, char**)
{
  auto context = Context();
  Init(&context);
  Loop(&context);
  Quit(&context);
}
