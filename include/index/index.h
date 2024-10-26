#pragma once

#include <cstdint>
#include <memory>

using Index = std::uint16_t;

[[maybe_unused]] constexpr inline void
Swap(Index& a, Index& b) noexcept
{
  std::swap(a, b);
}

struct [[maybe_unused]] IndexPair
{
  Index first{};
  Index second{};

  [[nodiscard]] [[maybe_unused]] constexpr inline bool operator==(IndexPair pair) const noexcept
  {
    return first == pair.first && second == pair.second;
  }

  [[nodiscard]] [[maybe_unused]] constexpr inline bool operator!=(IndexPair pair) const noexcept
  {
    return first != pair.first && second != pair.second;
  }

  [[nodiscard]] constexpr inline IndexPair Add(IndexPair other) const noexcept
  {
    const auto first_ = static_cast<Index>(first + other.first);
    const auto second_ = static_cast<Index>(second + other.second);
    return { first_, second_ };
  }

  [[nodiscard]] constexpr inline IndexPair Add(Index f, Index s) const noexcept
  {
    const auto first_ = static_cast<Index>(first + f);
    const auto second_ = static_cast<Index>(second + s);
    return { first_, second_ };
  }

  [[nodiscard]] constexpr inline IndexPair operator+(IndexPair other) const noexcept
  {
    const auto first_ = static_cast<Index>(first + other.first);
    const auto second_ = static_cast<Index>(second + other.second);
    return { first_, second_ };
  }

  [[nodiscard]] constexpr inline IndexPair& operator+=(IndexPair other) noexcept
  {
    first = static_cast<Index>(first + other.first);
    second = static_cast<Index>(second + other.second);
    return *this;
  }

  [[nodiscard]] [[maybe_unused]] constexpr inline IndexPair& Sort() noexcept
  {
    if (first > second)
      Swap(first, second);
    return *this;
  }
};

[[nodiscard]] [[maybe_unused]] constexpr inline IndexPair
Sorted(IndexPair pair) noexcept
{
  return pair.Sort();
}

[[maybe_unused]] [[nodiscard]] constexpr inline Index
MakeIndex(auto value) noexcept
{
  return static_cast<Index>(value);
}

[[maybe_unused]] [[nodiscard]] constexpr inline IndexPair
MakeIndex(auto value1, auto value2) noexcept
{
  return IndexPair{ MakeIndex(value1), MakeIndex(value2) };
}

[[maybe_unused]] [[nodiscard]] constexpr inline IndexPair
MakeSortedIndex(auto value1, auto value2) noexcept
{
  static_assert(std::is_same_v<decltype(make_index(value1)), Index>);
  static_assert(std::is_same_v<decltype(make_index(value2)), Index>);
  return IndexPair{ make_index(value1), make_index(value2) }.Sort();
}

struct [[maybe_unused]] IndexHash
{
  typedef std::size_t result_type;
  typedef Index argument_type;

  [[nodiscard]] [[maybe_unused]] constexpr inline result_type operator()(argument_type index) const noexcept
  {
    return index;
  }
};

struct [[maybe_unused]] IndexPairHash
{
  typedef std::size_t result_type;
  typedef IndexPair argument_type;

  [[nodiscard]] [[maybe_unused]] constexpr inline result_type operator()(argument_type pair) const noexcept
  {
    auto h = static_cast<std::uint32_t>(pair.first);
    h <<= sizeof(Index) << 3;
    h |= pair.second;
    return h;
  }
};

namespace std {
template<>
struct [[maybe_unused]] hash<IndexPair>
{
  typedef std::size_t result_type;
  typedef IndexPair argument_type;

  [[nodiscard]] [[maybe_unused]] constexpr inline result_type operator()(argument_type pair) const noexcept
  {
    auto h = static_cast<std::uint32_t>(pair.first);
    h <<= sizeof(Index) << 3;
    h |= pair.second;
    return h;
  }
};
}
