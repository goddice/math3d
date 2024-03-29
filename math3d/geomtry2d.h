#pragma once
#include "geometry_utils.h"

namespace goddice {
	class tools {
	public:
		// 1.1
		static float polygon_area(const Polygon& poly);
		// 1.2
		static bool is_line_seg_intersect(const Line& l1, const Line& l2);
		static point2 line_seg_intersection(const Line& l1, const Line& l2);
		// 1.3
		static float line_point_distance(const Line& l, const point2& p);
		// 1.4
		static Circle bounding_circle_jon(const std::vector<point2>& pts);
		// 1.5
		static Circle bounding_circle_inter_circles(const Circle& P, const Circle& Q);
		// 1.6
		static Circle appolonius10th(const Circle& c1, const Circle& c2, const Circle& c3);

		// helper functions
		static std::vector<point2> circle_intersection(const Circle& C1, const Circle& C2);
		static bool point_in_circle(const point2& p, const Circle& c);
		static bool circle_touch(const Circle& c1, const Circle& c2);
	private:
		// Given three colinear points p, q, r, the function checks if 
		// point q lies on line segment 'pr' 
		static bool on_seg(const point2& p, const point2& q, const point2& r);

		// To find orientation of ordered triplet (p, q, r). 
		// The function returns following values 
		// 0 --> p, q and r are colinear 
		// 1 --> Clockwise 
		// 2 --> Counterclockwise
		static int orientation(const point2& p, const point2& q, const point2& r);

		// check if the angle pqr is acute (include 90 degree)
		static bool is_acute(const point2& p, const point2& q, const point2& r);

		// find the circumcircle of a triangle
		static Circle circumcircle(const point2& p, const point2& q, const point2& r);

		// find the bounding circle of a triangle
		static Circle bounding_circle_tri(const point2& p, const point2& q, const point2& r);

		// find the bounding circle of two points
		static Circle bounding_circle_two_points(const point2& p, const point2& q);
	};
}