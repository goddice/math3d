#pragma once
#include "utils.h"

namespace goddice {
	class tools {
	public:
		static float polygon_area(const Polygon& poly);
		static bool is_line_seg_intersect(const Line& l1, const Line& l2);
		static point2 line_seg_intersection(const Line& l1, const Line& l2);
		static float line_point_distance(const Line& l, const point2& p);
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
	};
}