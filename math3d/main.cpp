#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "geometry_utils.h"
#include "drawing_utils.h"
#include "geomtry2d.h"
#include <random>

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

TEST_CASE("Bounding Circle of Acute Triangle", "[BoundingCircle]") {

	std::vector<goddice::point2> pts = { {100, 100}, {270, 110}, {180, 315} };
	goddice::Circle circle = goddice::tools::bounding_circle_jon(pts);

	goddice::Canvas canvas(480, 480);
	for (const auto& p : pts) {
		canvas.addPoint(p, 5, BLRgba32(0xFFFFFFFF));
	}
	canvas.addCircle(circle, 3, BLRgba32(0xF0F000FF));
	canvas.saveImage("imgs/bounding-circle-test1.bmp");

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

	goddice::Canvas canvas(480, 480);
	for (const auto& p : pts) {
		canvas.addPoint(p, 5, BLRgba32(0xFFFFFFFF));
	}
	canvas.addCircle(circle, 3, BLRgba32(0xF0F000FF));
	canvas.saveImage("imgs/bounding-circle-test2.bmp");

	for (const auto& p : pts) {
		REQUIRE(goddice::tools::point_in_circle(p, circle));
	}
}

TEST_CASE("Bounding Circle of intersection of two circles", "[BoundingCircle]") {
	goddice::Circle c1({ 150, 310 }, 80);
	goddice::Circle c2({ 230, 180 }, 120);
	goddice::Circle c = goddice::tools::bounding_circle_inter_circles(c1, c2);

	goddice::Canvas canvas(480, 480);
	canvas.addCircle(c1, 3, BLRgba32(0xFFFFFFFF));
	canvas.addCircle(c2, 3, BLRgba32(0xFFFFFFFF));
	canvas.addCircle(c, 5, BLRgba32(0xF0F000FF));
	canvas.saveImage("imgs/bounding-circle-test3.bmp");

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

TEST_CASE("Appolonius's 10th problem", "[Appolonius]") {
	goddice::Circle c1({ 80, 290 }, 30);
	goddice::Circle c2({ 230, 110 }, 40);
	goddice::Circle c3({ 310, 180 }, 60);
	goddice::Circle c = goddice::tools::appolonius10th(c1, c2, c3);

	goddice::Canvas canvas(480, 480);
	canvas.addCircle(c1, 5, BLRgba32(0xFFFFFFFF));
	canvas.addCircle(c2, 5, BLRgba32(0xFFFFFFFF));
	canvas.addCircle(c3, 5, BLRgba32(0xFFFFFFFF));
	canvas.addCircle(c, 2, BLRgba32(0xF0F000FF));
	canvas.saveImage("imgs/appolonius-test.bmp");

	REQUIRE(goddice::tools::circle_touch(c, c1));
	REQUIRE(goddice::tools::circle_touch(c, c2));
	REQUIRE(goddice::tools::circle_touch(c, c3));
}