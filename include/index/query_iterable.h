#pragma once

#include <utility>

template<typename Iterator>
struct [[maybe_unused]] QueryIterable
{
  Iterator m_begin;
  Iterator m_end;

  [[maybe_unused]] constexpr inline QueryIterable() noexcept
    : m_begin()
    , m_end()
  {
  }

  [[maybe_unused]] constexpr inline QueryIterable(Iterator begin, Iterator end) noexcept
    : m_begin(std::move(begin))
    , m_end(std::move(end))
  {
  }

  [[maybe_unused]] constexpr inline QueryIterable(std::pair<Iterator, Iterator> pair) noexcept // NOLINT(*-explicit-constructor)
    : m_begin(std::move(pair.first))
    , m_end(std::move(pair.second))
  {
  }

  [[nodiscard]] [[maybe_unused]] constexpr inline Iterator begin() const noexcept
  {
    return m_begin;
  }

  [[nodiscard]] [[maybe_unused]] constexpr inline Iterator end() const noexcept
  {
    return m_end;
  }
};
