#ifndef CENTURION_DETAIL_HINTS_IMPL_HEADER
#define CENTURION_DETAIL_HINTS_IMPL_HEADER

#include <optional>     // optional
#include <string>       // string, stoi, stoul, stof
#include <type_traits>  // is_same_v, is_convertible_v

#include "../core/czstring.hpp"
#include "../core/integers.hpp"
#include "czstring_compare.hpp"
#include "czstring_eq.hpp"
#include "from_string.hpp"
#include "static_bimap.hpp"

/// \cond FALSE

namespace cen::detail {

template <typename Key, usize Size>
using string_map = static_bimap<Key, czstring, czstring_compare, Size>;

template <typename Derived, typename Arg>
class crtp_hint
{
 public:
  using value_type = Arg;

  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, value_type>;
  }

  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return Derived::name();
  }

  [[nodiscard]] static auto value() -> std::optional<value_type>
  {
    return Derived::current_value();
  }

  [[nodiscard]] static auto from_string(const czstring str) -> value_type
  {
    return Derived::convert(str);
  }

  [[nodiscard]] static auto to_string(value_type value) -> std::string
  {
    return std::to_string(value);
  }
};

// A hint class that only accepts booleans
template <typename Hint>
class bool_hint : public crtp_hint<bool_hint<Hint>, bool>
{
 public:
  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, bool>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<bool>
  {
    return static_cast<bool>(SDL_GetHintBoolean(Hint::name(), SDL_FALSE));
  }

  [[nodiscard]] static auto convert(const czstring str) noexcept -> bool
  {
    return czstring_eq(str, "1") ? true : false;
  }

  [[nodiscard]] static auto to_string(const bool value) -> std::string
  {
    return value ? "1" : "0";
  }
};

// A hint class that only accepts strings
template <typename Hint>
class string_hint : public crtp_hint<string_hint<Hint>, czstring>
{
 public:
  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_convertible_v<T, czstring>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<czstring>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value)
    {
      return std::nullopt;
    }
    else
    {
      return value;
    }
  }

  [[nodiscard]] static auto convert(const czstring str) noexcept -> czstring
  {
    return str;
  }

  [[nodiscard]] static auto to_string(const czstring value) -> std::string
  {
    return value;
  }
};

// A hint class that only accepts integers
template <typename Hint>
class int_hint : public crtp_hint<int_hint<Hint>, int>
{
 public:
  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, int>;
  }

  [[nodiscard]] static auto current_value() -> std::optional<int>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value)
    {
      return std::nullopt;
    }
    else
    {
      return std::stoi(value);
    }
  }

  [[nodiscard]] static auto convert(const czstring str) -> int
  {
    return from_string<int>(str).value();
  }
};

// A hint class that only accepts unsigned integers
template <typename Hint>
class unsigned_int_hint : public crtp_hint<int_hint<Hint>, uint>
{
 public:
  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, uint>;
  }

  [[nodiscard]] static auto current_value() -> std::optional<uint>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value)
    {
      return std::nullopt;
    }
    else
    {
      return static_cast<uint>(std::stoul(value));
    }
  }

  [[nodiscard]] static auto convert(const czstring str) -> uint
  {
    return from_string<uint>(str).value();
  }
};

// A hint class that only accepts floats
template <typename Hint>
class float_hint : public crtp_hint<float_hint<Hint>, float>
{
 public:
  template <typename T>
  [[nodiscard]] constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, float>;
  }

  [[nodiscard]] static auto current_value() -> std::optional<float>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value)
    {
      return std::nullopt;
    }
    else
    {
      return std::stof(value);
    }
  }

  [[nodiscard]] static auto convert(const czstring str) -> float
  {
    return from_string<float>(str).value();
  }
};

}  // namespace cen::detail

/// \endcond

#endif  // CENTURION_DETAIL_HINTS_IMPL_HEADER
