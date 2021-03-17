#ifndef CENTURION_MOUSE_MOTION_EVENT_HEADER
#define CENTURION_MOUSE_MOTION_EVENT_HEADER

#include <SDL.h>

#include "../centurion_cfg.hpp"
#include "../input/mouse_button.hpp"
#include "../misc/integers.hpp"
#include "common_event.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace cen {

/// \addtogroup event
/// \{

/**
 * \class mouse_motion_event
 *
 * \brief Represents events that are triggered by the movement of the mouse.
 *
 * \see `SDL_MouseMotionEvent`
 *
 * \since 4.0.0
 *
 * \headerfile mouse_motion_event.hpp
 */
class mouse_motion_event final : public common_event<SDL_MouseMotionEvent>
{
 public:
  /**
   * \brief Creates a mouse motion event.
   *
   * \since 4.0.0
   */
  mouse_motion_event() noexcept : common_event{event_type::mouse_motion}
  {}

  /**
   * \brief Creates a `MouseMotionEvent` that is based on the supplied SDL
   * event.
   *
   * \param event the SDL event that will be copied.
   *
   * \since 4.0.0
   */
  explicit mouse_motion_event(const SDL_MouseMotionEvent& event) noexcept
      : common_event{event}
  {}

  /**
   * \brief Sets the window ID associated with the event.
   *
   * \param id the window ID associated with the event.
   *
   * \since 4.0.0
   */
  void set_window_id(const u32 id) noexcept
  {
    m_event.windowID = id;
  }

  /**
   * \brief Sets the mouse instance ID.
   *
   * \param which the mouse instance ID.
   *
   * \since 4.0.0
   */
  void set_which(const u32 which) noexcept
  {
    m_event.which = which;
  }

  /**
   * \brief Sets the mouse button state associated with the event.
   *
   * \details The state could be any of `SDL_BUTTON_LMASK`, `SDL_BUTTON_MMASK`,
   * `SDL_BUTTON_RMASK`, `SDL_BUTTON_X1MASK` and `SDL_BUTTON_X2MASK` OR'd
   * together.
   *
   * \param state the mouse button state associated with the event.
   *
   * \since 4.0.0
   */
  void set_state(const u32 state) noexcept
  {
    m_event.state = state;
  }

  /**
   * \brief Sets the x-coordinate of the mouse relative to the mouse that is
   * associated with the event.
   *
   * \param x the x-coordinate of the mouse.
   *
   * \since 4.0.0
   */
  void set_x(const i32 x) noexcept
  {
    m_event.x = x;
  }

  /**
   * \brief Sets the y-coordinate of the mouse relative to the mouse that is
   * associated with the event.
   *
   * \param y the y-coordinate of the mouse.
   *
   * \since 4.0.0
   */
  void set_y(const i32 y) noexcept
  {
    m_event.y = y;
  }

  /**
   * \brief Sets the value of the relative motion of the mouse along the x-axis.
   *
   * \param dx the value of the relative motion of the mouse along the x-axis.
   *
   * \since 4.0.0
   */
  void set_dx(const i32 dx) noexcept
  {
    m_event.xrel = dx;
  }

  /**
   * \brief Sets the value of the relative motion of the mouse along the y-axis.
   *
   * \param dy the value of the relative motion of the mouse along the y-axis.
   *
   * \since 4.0.0
   */
  void set_dy(const i32 dy) noexcept
  {
    m_event.yrel = dy;
  }

  /**
   * \brief Returns the window ID associated with the event.
   *
   * \return the window ID associated with the event.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto window_id() const noexcept -> u32
  {
    return m_event.windowID;
  }

  /**
   * \brief Returns the mouse instance ID, or `SDL_TOUCH_MOUSEID`.
   *
   * \details The returned value might be `SDL_TOUCH_MOUSEID` for events that
   * were generated by a touch input device.
   *
   * \return the mouse instance ID, or `SDL_TOUCH_MOUSEID`.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto which() const noexcept -> u32
  {
    return m_event.which;
  }

  /**
   * \brief Returns a bitmask for the current mouse button state.
   *
   * \remark If you want to check if a specific mouse button is pressed or
   * released, a better alternative would be to use the `pressed(MouseButton)
   * ` method.
   *
   * \return a bitmask for the current mouse button state.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto state() const noexcept -> u32
  {
    return m_event.state;
  }

  /**
   * \brief Indicates whether or not a mouse button is pressed.
   *
   * \param button the mouse button that will be checked.
   *
   * \return `true` if the specified mouse button is pressed; `false` otherwise.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto pressed(const mouse_button button) const noexcept -> bool
  {
    return m_event.state & SDL_BUTTON(static_cast<u32>(button));
  }

  /**
   * \brief Returns the x-coordinate of the mouse relative to the window.
   *
   * \return the x-coordinate of the mouse relative to the window.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto x() const noexcept -> i32
  {
    return m_event.x;
  }

  /**
   * \brief Returns the y-coordinate of the mouse relative to the window.
   *
   * \return the y-coordinate of the mouse relative to the window.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto y() const noexcept -> i32
  {
    return m_event.y;
  }

  /**
   * \brief Returns the relative motion of the mouse along the x-axis.
   *
   * \return the relative motion of the mouse along the x-axis.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto dx() const noexcept -> i32
  {
    return m_event.xrel;
  }

  /**
   * \brief Returns the relative motion of the mouse along the y-axis.
   *
   * \return the relative motion of the mouse along the y-axis.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto dy() const noexcept -> i32
  {
    return m_event.yrel;
  }
};

template <>
inline auto as_sdl_event(const common_event<SDL_MouseMotionEvent>& event)
    -> SDL_Event
{
  SDL_Event e;
  e.motion = event.get();
  return e;
}

/// \} End of group event

}  // namespace cen

#endif  // CENTURION_MOUSE_MOTION_EVENT_HEADER
