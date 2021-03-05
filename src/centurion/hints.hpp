/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Albin Johansson
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
 * \brief Provides utilities related to managing hints.
 *
 * \details Provides utilities related to managing hints ("configuration
 * variables" on the SDL2 wiki). Refer to the official SDL2 wiki or the
 * <code>SDL_hints.hpp</code> header for details regarding any specific hint
 * type.
 *
 * \todo `WindowsIntResourceIcon`, `WindowsIntResourceIconSmall`,
 * `X11WindowVisualID` are string hints because the types of their values
 * isn't known. Should be fixed if the type isn't actually string.
 *
 * \todo C++20: Make callback signature depend on the `UserData` and the type
 * of the associated hint, so that the values supplied to the callback aren't
 * always strings.
 *
 * \todo Document all of the hint classes.
 *
 * \file hints.hpp
 *
 * \since 4.1.0
 *
 * \author Albin Johansson
 *
 * \date 2019-2021
 *
 * \copyright MIT License
 */

#ifndef CENTURION_HINTS_HEADER
#define CENTURION_HINTS_HEADER

#include <SDL.h>

#include <optional>     // optional
#include <string>       // string
#include <type_traits>  // is_same_v, ...
#include <utility>      // pair, make_pair

#include "centurion_cfg.hpp"
#include "detail/hints_impl.hpp"
#include "exception.hpp"
#include "log.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace cen {

/**
 * \namespace cen::hint
 *
 * \ingroup configuration
 *
 * \brief Contains all hint types.
 *
 * \since 4.1.0
 */
namespace hint {

/// \addtogroup configuration
/// \{

struct render_driver;
struct audio_resampling_mode;
struct scale_quality;
struct framebuffer_acceleration;
struct audio_category;
struct wave_riff_chunk_size;
struct wave_truncation;
struct wave_fact_chunk;
struct logical_size_mode;

namespace qtwayland {
struct content_orientation;
}

namespace windows {
struct d3d_compiler;
}

/// \} End of group configuration

/// \cond FALSE

template <class Hint>
struct enum_hint_traits;

template <>
struct enum_hint_traits<render_driver> final
{
  enum class value
  {
    direct3d,
    opengl,
    opengles,
    opengles2,
    metal,
    software
  };
};

template <>
struct enum_hint_traits<audio_resampling_mode> final
{
  enum class value
  {
    normal = 0,
    fast = 1,
    medium = 2,
    best = 3
  };
};

template <>
struct enum_hint_traits<scale_quality> final
{
  enum class value
  {
    nearest = 0,
    linear = 1,
    best = 2
  };
};

template <>
struct enum_hint_traits<framebuffer_acceleration> final
{
  enum class value
  {
    off,
    on,
    direct3d,
    opengl,
    opengles,
    opengles2,
    metal,
    software
  };
};

template <>
struct enum_hint_traits<audio_category> final
{
  enum class value
  {
    ambient,
    playback
  };
};

template <>
struct enum_hint_traits<wave_riff_chunk_size> final
{
  enum class value
  {
    force,
    ignore_zero,
    ignore,
    maximum
  };
};

template <>
struct enum_hint_traits<wave_truncation> final
{
  enum class value
  {
    very_strict,
    strict,
    drop_frame,
    drop_block
  };
};

template <>
struct enum_hint_traits<wave_fact_chunk> final
{
  enum class value
  {
    truncate,
    strict,
    ignore_zero,
    ignore
  };
};

template <>
struct enum_hint_traits<logical_size_mode> final
{
  enum class value
  {
    letterbox,
    overscan
  };
};

template <>
struct enum_hint_traits<qtwayland::content_orientation> final
{
  enum class value
  {
    primary,
    portrait,
    landscape,
    inverted_portrait,
    inverted_landscape
  };
};

template <>
struct enum_hint_traits<windows::d3d_compiler> final
{
  enum class value
  {
    v46,
    v43,
    none
  };
};

/// \endcond

template <class Derived>
class enum_hint
{
 public:
  using value = typename enum_hint_traits<Derived>::value;

  template <typename T>
  constexpr static auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, value>;
  }

  static auto current_value() noexcept -> std::optional<value>
  {
    czstring hint = SDL_GetHint(Derived::name());
    if (!hint) {
      return std::nullopt;
    }
    return Derived::map.key_from(hint);
  }

  static auto to_string(const value value) -> std::string
  {
    return Derived::map.find(value);
  }
};

/// \addtogroup configuration
/// \{

/**
 * \class render_driver
 *
 * \brief Used to specify the render driver that will be used.
 *
 * \headerfile hints.hpp
 */
struct render_driver final : enum_hint<render_driver>
{
  static inline constexpr detail::string_map<value, 6> map{
      std::make_pair(value::direct3d, "direct3d"),
      std::make_pair(value::opengl, "opengl"),
      std::make_pair(value::opengles, "opengles"),
      std::make_pair(value::opengles2, "opengles2"),
      std::make_pair(value::metal, "metal"),
      std::make_pair(value::software, "software")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DRIVER;
  }
};

struct audio_resampling_mode final : enum_hint<audio_resampling_mode>
{
  static inline constexpr detail::string_map<value, 4> map{
      std::make_pair(value::normal, "default"),
      std::make_pair(value::fast, "fast"),
      std::make_pair(value::medium, "medium"),
      std::make_pair(value::best, "best")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_RESAMPLING_MODE;
  }
};

struct scale_quality final : enum_hint<scale_quality>
{
  static inline constexpr detail::string_map<value, 3> map{
      std::make_pair(value::nearest, "nearest"),
      std::make_pair(value::linear, "linear"),
      std::make_pair(value::best, "best")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_SCALE_QUALITY;
  }
};

struct framebuffer_acceleration final : enum_hint<framebuffer_acceleration>
{
  static inline constexpr detail::string_map<value, 8> map{
      std::make_pair(value::off, "0"),
      std::make_pair(value::on, "1"),
      std::make_pair(value::direct3d, "direct3d"),
      std::make_pair(value::opengl, "opengl"),
      std::make_pair(value::opengles, "opengles"),
      std::make_pair(value::opengles2, "opengles2"),
      std::make_pair(value::metal, "metal"),
      std::make_pair(value::software, "software")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_FRAMEBUFFER_ACCELERATION;
  }
};

struct audio_category final : enum_hint<audio_category>
{
  static inline constexpr detail::string_map<value, 2> map{
      std::make_pair(value::ambient, "ambient"),
      std::make_pair(value::playback, "playback")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_CATEGORY;
  }
};

struct wave_riff_chunk_size final : enum_hint<wave_riff_chunk_size>
{
  static inline constexpr detail::string_map<value, 4> map{
      std::make_pair(value::force, "force"),
      std::make_pair(value::ignore, "ignore"),
      std::make_pair(value::ignore_zero, "ignorezero"),
      std::make_pair(value::maximum, "maximum")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_RIFF_CHUNK_SIZE;
  }
};

struct wave_truncation final : enum_hint<wave_truncation>
{
  static inline constexpr detail::string_map<value, 4> map{
      std::make_pair(value::drop_block, "dropblock"),
      std::make_pair(value::drop_frame, "dropframe"),
      std::make_pair(value::strict, "strict"),
      std::make_pair(value::very_strict, "verystrict")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_TRUNCATION;
  }
};

struct wave_fact_chunk final : enum_hint<wave_fact_chunk>
{
  static inline constexpr detail::string_map<value, 4> map{
      std::make_pair(value::strict, "strict"),
      std::make_pair(value::ignore_zero, "ignorezero"),
      std::make_pair(value::ignore, "ignore"),
      std::make_pair(value::truncate, "truncate")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_FACT_CHUNK;
  }
};

struct logical_size_mode final : enum_hint<logical_size_mode>
{
  static inline constexpr detail::string_map<value, 2> map{
      std::make_pair(value::letterbox, "letterbox"),
      std::make_pair(value::overscan, "overscan")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_LOGICAL_SIZE_MODE;
  }
};

struct accelerometer_as_joystick final
    : detail::bool_hint<accelerometer_as_joystick>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ACCELEROMETER_AS_JOYSTICK;
  }
};

struct allow_top_most final : detail::bool_hint<allow_top_most>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ALLOW_TOPMOST;
  }
};

struct bmp_save_legacy_format final : detail::bool_hint<bmp_save_legacy_format>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_BMP_SAVE_LEGACY_FORMAT;
  }
};

struct double_buffer final : detail::bool_hint<double_buffer>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_DOUBLE_BUFFER;
  }
};

struct enable_steam_controllers final
    : detail::bool_hint<enable_steam_controllers>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ENABLE_STEAM_CONTROLLERS;
  }
};

struct grab_keyboard final : detail::bool_hint<grab_keyboard>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GRAB_KEYBOARD;
  }
};

struct idle_timer_disabled final : detail::bool_hint<idle_timer_disabled>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_IDLE_TIMER_DISABLED;
  }
};

struct ime_internal_editing final : detail::bool_hint<ime_internal_editing>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_IME_INTERNAL_EDITING;
  }
};

struct no_signal_handlers final : detail::bool_hint<no_signal_handlers>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_NO_SIGNAL_HANDLERS;
  }
};

struct opengl_es_driver final : detail::bool_hint<opengl_es_driver>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_OPENGL_ES_DRIVER;
  }
};

struct enable_opengl_shaders final : detail::bool_hint<enable_opengl_shaders>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_OPENGL_SHADERS;
  }
};

struct vsync final : detail::bool_hint<vsync>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_VSYNC;
  }
};

struct allow_screensaver final : detail::bool_hint<allow_screensaver>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_ALLOW_SCREENSAVER;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct video_external_context final : detail::bool_hint<video_external_context>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_EXTERNAL_CONTEXT;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 12)

struct disable_high_dpi final : detail::bool_hint<disable_high_dpi>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_HIGHDPI_DISABLED;
  }
};

struct minimize_on_focus_loss final : detail::bool_hint<minimize_on_focus_loss>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS;
  }
};

struct window_frame_usable_while_cursor_hidden final
    : detail::bool_hint<window_frame_usable_while_cursor_hidden>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN;
  }
};

struct render_batching final : detail::bool_hint<render_batching>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_BATCHING;
  }
};

struct return_key_hides_ime final : detail::bool_hint<return_key_hides_ime>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RETURN_KEY_HIDES_IME;
  }
};

struct touch_mouse_events final : detail::bool_hint<touch_mouse_events>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_TOUCH_MOUSE_EVENTS;
  }
};

struct mouse_touch_events final : detail::bool_hint<mouse_touch_events>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_TOUCH_EVENTS;
  }
};

struct tv_remote_as_joystick final : detail::bool_hint<tv_remote_as_joystick>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_TV_REMOTE_AS_JOYSTICK;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct display_usable_bounds final : detail::string_hint<display_usable_bounds>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_DISPLAY_USABLE_BOUNDS;
  }
};

#endif  // #if SDL_VERSION_ATLEAST(2, 0, 12)

struct orientations final : detail::string_hint<orientations>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ORIENTATIONS;
  }
};

struct window_share_pixel_format final
    : detail::string_hint<window_share_pixel_format>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT;
  }
};

struct event_logging final : detail::int_hint<event_logging>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_EVENT_LOGGING;
  }
};

struct thread_stack_size final : detail::unsigned_int_hint<thread_stack_size>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_THREAD_STACK_SIZE;
  }
};

struct timer_resolution final : detail::unsigned_int_hint<timer_resolution>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_TIMER_RESOLUTION;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 14)

struct preferred_locales final : detail::string_hint<preferred_locales>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_PREFERRED_LOCALES;
  }
};

struct thread_priority_policy final
    : detail::string_hint<thread_priority_policy>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    // This hint could be enum-based, but it isn't clear whether or not there
    // may be implementation specific identifiers other than those of the listed
    // pthread identifiers. So, we let this be a string-based hint.
    return SDL_HINT_THREAD_PRIORITY_POLICY;
  }
};

struct treat_time_critical_as_real_time final
    : detail::bool_hint<treat_time_critical_as_real_time>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_THREAD_FORCE_REALTIME_TIME_CRITICAL;
  }
};

struct audio_device_app_name final : detail::string_hint<audio_device_app_name>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_DEVICE_APP_NAME;
  }
};

struct audio_device_stream_name final
    : detail::string_hint<audio_device_stream_name>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_DEVICE_STREAM_NAME;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

namespace raspberrypi {

struct video_layer final : detail::int_hint<video_layer>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RPI_VIDEO_LAYER;
  }
};

}  // namespace raspberrypi

namespace appletv {

struct controller_ui_events final : detail::bool_hint<controller_ui_events>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_APPLE_TV_CONTROLLER_UI_EVENTS;
  }
};

struct remote_allow_rotation final : detail::bool_hint<remote_allow_rotation>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_APPLE_TV_REMOTE_ALLOW_ROTATION;
  }
};

}  // namespace appletv

namespace xinput {

struct is_enabled final : detail::bool_hint<is_enabled>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_XINPUT_ENABLED;
  }
};

struct use_old_joystick_mapping final
    : detail::bool_hint<use_old_joystick_mapping>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_XINPUT_USE_OLD_JOYSTICK_MAPPING;
  }
};

}  // namespace xinput

namespace emscripten {

struct keyboard_element final : detail::string_hint<keyboard_element>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 14)

struct asyncify final : detail::bool_hint<asyncify>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_EMSCRIPTEN_ASYNCIFY;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

}  // namespace emscripten

namespace qtwayland {

struct content_orientation final : enum_hint<content_orientation>
{
  static inline constexpr detail::string_map<value, 5> map{
      std::make_pair(value::primary, "primary"),
      std::make_pair(value::portrait, "portrait"),
      std::make_pair(value::landscape, "landscape"),
      std::make_pair(value::inverted_portrait, "inverted-portrait"),
      std::make_pair(value::inverted_landscape, "inverted-landscape")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_QTWAYLAND_CONTENT_ORIENTATION;
  }
};

struct window_flags final : detail::string_hint<window_flags>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_QTWAYLAND_WINDOW_FLAGS;
  }
};

}  // namespace qtwayland

namespace mouse {

struct focus_clickthrough final : detail::bool_hint<focus_clickthrough>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH;
  }
};

struct relative_mode_warp final : detail::bool_hint<relative_mode_warp>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_RELATIVE_MODE_WARP;
  }
};

struct double_click_time final : detail::int_hint<double_click_time>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_DOUBLE_CLICK_TIME;
  }
};

struct double_click_radius final : detail::int_hint<double_click_radius>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_DOUBLE_CLICK_RADIUS;
  }
};

struct normal_speed_scale final : detail::float_hint<normal_speed_scale>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_NORMAL_SPEED_SCALE;
  }
};

struct relative_speed_scale final : detail::float_hint<relative_speed_scale>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 14)

struct relative_scaling final : detail::bool_hint<relative_scaling>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_RELATIVE_SCALING;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

}  // namespace mouse

namespace d3d {

struct v11_debug final : detail::bool_hint<v11_debug>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DIRECT3D11_DEBUG;
  }
};

struct thread_safe final : detail::bool_hint<thread_safe>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DIRECT3D_THREADSAFE;
  }
};

}  // namespace d3d

namespace gamecontroller {

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct use_button_labels final : detail::bool_hint<use_button_labels>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS;
  }
};

struct type final : detail::string_hint<type>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERTYPE;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 12)

struct config final : detail::string_hint<config>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERCONFIG;
  }
};

struct config_file final : detail::string_hint<config_file>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERCONFIG_FILE;
  }
};

struct ignore_devices final : detail::string_hint<ignore_devices>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES;
  }
};

struct ignore_devices_except final : detail::string_hint<ignore_devices_except>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT;
  }
};

}  // namespace gamecontroller

namespace winrt {

struct privacy_policy_label final : detail::string_hint<privacy_policy_label>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_LABEL;
  }
};

struct privacy_policy_url final : detail::string_hint<privacy_policy_url>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_URL;
  }
};

struct handle_back_button final : detail::bool_hint<handle_back_button>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_HANDLE_BACK_BUTTON;
  }
};

}  // namespace winrt

namespace windows {

struct d3d_compiler final : enum_hint<d3d_compiler>
{
  static inline constexpr detail::string_map<value, 3> map{
      std::make_pair(value::v46, "d3dcompiler_46.dll"),
      std::make_pair(value::v43, "d3dcompiler_43.dll"),
      std::make_pair(value::none, "none")};

  constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_WIN_D3DCOMPILER;
  }
};

struct no_thread_naming final : detail::bool_hint<no_thread_naming>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING;
  }
};

struct enable_message_loop final : detail::bool_hint<enable_message_loop>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP;
  }
};

struct no_close_on_alt_f4 final : detail::bool_hint<no_close_on_alt_f4>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4;
  }
};

struct int_resource_icon final : detail::string_hint<int_resource_icon>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_INTRESOURCE_ICON;
  }
};

struct int_resource_icon_small final
    : detail::string_hint<int_resource_icon_small>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL;
  }
};

}  // namespace windows

namespace mac {

struct fullscreen_spaces final : detail::bool_hint<fullscreen_spaces>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES;
  }
};

struct background_app final : detail::bool_hint<background_app>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MAC_BACKGROUND_APP;
  }
};

struct ctrl_click_emulate_right_click final
    : detail::bool_hint<ctrl_click_emulate_right_click>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK;
  }
};

}  // namespace mac

namespace android {

struct block_on_pause final : detail::bool_hint<block_on_pause>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_BLOCK_ON_PAUSE;
  }
};

struct trap_back_button final : detail::bool_hint<trap_back_button>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_TRAP_BACK_BUTTON;
  }
};

struct apk_expansion_main_file_version final
    : detail::int_hint<apk_expansion_main_file_version>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION;
  }
};

struct apk_expansion_patch_file_version final
    : detail::int_hint<apk_expansion_patch_file_version>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_APK_EXPANSION_PATCH_FILE_VERSION;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 14)

struct pause_background_audio final : detail::bool_hint<pause_background_audio>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_BLOCK_ON_PAUSE_PAUSEAUDIO;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

}  // namespace android

namespace joystick {

struct allow_background_events final
    : detail::bool_hint<allow_background_events>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS;
  }
};

struct use_hidapi final : detail::bool_hint<use_hidapi>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI;
  }
};

struct use_hidapi_ps4 final : detail::bool_hint<use_hidapi_ps4>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_PS4;
  }
};

struct use_hidapi_ps4_rumble final : detail::bool_hint<use_hidapi_ps4_rumble>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 14)

struct use_hidapi_ps5 final : detail::bool_hint<use_hidapi_ps5>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_PS5;
  }
};

struct use_raw_input final : detail::bool_hint<use_raw_input>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_RAWINPUT;
  }
};

struct hidapi_correlate_xinput final
    : detail::bool_hint<hidapi_correlate_xinput>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_CORRELATE_XINPUT;
  }
};

struct linux_use_deadzones final : detail::bool_hint<linux_use_deadzones>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_LINUX_JOYSTICK_DEADZONES;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

struct use_hidapi_steam final : detail::bool_hint<use_hidapi_steam>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_STEAM;
  }
};

struct use_hidapi_switch final : detail::bool_hint<use_hidapi_switch>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_SWITCH;
  }
};

struct use_hidapi_xbox final : detail::bool_hint<use_hidapi_xbox>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_XBOX;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct use_hidapi_game_cube final : detail::bool_hint<use_hidapi_game_cube>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 12)

}  // namespace joystick

namespace x11 {

struct net_wm_ping final : detail::bool_hint<net_wm_ping>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_NET_WM_PING;
  }
};

struct net_wm_bypass_compositor final
    : detail::bool_hint<net_wm_bypass_compositor>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct force_egl final : detail::bool_hint<force_egl>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_FORCE_EGL;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 12)

struct xinerama final : detail::bool_hint<xinerama>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XINERAMA;
  }
};

struct xrandr final : detail::bool_hint<xrandr>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XRANDR;
  }
};

struct xvidmode final : detail::bool_hint<xvidmode>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XVIDMODE;
  }
};

#if SDL_VERSION_ATLEAST(2, 0, 12)

struct window_visual_id final : detail::string_hint<window_visual_id>
{
  [[nodiscard]] constexpr static auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_WINDOW_VISUALID;
  }
};

#endif  // SDL_VERSION_ATLEAST(2, 0, 12)

}  // namespace x11

}  // namespace hint

/**
 * \enum hint_priority
 *
 * \brief Provides three different priorities that can be specified when
 * setting the value of a hint.
 *
 * \since 4.1.0
 *
 * \see `SDL_HintPriority`
 *
 * \headerfile hints.hpp
 */
enum class hint_priority
{
  low = SDL_HINT_DEFAULT,       ///< The lowest possible priority.
  normal = SDL_HINT_NORMAL,     ///< The priority used by default by `set_hint`.
  override = SDL_HINT_OVERRIDE  ///< The highest priority.
};

/**
 * \brief Sets the value of the specified hint.
 *
 * \details This method will only accept values that are related to the
 * specified hint, supplying the wrong kind of value causes a compile-time
 * error. See the related hint class for more details about the associated
 * value type. However, whilst this method is type-safe, it doesn't ensure
 * that *correct* values are specified for the hints.
 *
 * \tparam Hint the type of the hint that will be modified.
 * \tparam priority the priority that will be used, defaults to `normal`.
 * \tparam Value the type of the hint value.
 *
 * \param value the new value that will be set for the specified hint.
 *
 * \return `true` if the hint was successfully set; `false` otherwise.
 *
 * \since 4.1.0
 */
template <typename Hint,
          hint_priority priority = hint_priority::normal,
          typename Value,
          typename = std::enable_if_t<Hint::template valid_arg<Value>()>>
auto set_hint(const Value& value) -> bool
{
  return static_cast<bool>(
      SDL_SetHintWithPriority(Hint::name(),
                              Hint::to_string(value).c_str(),
                              static_cast<SDL_HintPriority>(priority)));
}

/**
 * \brief Returns the current value of the specified hint.
 *
 * \note The returned value is a `std::optional` of the hint value type. Many
 * hints aren't actually set by default.
 *
 * \tparam Hint the type of the Hint to obtain the value of.
 *
 * \return the current value of the specified hint; `std::nullopt` if there is
 * no value set for the hint.
 *
 * \since 4.1.0
 */
template <typename Hint>
[[nodiscard]] auto get_hint() noexcept
{
  return Hint::current_value();
}

/**
 * \class hint_callback
 *
 * \brief Represents a handle for dealing with hint callbacks.
 *
 * \details This class provides methods for easily connecting and disconnecting
 * a callback from receiving updates.
 *
 * \tparam Hint the type of the associated hint. Should be one of the hint
 * types defined in the `hints.hpp` header.
 *
 * \tparam UserData the type of the user data, defaults to `void`.
 *
 * \since 4.1.0
 *
 * \headerfile hints.hpp
 */
template <typename Hint, typename UserData = void>
class hint_callback final
{
 public:
  /**
   * \brief Creates a `hint_callback`.
   *
   * \param callback the function object that will be called whenever the
   * associated hint is updated. The signature should be `void(void*,
   * czstring, czstring, czstring)`.
   * \param userData a pointer to some user data. Defaults to `nullptr`.
   *
   * \throws exception if the supplied function pointer is null.
   *
   * \since 4.1.0
   */
  explicit hint_callback(SDL_HintCallback callback,
                         UserData* userData = nullptr)
      : m_callback{callback}
      , m_userData{userData}
  {
    if (!callback) {
      throw exception{"Failed to create hint callback"};
    }
  }

  /**
   * \brief Registers the callback to be invoked whenever the associated hint is
   * updated.
   *
   * \see `SDL_AddHintCallback`
   *
   * \since 4.1.0
   */
  void connect() noexcept
  {
    SDL_AddHintCallback(Hint::name(),
                        m_callback,
                        static_cast<void*>(m_userData));
  }

  /**
   * \brief Unregisters the callback from being updated whenever the associated
   * hint is updated.
   *
   * \see `SDL_DelHintCallback`
   *
   * \since 4.1.0
   */
  void disconnect() noexcept
  {
    SDL_DelHintCallback(Hint::name(),
                        m_callback,
                        static_cast<void*>(m_userData));
  }

  /**
   * \brief Returns a pointer to the function that is invoked when the
   * associated hint is updated.
   *
   * \return a pointer to the function that is invoked when the associated
   * hint is updated.
   *
   * \since 4.1.0
   */
  [[nodiscard]] auto get() noexcept -> SDL_HintCallback
  {
    return m_callback;
  }

  /**
   * \brief Returns a pointer to the user data associated with the callback.
   *
   * \return a pointer to the user data associated with the callback, can be
   * null.
   *
   * \since 4.1.0
   */
  [[nodiscard]] auto user_data() const noexcept -> UserData*
  {
    return m_userData;
  }

 private:
  SDL_HintCallback m_callback;
  UserData* m_userData;
};

/**
 * \brief Adds a callback to observe changes of the value of the specified hint.
 *
 * \details A callback handle object is returned, which can be used to easily
 * disconnect the callback later. This function can be used with any function
 * object that is stateless, such as traditional function pointers and lambdas.
 * The simplest way to add a callback is with a lambda and no explicit user
 * data.
 *
 * \note The callback will be immediately invoked with the current value of
 * the hint.
 *
 * \note In a future version of centurion (that supports C++20), the
 * signature of the function object will be dependent on the `UserData` type.
 * Unfortunately, this isn't really doable with C++17. Since it requires
 * default-constructible stateless lambdas.
 *
 * \tparam Hint should one of the many hint types defined in this header.
 * However, all it requires is that the type provides a static method that
 * returns a `czstring`.
 * \tparam UserData the type of the user data, defaults to void.
 *
 * \param fun the function object that will be invoked when the hint is
 * updated. The signature should be `void(void*, czstring, czstring, czstring)`.
 * \param userData the user data to associate with the callback, defaults to
 * `nullptr`.
 *
 * \return a handle to the added callback.
 *
 * \since 4.1.0
 */
template <typename Hint, typename UserData = void>
auto add_hint_callback(SDL_HintCallback fun,
                       UserData* userData = nullptr) noexcept
    -> hint_callback<Hint, UserData>
{
  hint_callback<Hint, UserData> hintCallback{fun, userData};
  hintCallback.connect();
  return hintCallback;
}

/**
 * \brief Clears all stored hints.
 *
 * \see `SDL_ClearHints`
 *
 * \since 4.1.0
 */
inline void clear_hints() noexcept
{
  SDL_ClearHints();
}

/// \} End of group configuration

}  // namespace cen

#undef CENTURION_BOOL_HINT
#endif  // CENTURION_HINTS_HEADER