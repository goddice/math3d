#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace goddice {
	using vec2 = glm::vec2;
	using point2 = glm::vec2;

	class Polygon {
	public:
		explicit Polygon(int n) {
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

		inline const size_t size() const {
			return points.size();
		}
	private:
		std::vector<point2> points;
	};

	class Line {
	public:
		Line(std::initializer_list<point2> pts): 
		s(*pts.begin()), e(*std::next(pts.begin())) {}

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
			return glm::length(s - e);
		}
	private:
		point2 s, e;
	};

	class Circle {
	public:
		Circle(const point2& c, const float& r) {
			center_ = c;
			radius_ = r;
		}

		point2& center() {
			return center_;
		}

		const point2 center() const {
			return center_;
		}

		float& radius() {
			return radius_;
		}

		const float radius() const {
			return radius_;
		}

	private:
		point2 center_;
		float radius_;
	};
}
