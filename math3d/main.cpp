#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "utils.h"
#include "geomtry2d.h"

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