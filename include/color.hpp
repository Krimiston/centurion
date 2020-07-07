/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file color.hpp
 * @brief Provides the `Color` class.
 *
 * @author Albin Johansson
 * @date 2019-2020
 * @copyright MIT License
 */

#ifndef CENTURION_COLOR_HEADER
#define CENTURION_COLOR_HEADER

#include <SDL.h>

#include <string>
#include <type_traits>

#include "centurion_api.hpp"

namespace centurion {

/**
 * @class Color
 * @brief An 8-bit accuracy RGBA color.
 *
 * @details This class is designed to be easily be created and converted
 * from/to SDL colors, such as `SDL_Color` and `SDL_MessageBoxColor`.
 *
 * @headerfile color.hpp
 * @since 3.0.0
 */
class Color final {
 public:
  /**
   * @var Color::max
   * @brief The maximum possible value of a color component.
   *
   * @since 3.0.0
   */
  static inline constexpr Uint8 max = 0xFF;

  /**
   * @brief Creates a color. The created color will be equal to #000000FF.
   *
   * @since 3.0.0
   */
  constexpr Color() noexcept = default;

  /**
   * @brief Creates a color.
   *
   * @param red the red component value, in the range [0, 255].
   * @param green the green component value, in the range [0, 255].
   * @param blue the blue component value, in the range [0, 255].
   * @param alpha the alpha component value, in the rage [0, 255]. Defaults to
   * 255.
   *
   * @since 3.0.0
   */
  constexpr Color(Uint8 red,
                  Uint8 green,
                  Uint8 blue,
                  Uint8 alpha = 0xFF) noexcept
      : m_red{red}, m_green{green}, m_blue{blue}, m_alpha{alpha}
  {}

  /**
   * @brief Creates a color that is a copy of the supplied `SDL_Color`.
   *
   * @param color the `SDL_Color` that will be copied.
   *
   * @since 3.0.0
   */
  constexpr explicit Color(const SDL_Color& color) noexcept
      : m_red{color.r}, m_green{color.g}, m_blue{color.b}, m_alpha{color.a}
  {}

  /**
   * @brief Creates a color that is a copy of the supplied SDL_MessageBoxColor.
   *
   * @details Message box colors don't have an alpha component so the created
   * color will feature an alpha value of 255.
   *
   * @param color the message box color that will be copied.
   *
   * @since 3.0.0
   */
  constexpr explicit Color(const SDL_MessageBoxColor& color) noexcept
      : m_red{color.r}, m_green{color.g}, m_blue{color.b}, m_alpha{max}
  {}

  /**
   * @brief Sets the value of the red component.
   *
   * @param red the new value of the red component.
   *
   * @since 3.0.0
   */
  constexpr void set_red(Uint8 red) noexcept { m_red = red; }

  /**
   * @brief Sets the value of the green component.
   *
   * @param green the new value of the green component.
   *
   * @since 3.0.0
   */
  constexpr void set_green(Uint8 green) noexcept { m_green = green; }

  /**
   * @brief Sets the value of the blue component.
   *
   * @param blue the new value of the blue component.
   *
   * @since 3.0.0
   */
  constexpr void set_blue(Uint8 blue) noexcept { m_blue = blue; }

  /**
   * @brief Sets the value of the alpha component.
   *
   * @param alpha the new value of the alpha component.
   *
   * @since 3.0.0
   */
  constexpr void set_alpha(Uint8 alpha) noexcept { m_alpha = alpha; }

  /**
   * @brief Returns the value of the red component.
   *
   * @return the value of the red component, in the range [0, 255].
   *
   * @since 3.0.0
   */
  [[nodiscard]] constexpr auto red() const noexcept -> Uint8 { return m_red; }

  /**
   * @brief Returns the value of the green component.
   *
   * @return the value of the green component, in the range [0, 255].
   *
   * @since 3.0.0
   */
  [[nodiscard]] constexpr auto green() const noexcept -> Uint8
  {
    return m_green;
  }

  /**
   * @brief Returns the value of the blue component.
   *
   * @return the value of the blue component, in the range [0, 255].
   *
   * @since 3.0.0
   */
  [[nodiscard]] constexpr auto blue() const noexcept -> Uint8 { return m_blue; }

  /**
   * @brief Returns the value of the alpha component.
   *
   * @return the value of the alpha component, in the range [0, 255].
   *
   * @since 3.0.0
   */
  [[nodiscard]] constexpr auto alpha() const noexcept -> Uint8
  {
    return m_alpha;
  }

  /**
   * @brief Returns a textual representation of the color.
   *
   * @return a textual representation of the color.
   *
   * @since 3.0.0
   */
  [[nodiscard]] auto to_string() const -> std::string;

  /**
   * @brief Converts the the color into an `SDL_Color`.
   *
   * @return an `SDL_Color` that is equivalent to this color.
   *
   * @since 3.0.0
   */
  [[nodiscard]] explicit constexpr operator SDL_Color() const noexcept
  {
    return {m_red, m_green, m_blue, m_alpha};
  }

  /**
   * @brief Converts the the color into an `SDL_MessageBoxColor`.
   *
   * @note Message box colors don't feature an alpha value!
   *
   * @return an `SDL_MessageBoxColor` that is equivalent to this color.
   *
   * @since 3.0.0
   */
  [[nodiscard]] explicit constexpr operator SDL_MessageBoxColor() const noexcept
  {
    return {m_red, m_green, m_blue};
  }

  /**
   * @brief Converts the color to `SDL_Color*`.
   *
   * @warning The returned pointer is not to be freed or stored away!
   *
   * @return a reinterpreted pointer to the Color instance.
   *
   * @since 4.0,0
   */
  [[nodiscard]] explicit operator SDL_Color*() noexcept
  {
    return reinterpret_cast<SDL_Color*>(this);
  }

  /**
   * Converts the color to a pointer to a SDL_Color instance.
   *
   * @return a pointer to the Color instance reinterpreted as a SDL_Color
   * instance.
   * @since 4.0,0
   */
  [[nodiscard]] explicit operator const SDL_Color*() const noexcept
  {
    return reinterpret_cast<const SDL_Color*>(this);
  }

 private:
  Uint8 m_red{0};
  Uint8 m_green{0};
  Uint8 m_blue{0};
  Uint8 m_alpha{max};
};

inline auto Color::to_string() const -> std::string
{
  const auto r = std::to_string(m_red);
  const auto g = std::to_string(m_green);
  const auto b = std::to_string(m_blue);
  const auto a = std::to_string(m_alpha);
  return "[Color | R: " + r + ", G: " + g + ", B: " + b + ", A: " + a + "]";
}

/**
 * @brief Indicates whether or not the two colors are equal.
 *
 * @param lhs the left-hand side color.
 * @param rhs the right-hand side color.
 *
 * @return `true` if the colors are equal; `false` otherwise.
 *
 * @since 3.0.0
 */
[[nodiscard]] inline constexpr auto operator==(const Color& lhs,
                                               const Color& rhs) noexcept
    -> bool
{
  return (lhs.red() == rhs.red()) && (lhs.green() == rhs.green()) &&
         (lhs.blue() == rhs.blue()) && (lhs.alpha() == rhs.alpha());
}

/**
 * @copydoc operator==(const Color&, const Color&)
 */
[[nodiscard]] inline constexpr auto operator==(const Color& lhs,
                                               const SDL_Color& rhs) noexcept
    -> bool
{
  return (lhs.red() == rhs.r) && (lhs.green() == rhs.g) &&
         (lhs.blue() == rhs.b) && (lhs.alpha() == rhs.a);
}

/**
 * @copydoc operator==(const Color&, const Color&)
 */
[[nodiscard]] inline constexpr auto operator==(const SDL_Color& lhs,
                                               const Color& rhs) noexcept
    -> bool
{
  return rhs == lhs;
}

/**
 * @copybrief operator==(const Color&, const Color&)
 *
 * @note The alpha components are not taken into account.
 *
 * @param lhs the left-hand side color.
 * @param rhs the right-hand side color.
 *
 * @return `true` if the colors are equal; `false` otherwise.
 *
 * @since 3.0.0
 */
[[nodiscard]] inline constexpr auto operator==(
    const Color& lhs,
    const SDL_MessageBoxColor& rhs) noexcept -> bool
{
  return (lhs.red() == rhs.r) && (lhs.green() == rhs.g) &&
         (lhs.blue() == rhs.b);
}

/**
 * @copydoc operator==(const Color&, const SDL_MessageBoxColor&)
 */
[[nodiscard]] inline constexpr auto operator==(const SDL_MessageBoxColor& lhs,
                                               const Color& rhs) noexcept
    -> bool
{
  return rhs == lhs;
}

/**
 * @brief Indicates whether or not the two colors aren't equal.
 *
 * @param lhs the left-hand side color.
 * @param rhs the right-hand side color.
 *
 * @return `true` if the colors aren't equal; `false` otherwise.
 *
 * @since 3.0.0
 */
[[nodiscard]] inline constexpr auto operator!=(const Color& lhs,
                                               const Color& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/**
 * @copydoc operator!=(const Color&, const Color&)
 */
[[nodiscard]] inline constexpr bool operator!=(const Color& lhs,
                                               const SDL_Color& rhs) noexcept
{
  return !(lhs == rhs);
}

/**
 * @copydoc operator!=(const Color&, const Color&)
 */
[[nodiscard]] inline constexpr bool operator!=(const SDL_Color& lhs,
                                               const Color& rhs) noexcept
{
  return !(lhs == rhs);
}

/**
 * @copybrief operator!=(const Color&, const Color&)
 *
 * @note The alpha components are not taken into account.
 *
 * @param lhs the left-hand side color.
 * @param rhs the right-hand side color.
 *
 * @return `true` if the colors aren't equal; `false` otherwise.
 *
 * @since 3.0.0
 */
[[nodiscard]] inline constexpr auto operator!=(
    const Color& lhs,
    const SDL_MessageBoxColor& rhs) noexcept -> bool
{
  return !(lhs == rhs);
}

/**
 * @copydoc operator!=(const Color&, const SDL_MessageBoxColor&)
 */
[[nodiscard]] inline constexpr bool operator!=(const SDL_MessageBoxColor& lhs,
                                               const Color& rhs) noexcept
{
  return !(lhs == rhs);
}

static_assert(std::is_final_v<Color>);
static_assert(std::is_default_constructible_v<Color>);
static_assert(std::is_nothrow_copy_constructible_v<Color>);
static_assert(std::is_nothrow_copy_assignable_v<Color>);
static_assert(std::is_nothrow_move_constructible_v<Color>);
static_assert(std::is_nothrow_move_assignable_v<Color>);

}  // namespace centurion

#endif  // CENTURION_COLOR_HEADER
