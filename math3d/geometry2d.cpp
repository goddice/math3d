#include "geomtry2d.h"
#include <algorithm>
#include <complex>

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
		const int o1 = orientation(l1[0], l1[1], l2[0]);
		const int o2 = orientation(l1[0], l1[1], l2[1]);
		const int o3 = orientation(l2[0], l2[1], l1[0]);
		const int o4 = orientation(l2[0], l2[1], l1[1]);

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
		float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
		if (val == 0) return 0;  // colinear 
		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	bool tools::is_acute(const point2& p, const point2& q, const point2& r) {
		vec2 u = p - q;
		vec2 v = r - q;
		return glm::dot(u, v) >= 0;
	}

	bool tools::point_in_circle(const point2& p, const Circle& c) {
		return glm::length(p - c.center()) <= c.radius() + 1e-3;
	}

	// reference: https://en.wikipedia.org/wiki/Circumscribed_circle
	Circle tools::circumcircle(const point2& A, const point2& B, const point2& C) {
		point2 Bp = B - A;
		point2 Cp = C - A;
		float Dp = 2 * (Bp.x * Cp.y - Bp.y * Cp.x);
		float S = Bp.x * Bp.x + Bp.y * Bp.y;
		float T = Cp.x * Cp.x + Cp.y * Cp.y;
		float Upx = (Cp.y * S - Bp.y * T) / Dp;
		float Upy = (Bp.x * T - Cp.x * S) / Dp;
		vec2 Up(Upx, Upy);
		float r = glm::length(Up);
		point2 U = A + Up;
		return Circle(U, r);
	}

	Circle tools::bounding_circle_two_points(const point2& p, const point2& q) {
		point2 c = (p + q) * 0.5f;
		return Circle(c, 0.5f * glm::length(p - q));
	}

	Circle tools::bounding_circle_tri(const point2& p, const point2& q, const point2& r) {
		if (is_acute(p, q, r) && is_acute(q, r, p) && is_acute(r, p, q)) {
			return circumcircle(p, q, r);
		}
		else if (!is_acute(p, q, r)) {
			return bounding_circle_two_points(p, r);
		}
		else if (!is_acute(q, r, p)) {
			return bounding_circle_two_points(q, p);
		}
		else {
			return bounding_circle_two_points(r, q);
		}
	}

	Circle tools::bounding_circle_jon(const std::vector<point2>& pts) {
		if (pts.size() < 2) {
			return Circle({ 0.0, 0.0 }, 0.0);
		}
		else if (pts.size() == 2) {
			return bounding_circle_two_points(pts[0], pts[1]);
		}
		else if (pts.size() == 3) {
			return bounding_circle_tri(pts[0], pts[1], pts[2]);
		}
		else {
			// Jon Rokne algorithm

			point2 P, Q, R;

			// find P with smallest Py
			P = *std::min_element(pts.begin(), pts.end(), [](const auto& p1, const auto& p2) {return p1.y < p2.y; });

			// find Q that PQ is closest to x axis
			vec2 xaxis(1, 0);
			float best_score = 0;
			for (const auto& p : pts) {
				vec2 v = p - P;
				float vl = glm::length(v);
				if (vl > FLT_EPSILON) {
					float score = abs(glm::dot(v, xaxis)) / vl;
					if (score > best_score) {
						Q = p;
						best_score = score;
					}
				}
			}

			for (int i = 0; i < pts.size(); ++i) {

				// find R that PRQ is minimal
				// using the fact that cos(x) is monotonically decreasing in [0, pi]
				best_score = -1;
				for (const auto& p : pts) {
					vec2 u = P - p;
					vec2 v = Q - p;
					float ul = glm::length(u);
					float vl = glm::length(v);
					if (ul > FLT_EPSILON&& vl > FLT_EPSILON) {
						float score = glm::dot(v, u) / ul / vl;
						if (score > best_score) {
							R = p;
							best_score = score;
						}
					}
				}

				// if RQP and QPR is acute
				if (is_acute(R, Q, P) && is_acute(Q, P, R)) {
					if (is_acute(P, R, Q)) {
						// acute triangle
						return circumcircle(P, Q, R);
					}
					else {
						// obtuse triangle
						return bounding_circle_two_points(P, Q);
					}
				}
				else if (!is_acute(R, Q, P)) {
					Q = R;
				}
				else {
					P = R;
				}
			}

			return bounding_circle_tri(P, Q, R);
		}
	}
	
	Circle tools::bounding_circle_inter_circles(const Circle& P, const Circle& Q) {
		float l = glm::length(P.center() - Q.center());
		if (l > P.radius() + Q.radius()) {
			return Circle({ 0, 0 }, 0);
		}
		else if (l + P.radius() < Q.radius()) {
			return P;
		}
		else if (l + Q.radius() < P.radius()) {
			return Q;
		}
		else {
			float z = (l * l + P.radius() * P.radius() - Q.radius() * Q.radius()) / l * 0.5f;
			float t = sqrt(P.radius() * P.radius() - z * z);
			point2 R = P.center() - (P.center() - Q.center()) * z / l;
			return Circle(R, t);
		}
	}

	std::vector<point2> tools::circle_intersection(const Circle& C1, const Circle& C2) {
		float R = glm::length(C1.center() - C2.center());
		if (R > C1.radius() + C2.radius() || C1.radius() > R + C2.radius() || C2.radius() > R + C1.radius()) {
			return {};
		}
		else if (R == C1.radius() + C2.radius()) {
			return { C1.center() + (C2.center() - C1.center()) * C1.radius() / R };
		}
		else {
			float x1 = C1.center().x;
			float y1 = C1.center().y;
			float x2 = C2.center().x;
			float y2 = C2.center().y;
			float r1 = C1.radius();
			float r2 = C2.radius();
			float k = (r1 * r1 - r2 * r2) / R / R;
			float l = (r1 * r1 + r2 * r2) / R / R;
			float t = 0.5f * sqrtf(2 * l - k * k - 1);
			point2 v = point2(0.5 * (x1 + x2), 0.5 * (y1 + y2));
			v = v + point2(x2 - x1, y2 - y1) * k * 0.5f;
			point2 v1 = v + point2(y2 - y1, x1 - x2) * t;
			point2 v2 = v - point2(y2 - y1, x1 - x2) * t;
			return { v1,v2 };
		}
	}

	Circle tools::appolonius10th(const Circle& c1, const Circle& c2, const Circle& c3) {
		std::vector<Circle> circles = { c1, c2, c3 };
		std::sort(circles.begin(), circles.end(), [](const auto& x, const auto& y) {return x.radius() < y.radius(); });
		std::complex<float> z1(circles[1].center().x, circles[1].center().y);
		std::complex<float> z2(circles[2].center().x, circles[2].center().y);
		std::complex<float> z3(circles[0].center().x, circles[0].center().y);
		
		float r1 = circles[1].radius() - circles[0].radius();
		float r2 = circles[2].radius() - circles[0].radius();
		float k1 = (std::norm(z1 - z3) - r1 * r1);
		float k2 = (std::norm(z2 - z3) - r2 * r2);
		auto W1 = std::conj(z1 - z3) / k1;
		auto rho1 = r1 / k1;
		auto W2 = std::conj(z2 - z3) / k2;
		auto rho2 = r2 / k2;
		auto k3 = sqrt(std::norm(W1 - W2));
		auto cosalpha = (rho1 - rho2) / k3;
		auto sinalpha1 = sqrt(1 - cosalpha * cosalpha);
		auto sinalpha2 = -sinalpha1;
		auto U1 = std::complex<float>(cosalpha, sinalpha1) * std::conj(W1 - W2) / k3;
		auto U2 = std::complex<float>(cosalpha, -sinalpha1) * std::conj(W1 - W2) / k3;
		auto cc1 = (W1 * U1).real() - rho1;
		auto cc2 = (W1 * U2).real() - rho1;
		
		auto cc = cc1;
		auto U = U1;
		if (cc2 > cc1) {
			cc = cc2;
			U = U2;
		}

		if (cc <= 0) {
			return Circle({ 0,0 }, 0); 
		}
		auto z = U / 2.f / cc + z3;
		return Circle({ z.real(), z.imag() }, 0.5f / cc + circles[0].radius());
	}

	bool tools::circle_touch(const Circle& c1, const Circle& c2) {
		float r1 = c1.radius();
		float r2 = c2.radius();
		float l = glm::length(c1.center() - c2.center());
		return fabs(r1 + r2 - l) < FLT_EPSILON || fabs(r1 + l - r2) < FLT_EPSILON || fabs(r2 + l - r1) < FLT_EPSILON;
	}
}