#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace goddice {
	using vec2 = glm::vec2;
	using point2 = glm::vec2;

	class Polygon {
	public:
		Polygon(int n) {
			points.resize(n);
		}
		Polygon(std::initializer_list<point2> pts) {
			points.insert(points.begin(), pts.begin(), pts.end());
		}
		const point2 operator[](int i) const {
			return points[i];
		}
		point2& operator[](int i) {
			return points[i];
		}
		inline const int size() const {
			return points.size();
		}
	private:
		std::vector<point2> points;
	};

	class Line {
	public:
		Line(std::initializer_list<point2> pts) {
			s = *pts.begin();
			e = *std::next(pts.begin());
		}
		const point2 operator[](int i) const {
			if (i == 0)
				return s;
			else
				return e;
		}
		point2& operator[](int i) {
			if (i == 0)
				return s;
			else
				return e;
		}
		inline const float lengh() const {
			return (s - e).length();
		}
	private:
		point2 s, e;
	};
}
