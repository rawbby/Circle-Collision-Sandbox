#pragma once

#include <bit>
#include <cmath>
#include <cstdint>

#include "geometry/math.h"
#include "index/index.h"

namespace constraint {

[[maybe_unused]] constexpr auto max_extend = 12u;
[[maybe_unused]] constexpr auto min_extend = 4u;
[[maybe_unused]] constexpr auto max_velocity = 4u;
[[maybe_unused]] constexpr auto max_aabb_extend = 16u;

[[maybe_unused]] constexpr auto max_extend_f = 12.0f;
[[maybe_unused]] constexpr auto min_extend_f = 4.0f;
[[maybe_unused]] constexpr auto max_velocity_f = 4.0f;
[[maybe_unused]] constexpr auto max_aabb_extend_f = 16.0f;

[[maybe_unused]] constexpr auto max_world_width = static_cast<unsigned>(static_cast<Index>(~0u) / max_aabb_extend);
[[maybe_unused]] constexpr auto max_world_height = static_cast<unsigned>(static_cast<Index>(~0u) / max_aabb_extend);

[[maybe_unused]] constexpr auto max_world_width_f = static_cast<float>(max_world_width);
[[maybe_unused]] constexpr auto max_world_height_f = static_cast<float>(max_world_height);

[[maybe_unused]] constexpr auto world_width = 512u;
[[maybe_unused]] constexpr auto world_height = 256u;

[[maybe_unused]] constexpr auto world_width_f = 512.0f;
[[maybe_unused]] constexpr auto world_height_f = 256.0f;

template<typename Rep = int, int Exponent = 0, int Radix = 2>
[[nodiscard]] [[maybe_unused]] constexpr inline Index
Quantify(cnl::fixed_point<Rep, Exponent, Radix> scalar) noexcept
{
  static_assert(world_width <= max_world_width);
  static_assert(world_height <= max_world_height);
  // TODO extract representation after trunc instead of uint32
  auto index = static_cast<std::uint32_t>(scalar);
  index >>= std::countr_zero(max_aabb_extend);
  return static_cast<Index>(index);
}

[[nodiscard]] [[maybe_unused]] constexpr inline IndexPair
Quantify(auto a, auto b) noexcept
{
  return { Quantify(a), Quantify(b) };
}

template<typename S>
[[nodiscard]] [[maybe_unused]] constexpr inline IndexPair
Quantify(Vec<S> s) noexcept
{
  return { Quantify(s.x), Quantify(s.y) };
}

}
