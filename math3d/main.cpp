#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "utils.h"
#include "geomtry2d.h"
#include <random>

#include <blend2d.h>

TEST_CASE("The area of a polygon is calculated", "[PolygonArea]") {
	goddice::Polygon hex = {
		{6.0f, 2.0f},
		{5.0f, 4.0f},
		{2.0f, 3.0f},
		{4.0f, 3.0f},
		{2.0f, 1.0f},
		{6.0f, 2.0f}
	};
	REQUIRE(5.5f == goddice::tools::polygon_area(hex));
}

TEST_CASE("Two line segments not intersect", "[LineSegmentIntersection]") {
	goddice::Line l1 = { {1, 1}, {10, 1} };
	goddice::Line l2 = { {1, 2}, {10, 2} };
	REQUIRE_FALSE(goddice::tools::is_line_seg_intersect(l1, l2));
}

TEST_CASE("Two line segments intersect", "[LineSegmentIntersection]") {
	goddice::Line l1 = { {10, 0}, {0, 10} };
	goddice::Line l2 = { {0, 0}, {10, 10} };
	goddice::point2 result = goddice::tools::line_seg_intersection(l1, l2);
	goddice::point2 true_result = goddice::point2(5, 5);
	REQUIRE(result.x == true_result.x);
	REQUIRE(result.y == true_result.y);
}

TEST_CASE("Boudning Circle of Acute Triangle", "[BoundingCircle]") {

	std::vector<goddice::point2> pts = { {100, 100}, {270, 110}, {180, 315} };
	goddice::Circle circle = goddice::tools::bounding_circle_jon(pts);

	BLImage img(480, 480, BL_FORMAT_PRGB32);
	// Attach a rendering context into `img`.
	BLContext ctx(img);

	// Clear the image.
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();

	ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
	ctx.fillCircle(pts[0].x, pts[0].y, 5);
	ctx.fillCircle(pts[1].x, pts[1].y, 5);
	ctx.fillCircle(pts[2].x, pts[2].y, 5);

	ctx.setStrokeStyle(BLRgba32(0xF0F000FF));
	ctx.setStrokeWidth(5);
	ctx.strokeCircle(circle.center().x, circle.center().y, circle.radius());

	// Detach the rendering context from `img`.
	ctx.end();

	// Let's use some built-in codecs provided by Blend2D.
	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("bounding-circle-test1.bmp", codec);

	for (const auto& p : pts) {
		REQUIRE(goddice::tools::point_in_circle(p, circle));
	}
}

TEST_CASE("Boudning Circle of Point Set", "[BoundingCircle]") {

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(100, 350);
	std::vector<goddice::point2> pts;
	for (int i = 0; i < 12; ++i) {
		pts.emplace_back(goddice::point2(dist(rng), dist(rng)));
	}

	goddice::Circle circle = goddice::tools::bounding_circle_jon(pts);

	BLImage img(480, 480, BL_FORMAT_PRGB32);
	// Attach a rendering context into `img`.
	BLContext ctx(img);

	// Clear the image.
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();

	ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
	for (const auto& p : pts) {
		ctx.fillCircle(p.x, p.y, 5);
	}

	ctx.setStrokeStyle(BLRgba32(0xF0F000FF));
	ctx.setStrokeWidth(5);
	ctx.strokeCircle(circle.center().x, circle.center().y, circle.radius());

	// Detach the rendering context from `img`.
	ctx.end();

	// Let's use some built-in codecs provided by Blend2D.
	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("bounding-circle-test2.bmp", codec);

	for (const auto& p : pts) {
		REQUIRE(goddice::tools::point_in_circle(p, circle));
	}
}

TEST_CASE("Bounding Circle of intersection of two circles", "[BoundingCircle]") {
	goddice::Circle c1({ 150, 310 }, 80);
	goddice::Circle c2({ 230, 180 }, 120);
	goddice::Circle c = goddice::tools::bounding_circle_inter_circles(c1, c2);

	BLImage img(480, 480, BL_FORMAT_PRGB32);
	// Attach a rendering context into `img`.
	BLContext ctx(img);

	// Clear the image.
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();

	ctx.setStrokeStyle(BLRgba32(0xFFFFFFFF));
	ctx.setStrokeWidth(3);
	ctx.strokeCircle(c1.center().x, c1.center().y, c1.radius());

	ctx.setStrokeStyle(BLRgba32(0xFFFFFFFF));
	ctx.setStrokeWidth(3);
	ctx.strokeCircle(c2.center().x, c2.center().y, c2.radius());

	ctx.setStrokeStyle(BLRgba32(0xF0F000FF));
	ctx.setStrokeWidth(5);
	ctx.strokeCircle(c.center().x, c.center().y, c.radius());
	// Detach the rendering context from `img`.
	ctx.end();

	// Let's use some built-in codecs provided by Blend2D.
	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("bounding-circle-test3.bmp", codec);

	std::vector<goddice::point2> pts = goddice::tools::circle_intersection(c1, c2);
	for (const auto& p : pts) {
		REQUIRE(goddice::tools::point_in_circle(p, c));
	}
}

TEST_CASE("Intersection of two Circles", "[CircleIntersection]") {
	goddice::Circle c1({ 150, 310 }, 80);
	goddice::Circle c2({ 230, 180 }, 120);
	std::vector<goddice::point2> pts = goddice::tools::circle_intersection(c1, c2);
	for (const auto& p : pts) {
		REQUIRE(goddice::tools::point_in_circle(p, c1));
		REQUIRE(goddice::tools::point_in_circle(p, c2));
	}
}