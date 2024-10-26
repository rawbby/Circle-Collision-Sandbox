#pragma once

#include "./circle.h"
#include "./math.h"

#include <accelerator/constraint.h>

template<typename S>
struct [[maybe_unused]] AABB
{
  Vec<S, S> min;
  Vec<S, S> max;

  [[maybe_unused]] constexpr inline AABB(Vec<S, S> min, Vec<S, S> max) noexcept
    : min(min)
    , max(max)
  {
  }
};

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MinAABB(Circle circle) noexcept
{
  const auto min_x = circle.p.x - circle.r;
  const auto min_y = circle.p.y - circle.r;
  static_assert(std::is_same_v<decltype(min_x), decltype(min_y)>);
  using S = decltype(min_x);
  return Vec<S>{ min_x, min_y };
}

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MinAABB(Dynamic dynamic) noexcept
{
  const auto min_x = dynamic.p.x - dynamic.r + std::min(dynamic.v.x, Float{ 0.0f });
  const auto min_y = dynamic.p.y - dynamic.r + std::min(dynamic.v.y, Float{ 0.0f });
  static_assert(std::is_same_v<decltype(min_x), decltype(min_y)>);
  return Vec<decltype(min_x)>{ min_x, min_y };
}

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MaxAABB(Circle circle) noexcept
{
  const auto max_x = circle.p.x + circle.r;
  const auto max_y = circle.p.y + circle.r;
  static_assert(std::is_same_v<decltype(max_x), decltype(max_y)>);
  return Vec<decltype(max_x)>{ max_x, max_y };
}

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MaxAABB(Dynamic dynamic) noexcept
{
  const auto max_x = dynamic.p.x + dynamic.r + std::max(dynamic.v.x, Float{ 0.0f });
  const auto max_y = dynamic.p.y + dynamic.r + std::max(dynamic.v.y, Float{ 0.0f });
  static_assert(std::is_same_v<decltype(max_x), decltype(max_y)>);
  return Vec<decltype(max_x)>{ max_x, max_y };
}

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MakeAABB(Circle circle) noexcept
{
  const auto min = MinAABB(circle);
  const auto max = MaxAABB(circle);
  static_assert(std::is_same_v<decltype(min.x), decltype(max.x)>);
  return AABB<decltype(min.x)>{ { min.x, min.y }, { max.x, max.y } };
}

[[nodiscard]] [[maybe_unused]] constexpr inline auto
MakeAABB(Dynamic dynamic) noexcept
{
  const auto min = MinAABB(dynamic);
  const auto max = MaxAABB(dynamic);
  static_assert(std::is_same_v<decltype(min.x), decltype(max.x)>);
  return AABB<decltype(min.x)>{ { min.x, min.y }, { max.x, max.y } };
}

template<typename SA, typename SB>
[[nodiscard]] [[maybe_unused]] constexpr inline bool
MayCollide(AABB<SA> a, AABB<SB> b) noexcept
{
  // clang-format off
  return (a.min.x < b.max.x && a.max.x > b.min.x) &&  // Overlap on the X axis
         (a.min.y < b.max.y && a.max.y > b.min.y);    // Overlap on the Y axis
  // clang-format on
}
