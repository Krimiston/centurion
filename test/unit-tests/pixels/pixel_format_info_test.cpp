#include <gtest/gtest.h>

#include <iostream>  // clog
#include <memory>    // unique_ptr

#include "pixels.hpp"
#include "system.hpp"
#include "video/color.hpp"

class PixelFormatInfoTest : public testing::Test {
 protected:
  static void SetUpTestSuite()
  {
    info = std::make_unique<cen::PixelFormatInfo>(cen::PixelFormat::RGBA8888);
  }

  static void TearDownTestSuite() { info.reset(); }

  inline static std::unique_ptr<cen::PixelFormatInfo> info;
};

TEST_F(PixelFormatInfoTest, PointerConstructor)
{
  static_assert(!noexcept(cen::PixelFormatInfo{nullptr}));
  static_assert(noexcept(cen::PixelFormatInfoHandle{nullptr}));

  ASSERT_THROW(cen::PixelFormatInfo{nullptr}, cen::Error);
  ASSERT_NO_THROW(cen::PixelFormatInfoHandle{nullptr});
}

TEST_F(PixelFormatInfoTest, FormatConstructor)
{
  const auto invalid = static_cast<cen::PixelFormat>(0xFFFFFFFF);
  ASSERT_THROW(cen::PixelFormatInfo{invalid}, cen::SDLError);
}

TEST_F(PixelFormatInfoTest, HandleFromOwner)
{
  const cen::PixelFormatInfoHandle handle{*info};
  ASSERT_TRUE(handle);
}

TEST_F(PixelFormatInfoTest, GetFormat)
{
  ASSERT_EQ(cen::PixelFormat::RGBA8888, info->GetFormat());
}

TEST_F(PixelFormatInfoTest, GetName)
{
  ASSERT_STREQ(SDL_GetPixelFormatName(info->get()->format), info->GetName());
}

TEST_F(PixelFormatInfoTest, RGBToPixel)
{
  constexpr auto color = cen::colors::hot_pink;
  ASSERT_EQ(SDL_MapRGB(info->get(), color.GetRed(), color.GetGreen(), color.GetBlue()),
            info->RGBToPixel(color));
}

TEST_F(PixelFormatInfoTest, RGBAToPixel)
{
  constexpr auto color = cen::colors::honey_dew;
  ASSERT_EQ(SDL_MapRGBA(info->get(),
                        color.GetRed(),
                        color.GetGreen(),
                        color.GetBlue(),
                        color.GetAlpha()),
            info->RGBAToPixel(color));
}

TEST_F(PixelFormatInfoTest, PixelToRGB)
{
  constexpr auto color = cen::colors::hot_pink;
  const Uint32 pixel =
      (color.GetRed() << 24u) | (color.GetGreen() << 16u) | (color.GetBlue() << 8u);
  ASSERT_EQ(color, info->PixelToRGB(pixel));
}

TEST_F(PixelFormatInfoTest, PixelToRGBA)
{
  constexpr auto color = cen::colors::aquamarine;
  const Uint32 pixel = (color.GetRed() << 24u) | (color.GetGreen() << 16u) |
                       (color.GetBlue() << 8u) | (color.GetAlpha() << 0u);
  ASSERT_EQ(color, info->PixelToRGBA(pixel));
}

TEST_F(PixelFormatInfoTest, ToString)
{
  std::clog << *info << '\n';
}
