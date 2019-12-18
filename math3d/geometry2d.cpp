#include "geomtry2d.h"
#include <algorithm>

namespace goddice {
	float tools::polygon_area(const Polygon& poly) {
		float area = 0;
		for (int i = 0; i < poly.size() - 1; ++i) {
			area += (poly[i].x * poly[i + 1].y - poly[i + 1].x * poly[i].y);
		}
		area *= 0.5;
		return area;
	}

	bool tools::is_line_seg_intersect(const Line& l1, const Line& l2) {
		// Find the four orientations needed for general and 
		// special cases 
		int o1 = orientation(l1[0], l1[1], l2[0]);
		int o2 = orientation(l1[0], l1[1], l2[1]);
		int o3 = orientation(l2[0], l2[1], l1[0]);
		int o4 = orientation(l2[0], l2[1], l1[1]);

		// General case 
		if (o1 != o2 && o3 != o4) return true;

		// Special Cases 
		// l1[0], l1[1] and l2[0] are colinear and l2[0] lies on segment l1[0]l1[1] 
		if (o1 == 0 && on_seg(l1[0], l2[0], l1[1])) return true;
		// l1[0], l1[1] and l2[1] are colinear and l2[1] lies on segment l1[0]l1[1] 
		if (o2 == 0 && on_seg(l1[0], l2[1], l1[1])) return true;
		// l2[0], l2[1] and l1[0] are colinear and l1[0] lies on segment l2[0]l2[1] 
		if (o3 == 0 && on_seg(l2[0], l1[0], l2[1])) return true;
		// l2[0], l2[1] and l1[1] are colinear and l1[1] lies on segment l2[0]l2[1] 
		if (o4 == 0 && on_seg(l2[0], l1[1], l2[1])) return true;

		return false; // Doesn't fall in any of the above cases 
	}

	point2 tools::line_seg_intersection(const Line& l1, const Line& l2) {
		if (!is_line_seg_intersect(l1, l2)) {
			return point2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		}
		else {
			float a1 = l1[1].y - l1[0].y;
			float b1 = l1[0].x - l1[1].x;
			float c1 = a1 * l1[0].x + b1 * l1[0].y;
			float a2 = l2[1].y - l2[0].y;
			float b2 = l2[0].x - l2[1].x;
			float c2 = a2 * l2[0].x + b2 * l2[0].y;
			float inv_det = 1.0f / (a1 * b2 - a2 * b1);
			return point2(b2 * c1 - b1 * c2, a1 * c2 - a2 * c1) * inv_det;
		}
	}

	float tools::line_point_distance(const Line& l, const point2& p) {
		float a2 = (p.y - l[0].y) * (l[1].x - l[0].x) - (p.x - l[0].x) * (l[1].y - l[0].y);
		return fabs(a2) / l.lengh();
	}

	bool tools::on_seg(const point2& p, const point2& q, const point2& r) {
		if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
			q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
			return true;
		return false;
	}
	int tools::orientation(const point2& p, const point2& q, const point2& r) {
		int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
		if (val == 0) return 0;  // colinear 
		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}
}