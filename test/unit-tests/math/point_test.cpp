#include "math/point.hpp"

#include <gtest/gtest.h>

#include <cmath>        // abs, sqrt
#include <iostream>     // cout
#include <type_traits>  // ...

#include "core/log.hpp"
#include "serialization_utils.hpp"

static_assert(std::is_nothrow_default_constructible_v<cen::ipoint>);
static_assert(std::is_nothrow_destructible_v<cen::ipoint>);
static_assert(std::is_nothrow_copy_constructible_v<cen::ipoint>);
static_assert(std::is_nothrow_copy_assignable_v<cen::ipoint>);
static_assert(std::is_nothrow_move_constructible_v<cen::ipoint>);
static_assert(std::is_nothrow_move_assignable_v<cen::ipoint>);

static_assert(std::is_nothrow_default_constructible_v<cen::fpoint>);
static_assert(std::is_nothrow_destructible_v<cen::fpoint>);
static_assert(std::is_nothrow_copy_constructible_v<cen::fpoint>);
static_assert(std::is_nothrow_copy_assignable_v<cen::fpoint>);
static_assert(std::is_nothrow_move_constructible_v<cen::fpoint>);
static_assert(std::is_nothrow_move_assignable_v<cen::fpoint>);

TEST(Point, Point)
{
  // clang-format off
  static_assert(std::is_same_v<decltype(cen::point(1, 1)), cen::ipoint>);
  static_assert(std::is_same_v<decltype(cen::point(1u, 1u)), cen::ipoint>);
  static_assert(std::is_same_v<decltype(cen::point(1.0f, 1.0f)), cen::fpoint>);
  static_assert(std::is_same_v<decltype(cen::point(1.0, 1.0)), cen::fpoint>);
  // clang-format on

  const auto ipoint = cen::point(123, 456);
  const auto fpoint = cen::point(12.3f, 45.6f);

  EXPECT_EQ(123, ipoint.x());
  EXPECT_EQ(456, ipoint.y());
  EXPECT_EQ(12.3f, fpoint.x());
  EXPECT_EQ(45.6f, fpoint.y());
}

TEST(Point, DistanceUnitXStep)
{
  const cen::ipoint a{0, 123};
  const cen::ipoint b{1, 123};

  EXPECT_EQ(cen::distance(a, b), 1);
  EXPECT_EQ(cen::distance(b, a), 1);
}

TEST(Point, DistanceUnitYStep)
{
  const cen::ipoint a{123, 0};
  const cen::ipoint b{123, 1};

  EXPECT_EQ(cen::distance(a, b), 1);
  EXPECT_EQ(cen::distance(b, a), 1);
}

TEST(Point, Distance)
{
  const cen::fpoint a{189.2f, 86.9f};
  const cen::fpoint b{66.5f, 36.6f};

  const auto expected = std::sqrt(std::abs(a.x() - b.x()) + std::abs(a.y() - b.y()));

  EXPECT_EQ(cen::distance(a, b), expected);
  EXPECT_EQ(cen::distance(b, a), expected);
}

TEST(Point, EqualityOperatorReflexivity)
{
  const cen::fpoint point;
  EXPECT_EQ(point, point);
}

TEST(Point, EqualityOperatorComparisonSame)
{
  const cen::fpoint fst{211.5f, 823.1f};
  const cen::fpoint snd{fst};
  EXPECT_EQ(fst, snd);
  EXPECT_EQ(snd, fst);
  EXPECT_FALSE(fst != snd);
  EXPECT_FALSE(snd != fst);
}

TEST(Point, EqualityOperatorComparisonDifferent)
{
  const cen::fpoint fst{531.5f, 8313.4f};
  const cen::fpoint snd{34.2f, 173.3f};
  EXPECT_NE(fst, snd);
  EXPECT_NE(snd, fst);
}

TEST(Point, InequalityOperatorSelf)
{
  const cen::fpoint point;
  EXPECT_FALSE(point != point);
}

TEST(Point, InequalityOperatorDifferent)
{
  const cen::fpoint fst{8392.5f, 12452.4f};
  const cen::fpoint snd{5236.2f, 321.3f};
  EXPECT_NE(fst, snd);
  EXPECT_NE(snd, fst);
}

TEST(Point, InequalityOperatorEqual)
{
  const cen::fpoint fst{211.5f, 823.1f};
  const cen::fpoint snd{fst};
  EXPECT_FALSE(fst != snd);
  EXPECT_FALSE(snd != fst);
}

TEST(Point, AdditionOperator)
{
  const cen::fpoint fst{62.4f, 381.3f};
  const cen::fpoint snd{779.3f, 819.3f};

  const auto expectedX = fst.x() + snd.x();
  const auto expectedY = fst.y() + snd.y();

  const auto fstSnd = fst + snd;
  EXPECT_EQ(fstSnd.x(), expectedX);
  EXPECT_EQ(fstSnd.y(), expectedY);

  const auto sndFst = snd + fst;
  EXPECT_EQ(sndFst.x(), expectedX);
  EXPECT_EQ(sndFst.y(), expectedY);
}

TEST(Point, SubtractionOperator)
{
  const cen::fpoint fst{673, 123};
  const cen::fpoint snd{-547, 451};

  const auto fstSnd = fst - snd;
  EXPECT_EQ(fstSnd.x(), fst.x() - snd.x());
  EXPECT_EQ(fstSnd.y(), fst.y() - snd.y());

  const auto sndFst = snd - fst;
  EXPECT_EQ(sndFst.x(), snd.x() - fst.x());
  EXPECT_EQ(sndFst.y(), snd.y() - fst.y());

  EXPECT_NE(fstSnd, sndFst);
}

TEST(Point, IPointToFPoint)
{
  const cen::ipoint source{684, 912};
  const auto result = cen::cast<cen::fpoint>(source);

  EXPECT_EQ(result.x(), static_cast<float>(source.x()));
  EXPECT_EQ(result.y(), static_cast<float>(source.y()));
}

TEST(Point, FPointToIPoint)
{
  const cen::fpoint source{58.8f, 123.4f};
  const auto result = cen::cast<cen::ipoint>(source);

  EXPECT_EQ(result.x(), static_cast<int>(source.x()));
  EXPECT_EQ(result.y(), static_cast<int>(source.y()));
}

TEST(Point, ConversionToPointer)
{
  cen::fpoint point{3813.3f, 892.5f};

  auto* ptr = static_cast<SDL_FPoint*>(point);
  EXPECT_EQ(point.x(), ptr->x);
  EXPECT_EQ(point.y(), ptr->y);

  const auto* cptr = static_cast<const SDL_FPoint*>(point);
  EXPECT_EQ(point.x(), cptr->x);
  EXPECT_EQ(point.y(), cptr->y);
}

TEST(Point, Get)
{
  const cen::fpoint point{3923.3f, 7718.1f};
  const auto& ptr = point.get();

  EXPECT_EQ(point.x(), ptr.x);
  EXPECT_EQ(point.y(), ptr.y);
}

TEST(Point, Data)
{
  const cen::ipoint ip{123, 456};
  ASSERT_TRUE(ip.data());
  EXPECT_EQ(123, ip.data()->x);
  EXPECT_EQ(456, ip.data()->y);
}

TEST(Point, ToString)
{
  const cen::ipoint ip{123, 456};
  cen::log::put(cen::to_string(ip));

  const cen::fpoint fp{12.3f, 45.6f};
  cen::log::put(cen::to_string(fp));
}

TEST(Point, StreamOperator)
{
  const cen::ipoint ip{123, 456};
  std::cout << "COUT: " << ip << '\n';

  const cen::fpoint fp{12.3f, 45.6f};
  std::cout << "COUT: " << fp << '\n';
}

TEST(Point, Serialization)
{
  const auto x = 839.9f;
  const auto y = 931.5f;
  serialize_save("point.binary", cen::fpoint{x, y});

  const auto point = serialize_create<cen::fpoint>("point.binary");
  EXPECT_EQ(x, point.x());
  EXPECT_EQ(y, point.y());
}