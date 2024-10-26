#pragma once

#include "./math.h"

struct [[maybe_unused]] Circle
{
  Vec2F p;
  Float r;
};

struct [[maybe_unused]] Dynamic
{
  Vec2F p;
  Float r;
  Vec2F v;
};

[[nodiscard]] [[maybe_unused]] constexpr inline bool
DoCollide(Dynamic d0, Dynamic d1) noexcept
{
  // Equation to solve for t:
  // |(d0.p + t * d0.v) - (d1.p + t * d1.v)| <= d0.r + d1.r
  // <=> (dvv)t^2 + (2dpv)t + dpp <= x*x

  const auto x = d1.r + d0.r;
  const auto xx = (x * x);

  // *** CHECK FOR COLLISION AT t=0 ***

  const auto dp = d1.p - d0.p;
  const auto dpp = Dot(dp, dp);
  const auto c = dpp;

  if (c <= xx)
    return true; // colliding at t=0

  // *** CHECK FOR COLLISION AT t=1 ***

  const auto v0_ = Vec2F{ d0.p + d0.v } - d0.p;
  const auto v1_ = Vec2F{ d1.p + d1.v } - d1.p;
  const auto dv = v1_ - v0_;
  // ensure the velocity is accurate with roundings
  // const auto dv = (dt * d1.v) - (dt * d0.v);

  const auto dpv = Dot(dp, dv);
  const auto dvv = Dot(dv, dv);
  const auto a = dvv;
  const auto b = dpv + dpv;

  if (a + b + c <= xx)
    return true; // colliding at t=1

  // *** ANALYSE DERIVATIVE FOR CLOSEST POINT ***
  // 2at + b = 0 <=> t = -b / 2a

  if (b > c0)
    return false; // Closest Point at t<0

  const auto a2 = a + a;
  if (-b > a2)
    return false; // Closest Point at t>1

  if (a == c0) {
    if (b == c0) // Constant Equation
      return c <= xx;
    // Linear Equation
    // 0 = bt + c <=> t_min = -c / b
    const auto t_min = -c / b;
    return t_min < c1 && t_min > c0;
  }

  // Closest Point at 0 < t < 1:
  // t = -b / (a + a) <=> c - (bb / a4)
  return c - (b * b) / (a2 + a2) <= xx;
}

[[nodiscard]] [[maybe_unused]] constexpr inline bool
Overlap(Circle a, Circle b) noexcept
{
  const auto x = b.r + a.r;
  const auto dp = b.p - a.p;
  return Dot(dp, dp) <= x * x;
}

[[nodiscard]] [[maybe_unused]] constexpr inline Circle
Advance(Dynamic dynamic) noexcept
{
  return { dynamic.p + dynamic.v, dynamic.r };
}

[[nodiscard]] [[maybe_unused]] constexpr inline Circle
Advance(Dynamic dynamic, auto dt) noexcept
{
  return { dynamic.p + dt * dynamic.v, dynamic.r };
}
