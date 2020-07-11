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
 * @file centurion.hpp
 *
 * @brief The main header file for the library.
 *
 * @details Provides the `centurion_lib` and `centurion_config` classes, which
 * are related to the initialization of the library.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_HEADER
#define CENTURION_HEADER

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "centurion_api.hpp"
#include "centurion_types.hpp"

/**
 * @namespace centurion
 *
 * @brief The top-level namespace that all components of the library reside in.
 */
namespace centurion {

/**
 * @struct centurion_config
 *
 * @brief Used to specify how the library is initialized.
 *
 * @details All fields are initialized to the default values used by the
 * library.
 *
 * @since 4.0.0
 *
 * @var centurion_config::initCore
 * Indicates whether or not the SDL2 core is initialized.
 *
 * @var centurion_config::initImage
 * Indicates whether or not SDL2_image is initialized.
 *
 * @var centurion_config::initMixer
 * Indicates whether or not SDL2_mixer is initialized.
 *
 * @var centurion_config::initTTF
 * Indicates whether or not SDL2_ttf is initialized.
 *
 * @var centurion_config::coreFlags
 * Flags passed on to `SDL_Init()`, if @ref centurion_config.initCore is `true`.
 *
 * @var centurion_config::imageFlags
 * Flags passed on to `IMG_Init()`, if @ref centurion_config.initImage is
 * `true`.
 *
 * @var centurion_config::mixerFlags
 * Flags passed on to `Mix_Init()`, if @ref centurion_config.initMixer is
 * `true`.
 *
 * @var centurion_config::mixerFreq
 * The frequency used by SDL2_mixer, if @ref centurion_config.initMixer is
 * `true`.
 *
 * @var centurion_config::mixerFormat
 * The format used by SDL2_mixer, if @ref centurion_config.initMixer is `true`.
 *
 * @var centurion_config::mixerChannels
 * The amount of channels used by SDL2_mixer, if @ref centurion_config.initMixer
 * is `true`.
 *
 * @var centurion_config::mixerChunkSize
 * The chunk size used by SDL2_mixer, if @ref centurion_config.initMixer is
 * `true`.
 *
 * @headerfile centurion.hpp
 */
struct centurion_config final {
  bool initCore = true;
  bool initImage = true;
  bool initMixer = true;
  bool initTTF = true;

  u32 coreFlags = SDL_INIT_EVERYTHING;

  int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;

  int mixerFlags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MID |
                   MIX_INIT_MOD | MIX_INIT_OPUS;

  int mixerFreq = MIX_DEFAULT_FREQUENCY;
  u16 mixerFormat = MIX_DEFAULT_FORMAT;
  int mixerChannels = MIX_DEFAULT_CHANNELS;
  int mixerChunkSize = 4096;
};

/**
 * @class centurion_lib
 *
 * @brief Used to initialize and de-initialize the library.
 *
 * @since 3.0.0
 *
 * @note The signature of the main-method must be `ìnt(int, char**)` when
 * using the Centurion library!
 *
 * @par Examples
 * This is how you should initialize the library.
 * @code{.cpp}
 * #include <centurion_as_ctn.hpp>
 *
 * int main(int, char**)
 * {
 *   ctn::centurion_lib c;
 *
 *   // The library is now initialized, proceed to using it!
 *
 *   return 0;
 * }
 * @endcode
 * You can also manually configure exactly how the library is initialized, using
 * the @link centurion::centurion_config @endlink struct.
 * @code{.cpp}
 * #include <centurion_as_ctn.hpp>
 *
 * int main(int, char**)
 * {
 *   ctn::centurion_config cfg;
 *   cfg.initMixer = false;
 *   // ...
 *
 *   ctn::centurion_lib c{cfg};
 *
 *   // The library is now initialized, proceed to using it!
 *
 *   return 0;
 * }
 * @endcode
 *
 * @headerfile centurion.hpp
 */
class centurion_lib final {
 public:
  /**
   * @brief Initializes the library.
   *
   * @pre there mustn't exist any other instances of this class at the time of
   * invocation of this constructor.
   *
   * @throws centurion_exception if any of the SDL libraries can't be loaded.
   *
   * @since 3.0.0
   */
  CENTURION_API
  centurion_lib();

  /**
   * @brief Initializes the library according to the supplied configuration.
   *
   * @pre there mustn't exist any other instances of this class at the time of
   * invocation of this constructor.
   *
   * @param cfg the configuration spec, determines what gets initialized.
   *
   * @throws centurion_exception if any of the SDL libraries can't be loaded.
   *
   * @since 4.0.0
   */
  CENTURION_API
  explicit centurion_lib(const centurion_config& cfg);

  /**
   * @brief Closes the Centurion library.
   *
   * @since 3.0.0
   */
  CENTURION_API
  ~centurion_lib() noexcept;

  centurion_lib(const centurion_lib&) = delete;

  centurion_lib(centurion_lib&&) = delete;

  auto operator=(const centurion_lib&) -> centurion_lib& = delete;

  auto operator=(centurion_lib&&) -> centurion_lib& = delete;

 private:
  centurion_config cfg;

  void init_sdl();

  void init_ttf();

  void init_img();

  void init_mix();

  void init();

  void close() noexcept;
};

}  // namespace centurion

#ifdef CENTURION_HEADER_ONLY
#include "centurion.cpp"
#endif

#endif  // CENTURION_HEADER
