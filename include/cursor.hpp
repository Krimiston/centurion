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
 * @file cursor.hpp
 * @brief Provides the cursor API.
 *
 * @author Albin Johansson
 * @date 2019-2020
 * @copyright MIT License
 */

#ifndef CENTURION_CURSOR_HEADER
#define CENTURION_CURSOR_HEADER

#include <SDL.h>

#include <memory>
#include <type_traits>

#include "centurion_api.hpp"
#include "centurion_utils.hpp"
#include "point.hpp"
#include "surface.hpp"

namespace centurion {

/**
 * @brief Represents the various available system cursors.
 *
 * @details Mirrors the values of the `SDL_SystemCursor` enum.
 *
 * @since 4.0.0
 *
 * @see `SDL_SystemCursor`
 *
 * @headerfile cursor.hpp
 */
enum class SystemCursor {
  Arrow = SDL_SYSTEM_CURSOR_ARROW,
  IBeam = SDL_SYSTEM_CURSOR_IBEAM,
  Wait = SDL_SYSTEM_CURSOR_WAIT,
  Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
  WaitArrow = SDL_SYSTEM_CURSOR_WAITARROW,
  Arrow_NW_SE = SDL_SYSTEM_CURSOR_SIZENWSE,
  Arrow_NE_SW = SDL_SYSTEM_CURSOR_SIZENESW,
  Arrow_W_E = SDL_SYSTEM_CURSOR_SIZEWE,
  Arrow_N_S = SDL_SYSTEM_CURSOR_SIZENS,
  ArrowAll = SDL_SYSTEM_CURSOR_SIZEALL,
  No = SDL_SYSTEM_CURSOR_NO,
  Hand = SDL_SYSTEM_CURSOR_HAND
};

/**
 * @brief Indicates whether or not two system cursor values are the same.
 *
 * @param lhs the left-hand side system cursor value.
 * @param rhs the right-hand side system cursor value.
 *
 * @return `true` if the system cursor values are the same; `false` otherwise.
 *
 * @since 4.0.0
 */
[[nodiscard]] inline constexpr auto operator==(SystemCursor lhs,
                                               SDL_SystemCursor rhs) noexcept
    -> bool
{
  return static_cast<SDL_SystemCursor>(lhs) == rhs;
}

/**
 * @copydoc operator==(SystemCursor, SDL_SystemCursor)
 */
[[nodiscard]] inline constexpr auto operator==(SDL_SystemCursor lhs,
                                               SystemCursor rhs) noexcept
    -> bool
{
  return rhs == lhs;
}

/**
 * @brief Indicates whether or not two system cursor values aren't the same.
 *
 * @param lhs the left-hand side system cursor value.
 * @param rhs the right-hand side system cursor value.
 *
 * @return `true` if the system cursor values aren't the same; `false`
 * otherwise.
 *
 * @since 4.0.0
 */
[[nodiscard]] inline constexpr auto operator!=(SystemCursor lhs,
                                               SDL_SystemCursor rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/**
 * @copydoc operator!=(SystemCursor, SDL_SystemCursor)
 */
[[nodiscard]] inline constexpr auto operator!=(SDL_SystemCursor lhs,
                                               SystemCursor rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

/**
 * @class Cursor
 * @brief Represents a mouse cursor.
 *
 * @details Cursors can be created from various preset shapes or from images
 * (surfaces).
 *
 * @since 4.0.0
 *
 * @headerfile cursor.hpp
 */
class Cursor final {
 public:
  /**
   * @brief Creates a cursor based on the specified cursor type.
   *
   * @param id the cursor type that will be used.
   *
   * @throws CenturionException if the cursor cannot be created.
   *
   * @since 4.0.0
   */
  CENTURION_API
  explicit Cursor(SystemCursor id);

  /**
   * @brief Creates a cursor based on the supplied `SDL_Cursor`.
   *
   * @details The ownership of the supplied pointer will be claimed by the
   * created cursor instance.
   *
   * @param cursor a pointer to an `SDL_Cursor` that will be adopted.
   *
   * @throws CenturionException if the supplied pointer is null.
   *
   * @since 4.0.0
   */
  CENTURION_API
  explicit Cursor(gsl::owner<SDL_Cursor*> cursor);

  /**
   * @brief Creates a cursor based on the supplied surface.
   *
   * @details The supplied hotspot must be within the area of the supplied
   * surface.
   *
   * @param surface the surface that will represent the cursor.
   * @param hotspot the point used to determine where the mouse
   * actually is.
   *
   * @throws CenturionException if the cursor cannot be created.
   *
   * @since 4.0.0
   */
  CENTURION_API
  explicit Cursor(const Surface& surface, const IPoint& hotspot);

  /**
   * @brief Creates a cursor by moving the supplied cursor.
   *
   * @param other the cursor that will be moved.
   *
   * @since 4.0.0
   */
  CENTURION_API
  Cursor(Cursor&& other) noexcept;

  Cursor(const Cursor&) = delete;

  CENTURION_API
  ~Cursor() noexcept;

  /**
   * @brief Moves the contents of the supplied cursor into this cursor.
   *
   * @param other the cursor that will be moved.
   *
   * @return the updated cursor.
   *
   * @since 4.0.0
   */
  CENTURION_API
  auto operator=(Cursor&& other) noexcept -> Cursor&;

  auto operator=(const Cursor&) -> Cursor& = delete;

  /**
   * @copydoc Cursor(SystemCursor)
   */
  CENTURION_QUERY
  static auto unique(SystemCursor id) -> std::unique_ptr<Cursor>;

  /**
   * @copydoc Cursor(gsl::owner<SDL_Cursor*>)
   */
  CENTURION_QUERY
  static auto unique(gsl::owner<SDL_Cursor*> cursor) -> std::unique_ptr<Cursor>;

  /**
   * @copydoc Cursor(const Surface&, const IPoint&)
   */
  CENTURION_QUERY
  static auto unique(const Surface& surface, const IPoint& hotspot)
      -> std::unique_ptr<Cursor>;

  /**
   * @copydoc Cursor(SystemCursor)
   */
  CENTURION_QUERY
  static auto shared(SystemCursor id) -> std::shared_ptr<Cursor>;

  /**
   * @copydoc Cursor(gsl::owner<SDL_Cursor*>)
   */
  CENTURION_QUERY
  static auto shared(gsl::owner<SDL_Cursor*> cursor) -> std::shared_ptr<Cursor>;

  /**
   * @copydoc Cursor(const Surface&, const IPoint&)
   */
  CENTURION_QUERY
  static auto shared(const Surface& surface, const IPoint& hotspot)
      -> std::shared_ptr<Cursor>;

  /**
   * Forces a cursor redraw.
   *
   * @since 4.0.0
   */
  CENTURION_API
  static void force_redraw() noexcept;

  /**
   * Resets the cursor to the system default.
   *
   * @since 4.0.0
   */
  CENTURION_API
  static void reset() noexcept;

  /**
   * Sets whether or not the cursor is visible.
   *
   * @param visible true if the cursor should be made visible; false otherwise.
   * @since 4.0.0
   */
  CENTURION_API
  static void set_visible(bool visible) noexcept;

  /**
   * Indicates whether or not the cursor is visible.
   *
   * @return true if the cursor is visible; false otherwise.
   * @since 4.0.0
   */
  CENTURION_QUERY
  static auto visible() noexcept -> bool;

  /**
   * @brief Makes the cursor the used cursor.
   *
   * @since 4.0.0
   */
  CENTURION_API
  void enable() noexcept;

  /**
   * @brief Indicates whether or not the cursor is currently being used.
   *
   * @note This method only checks if the currently used SDL cursor is the
   * same instance referenced in the invoked Centurion cursor. In other
   * words, if two cursors of the same type has been created, and one of them
   * is enabled, then this method could still return false even if the
   * cursors have the same type.
   *
   * @return `true` if the cursor is being used; `false` otherwise.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto is_enabled() const noexcept -> bool;

  /**
   * Returns a pointer to the internal SDL_Cursor. Use of this method is
   * not recommended, since it purposefully breaks const-correctness. However
   * it is useful since many SDL calls use non-const pointers even when no
   * change will be applied.
   *
   * @return a pointer to the internal SDL_Cursor.
   * @since 4.0.0
   */
  [[nodiscard]] auto get() const noexcept -> SDL_Cursor* { return m_cursor; }

 private:
  SDL_Cursor* m_cursor = nullptr;
  SDL_Surface* m_surface = nullptr;

  /**
   * Destroys the components in the Cursor instance.
   *
   * @since 4.0.0
   */
  void destroy() noexcept;

  /**
   * Moves the contents of the supplied cursor instance into this instance.
   *
   * @param other the instance that will be moved.
   * @since 4.0.0
   */
  void move(Cursor&& other) noexcept;
};

static_assert(std::is_final_v<Cursor>);
static_assert(std::is_nothrow_move_constructible_v<Cursor>);
static_assert(std::is_nothrow_move_assignable_v<Cursor>);
static_assert(!std::is_copy_constructible_v<Cursor>);
static_assert(!std::is_copy_assignable_v<Cursor>);

}  // namespace centurion

#ifdef CENTURION_HEADER_ONLY
#include "cursor.cpp"
#endif

#endif  // CENTURION_CURSOR_HEADER