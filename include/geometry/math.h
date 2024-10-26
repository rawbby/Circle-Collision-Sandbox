#pragma once

#include <cmath>

#include <cnl/elastic_integer.h>
#include <cnl/fixed_point.h>

[[maybe_unused]] constexpr auto c_25 = cnl::fixed_point<int, -2>(0.25f);
[[maybe_unused]] constexpr auto c_5 = cnl::fixed_point<int, -1>(0.5f);
[[maybe_unused]] constexpr auto c0 = cnl::fixed_point<int>(0);
[[maybe_unused]] constexpr auto c1 = cnl::fixed_point<int>(1);
[[maybe_unused]] constexpr auto c2 = cnl::fixed_point<int>(2);
[[maybe_unused]] constexpr auto c4 = cnl::fixed_point<int>(4);
[[maybe_unused]] constexpr auto c8 = cnl::fixed_point<int>(8);
[[maybe_unused]] constexpr auto c16 = cnl::fixed_point<int>(16);
[[maybe_unused]] constexpr auto c32 = cnl::fixed_point<int>(32);
[[maybe_unused]] constexpr auto c64 = cnl::fixed_point<int>(64);
[[maybe_unused]] constexpr auto c128 = cnl::fixed_point<int>(128);
[[maybe_unused]] constexpr auto c256 = cnl::fixed_point<int>(256);
[[maybe_unused]] constexpr auto c512 = cnl::fixed_point<int>(512);
[[maybe_unused]] constexpr auto c1024 = cnl::fixed_point<int>(1024);

[[maybe_unused]] [[nodiscard]] constexpr inline auto
Sqrt(auto s) noexcept
{
  return cnl::sqrt(s);
}

[[maybe_unused]] [[nodiscard]] constexpr inline auto
Abs(auto s) noexcept
{
  return cnl::abs(s);
}

template<typename X, typename Y = X>
class Vec
{
public:
  X x;
  Y y;

  [[nodiscard]] [[maybe_unused]] constexpr inline Vec() noexcept
    : x()
    , y()
  {
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec(X_ x, Y_ y) noexcept
    : x(x)
    , y(y)
  {
  }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec(Vec<X_, Y_> other) noexcept
    : x(other.x)
    , y(other.y)
  {
  }
#pragma clang diagnostic pop

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec<X, Y>& operator=(Vec<X_, Y_> other) noexcept
  {
    if constexpr (std::is_same_v<X, X_> && std::is_same_v<Y, Y_>)
      if (this == &other)
        return *this;

    x = other.x;
    y = other.y;
    return *this;
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline auto operator+(Vec<X_, Y_> other) const noexcept
  {
    const auto x_ = x + other.x;
    const auto y_ = y + other.y;
    return Vec<decltype(x_), decltype(y_)>(x_, y_);
  }

  template<typename X_, typename Y_>
  [[maybe_unused]] constexpr inline Vec<X, Y>& operator+=(const Vec<X_, Y_>& other) noexcept
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline auto operator-(const Vec<X_, Y_>& other) const noexcept
  {
    const auto x_ = x - other.x;
    const auto y_ = y - other.y;
    return Vec<decltype(x_), decltype(y_)>(x_, y_);
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec<X, Y>& operator-=(const Vec<X_, Y_>& other) noexcept
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  template<typename S>
  [[nodiscard]] [[maybe_unused]] constexpr inline auto operator*(S s) const noexcept
  {
    const auto x_ = x * s;
    const auto y_ = y * s;
    return Vec<decltype(x_), decltype(y_)>(x_, y_);
  }

  template<typename S>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec<X, Y>& operator*=(S s) noexcept
  {
    x *= s;
    y *= s;
    return *this;
  }

  template<typename S>
  [[nodiscard]] [[maybe_unused]] constexpr inline auto operator/(S s) const noexcept
  {
    const auto x_ = x / s;
    const auto y_ = y / s;
    return Vec<decltype(x_), decltype(y_)>(x_, y_);
  }

  template<typename S>
  [[nodiscard]] [[maybe_unused]] constexpr inline Vec<X, Y>& operator/=(S s) noexcept
  {
    x /= s;
    y /= s;
    return *this;
  }

  [[nodiscard]] [[maybe_unused]] constexpr inline auto operator-() const noexcept
  {
    const auto x_ = -x;
    const auto y_ = -y;
    return Vec<decltype(x_), decltype(y_)>(x_, y_);
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline bool operator==(const Vec<X_, Y_>& other) const noexcept
  {
    return x == other.x && y == other.y;
  }

  template<typename X_, typename Y_>
  [[nodiscard]] [[maybe_unused]] constexpr inline bool operator!=(const Vec<X_, Y_>& other) const noexcept
  {
    return x != other.x && y != other.y;
  }
};

template<typename S, typename X, typename Y>
[[nodiscard]] [[maybe_unused]] constexpr inline auto
operator*(S s, Vec<X, Y> v) noexcept
{
  const auto x_ = s * v.x;
  const auto y_ = s * v.y;
  return Vec<decltype(x_), decltype(y_)>(x_, y_);
}

template<typename X, typename Y>
[[nodiscard]] [[maybe_unused]] constexpr inline auto
Abs(Vec<X, Y> v) noexcept
{
  const auto x_ = Abs(v.x);
  const auto y_ = Abs(v.y);
  return Vec<decltype(x_), decltype(y_)>{ x_, y_ };
}

template<typename X0, typename Y0, typename X1, typename Y1>
[[nodiscard]] [[maybe_unused]] constexpr inline auto
Dot(Vec<X0, Y0> v0, Vec<X1, Y1> v1) noexcept
{
  return v0.x * v1.x + v0.y * v1.y;
}

template<typename X, typename Y>
[[nodiscard]] [[maybe_unused]] constexpr inline auto
Length(Vec<X, Y> v) noexcept
{
  return Sqrt(Dot(v, v));
}

template<typename X, typename Y>
[[nodiscard]] [[maybe_unused]] constexpr inline auto
Normalize(Vec<X, Y> v) noexcept
{
  return v / Length(v);
}

using Float = cnl::fixed_point<cnl::elastic_integer<23, int>, -10>;
using Vec2F = Vec<Float, Float>;
