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
 * @file font.hpp
 *
 * @brief Provides the font API.
 *
 * @author Albin Johansson
 *
 * @date 2019-2020
 *
 * @copyright MIT License
 */

#ifndef CENTURION_FONT_HEADER
#define CENTURION_FONT_HEADER

#include <SDL_ttf.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "area.hpp"
#include "centurion_api.hpp"
#include "centurion_exception.hpp"
#include "centurion_utils.hpp"

namespace centurion {

/**
 * @struct GlyphMetrics
 *
 * @brief Provides metrics about a glyph in a font.
 *
 * @since 4.0.0
 *
 * @headerfile font.hpp
 */
struct GlyphMetrics final {
  int minX;
  int minY;
  int maxX;
  int maxY;
  int advance;
};

/**
 * @brief Represents a True Type font.
 *
 * @see `TTF_Font`
 *
 * @since 3.0.0
 *
 * @headerfile font.hpp
 */
class Font final {
 public:
  /**
   * @enum Hint
   *
   * @brief Provides different possible TrueType font hint values.
   *
   * @since 3.1.0
   *
   * @headerfile font.hpp
   */
  enum class Hint {
    Normal = TTF_HINTING_NORMAL,
    Light = TTF_HINTING_LIGHT,
    Mono = TTF_HINTING_MONO,
    None = TTF_HINTING_NONE
  };

  /**
   * @brief Creates a Font based on the `.ttf`-file at the specified path.
   *
   * @param file the file path of the TrueType font file.
   * @param size the font size, must be greater than zero.
   *
   * @throws CenturionException if the font cannot be loaded or if the supplied
   * size isn't greater than zero.
   *
   * @since 3.0.0
   */
  CENTURION_API
  Font(gsl::czstring file, int size);

  Font(const Font&) = delete;

  /**
   * @brief Creates a font by moving the supplied font.
   *
   * @param other the font that will be moved.
   *
   * @since 3.0.0
   */
  CENTURION_API
  Font(Font&& other) noexcept;

  auto operator=(const Font&) -> Font& = delete;

  /**
   * @brief Moves the supplied font into this font.
   *
   * @param other the font that will be moved.
   *
   * @return the font that absorbed the moved font.
   *
   * @since 3.0.0
   */
  CENTURION_API
  auto operator=(Font&& other) noexcept -> Font&;

  CENTURION_API
  ~Font() noexcept;

  /**
   * @copydoc Font(gsl::czstring, int)
   */
  CENTURION_QUERY
  static auto unique(gsl::czstring file, int size) -> std::unique_ptr<Font>;

  /**
   * @copydoc Font(gsl::czstring, int)
   */
  CENTURION_QUERY
  static auto shared(gsl::czstring file, int size) -> std::shared_ptr<Font>;

  /**
   * @brief Resets the style of the font.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void reset() noexcept;

  /**
   * @brief Sets whether the font is bold.
   *
   * @param bold `true` if the font should be bold; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void set_bold(bool bold) noexcept;

  /**
   * @brief Sets whether the font is italic.
   *
   * @param italic `true` if the font should be italic; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void set_italic(bool italic) noexcept;

  /**
   * @brief Sets whether the font is underlined.
   *
   * @param underlined `true` if the font should be underlined; `false`
   * otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void set_underlined(bool underlined) noexcept;

  /**
   * @brief Sets whether the font is strikethrough.
   *
   * @param strikethrough `true` if the font should be strikethrough; `false`
   * otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void set_strikethrough(bool strikethrough) noexcept;

  /**
   * @brief Sets the outlined property of the font.
   *
   * @param outlined `true` if the font should be outlined; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_API
  void set_outlined(bool outlined) noexcept;

  /**
   * @brief Sets the TrueType font hint of the font.
   *
   * @param hint the font hinting that will be used.
   *
   * @since 3.1.0
   */
  CENTURION_API
  void set_font_hinting(Hint hint) noexcept;

  /**
   * @brief Sets whether or not font kerning is allowed.
   *
   * @details Kerning is the process of adjusting the spacing between certain
   * characters in order to improve the appearance of a font.
   *
   * @param kerning `true` if kerning should be allowed; `false` otherwise.
   *
   * @since 4.0.0
   */
  CENTURION_API
  void set_kerning(bool kerning) noexcept;

  /**
   * @brief Returns the maximum height of a character in this font.
   *
   * @details This is usually the same as the point size.
   *
   * @return the maximum height of a character in this font.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto height() const noexcept -> int;

  /**
   * @brief Returns the offset from the baseline to the bottom of the font
   * characters.
   *
   * @details The returned value is negative, relative to the baseline.
   *
   * @return the offset from the baseline to the bottom of the font characters.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto descent() const noexcept -> int;

  /**
   * @brief Returns the offset from the baseline to the top of the font
   * characters.
   *
   * @details The returned value is positive, relative to the baseline.
   *
   * @return the offset from the baseline to the top of the font characters.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto ascent() const noexcept -> int;

  /**
   * @brief Returns the recommended pixel height of rendered text in the font.
   *
   * @details The returned value is usually larger than the height of the font.
   *
   * @return Returns the recommended pixel height of rendered text in the font.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto line_skip() const noexcept -> int;

  /**
   * @brief Returns the number of available font faces in the font.
   *
   * @return the number of available font faces in the font.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto font_faces() const noexcept -> int;

  /**
   * @brief Returns the TrueType font hinting of the font.
   *
   * @detauks This property is set to `Normal` by default.
   *
   * @return the TrueType font hinting of the font.
   *
   * @since 3.1.0
   */
  CENTURION_QUERY
  auto font_hinting() const noexcept -> Hint;

  /**
   * @brief Indicates whether or not kerning is being used.
   *
   * @return `true` if kerning is being used by the font; `false` otherwise.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto kerning() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is bold.
   *
   * @return `true` if the font is bold; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto bold() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is italic.
   *
   * @return `true` if the font is italic; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto italic() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is underlined.
   *
   * @return `true` if the font is underlined; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto underlined() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is a strikethrough font.
   *
   * @return `true` if the font is a strikethrough font; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto strikethrough() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is outlined.
   *
   * @return `true` if the font is outlined; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto outlined() const noexcept -> bool;

  /**
   * @brief Indicates whether or not the font is fixed width.
   *
   * @return `true` if the font is fixed width; `false` otherwise.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto is_fixed_width() const noexcept -> bool;

  /**
   * @brief Returns the kerning amount between two glyphs in the font, if
   * kerning would be enabled.
   *
   * @details In other words, you can use this method to obtain the
   * kerning amount between, for instance, the characters 'a' and 'V' if they
   * were to be rendered next to each other.
   *
   * @param firstGlyph the first glyph.
   * @param secondGlyph the second glyph.
   *
   * @return the kerning amount between to glyphs in the font.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto kerning_amount(Uint16 firstGlyph, Uint16 secondGlyph) const noexcept
      -> int;

  /**
   * @brief Indicates whether or not the specified glyph is available in the
   * font.
   *
   * @param glyph the unicode glyph that will be checked.
   *
   * @return `true` if the glyph is available in the font; `false` otherwise.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto is_glyph_provided(Uint16 glyph) const noexcept -> bool;

  /**
   * @brief Returns the metrics of the specified glyph in this font.
   *
   * @param glyph the glyph to obtain the metrics of.
   *
   * @return the metrics of the specified glyph; nothing if the metrics
   * couldn't be obtained.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto glyph_metrics(Uint16 glyph) const noexcept -> Optional<GlyphMetrics>;

  /**
   * @brief Returns the family name of the font.
   *
   * @return the family name of the font.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto family_name() const noexcept -> gsl::czstring;

  /**
   * @brief Returns the font face style name of the font.
   *
   * @note This information may not be available.
   *
   * @return the font face style name of the font; `nullptr` if it isn't
   * available.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto style_name() const noexcept -> gsl::czstring;

  /**
   * @brief Returns the width of the supplied string, if it was rendered using
   * the font.
   *
   * @param string the string to determine the width of.
   *
   * @return the width of the supplied string, if it was rendered using the
   * font. The returned value is 0 if the string is null.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto string_width(gsl::czstring string) const noexcept -> int;

  /**
   * @brief Returns the height of the supplied string, if it was rendered
   * using the font.
   *
   * @param string the string to determine the height of.
   *
   * @return the height of the supplied string, if it was rendered using the
   * font. The returned value is 0 if the string is null.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto string_height(gsl::czstring string) const noexcept -> int;

  /**
   * @brief Returns the size of the supplied string, if it was rendered using
   * the font.
   *
   * @param s the string to determine the size of.
   *
   * @return the size of the string, if it was rendered using the font. The
   * returned size is 0x0 if the supplied string is null.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  auto string_size(gsl::czstring s) const noexcept -> area_i;

  /**
   * @brief Returns the compile-time version of SDL2_ttf that is being used.
   *
   * @return the compile-time version of SDL2_ttf that is being used.
   *
   * @since 4.0.0
   */
  CENTURION_QUERY
  static auto ttf_version() noexcept -> SDL_version;

  /**
   * @brief Returns a textual representation of the font instance.
   *
   * @return a textual representation of the font instance.
   *
   * @since 3.0.0
   */
  CENTURION_QUERY
  auto to_string() const -> std::string;

  /**
   * @brief Returns the size of the font.
   *
   * @return the size of the font.
   *
   * @since 3.0.0
   */
  [[nodiscard]] auto size() const noexcept -> int { return m_size; }

  /**
   * @brief Returns a pointer to the associated `TTF_Font`.
   *
   * @warning Use of this method is not recommended, since it purposefully
   * breaks const-correctness. However, it's useful since many SDL calls use
   * non-const pointers even when no change will be applied.
   *
   * @return a pointer to the associated `TTF_Font`.
   *
   * @since 4.0.0
   */
  [[nodiscard]] auto get() const noexcept -> TTF_Font* { return m_font; }

  /**
   * @brief Converts to `TTF_Font*`.
   *
   * @return a pointer to the associated `TTF_Font` instance.
   *
   * @since 3.0.0
   */
  [[nodiscard]] explicit operator TTF_Font*() noexcept { return m_font; }

  /**
   * @brief Converts to `const TTF_Font*`.
   *
   * @return a pointer to the associated `TTF_Font` instance.
   *
   * @since 3.0.0
   */
  [[nodiscard]] explicit operator const TTF_Font*() const noexcept
  {
    return m_font;
  }

 private:
  TTF_Font* m_font = nullptr;
  int m_style = 0;
  int m_size = 0;

  /**
   * @brief Destroys the resources associated with the font.
   *
   * @since 4.0.0
   */
  void destroy() noexcept;

  /**
   * @brief Moves the contents of the supplied font instance into this instance.
   *
   * @param other the instance that will be moved.
   *
   * @since 4.0.0
   */
  void move(Font&& other) noexcept;

  /**
   * @brief Enables the font style associated with the supplied bit mask.
   *
   * @details The possible values are `TTF_STYLE_BOLD`, `TTF_STYLE_ITALIC`,
   * `TTF_STYLE_UNDERLINE` and `TTF_STYLE_STRIKETHROUGH`.
   *
   * @param mask the bit mask of the font style to enable.
   *
   * @since 3.0.0
   */
  void add_style(int mask) noexcept;

  /**
   * @brief Removes the font style associated with the supplied bit mask.
   *
   * @details The possible values are `TTF_STYLE_BOLD`, `TTF_STYLE_ITALIC`,
   * `TTF_STYLE_UNDERLINE` and `TTF_STYLE_STRIKETHROUGH`.
   *
   * @param mask the bit mask of the font style to disable.
   *
   * @since 3.0.0
   */
  void remove_style(int mask) noexcept;
};

static_assert(std::is_final_v<Font>);
static_assert(std::is_nothrow_move_constructible_v<Font>);
static_assert(std::is_nothrow_move_assignable_v<Font>);
static_assert(!std::is_copy_constructible_v<Font>);
static_assert(!std::is_copy_assignable_v<Font>);

}  // namespace centurion

#ifdef CENTURION_HEADER_ONLY
#include "font.cpp"
#endif

#endif  // CENTURION_FONT_HEADER