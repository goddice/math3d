#pragma once
#include "geometry_utils.h"

#include <blend2d.h>

namespace goddice {
	class Canvas {
	public:
		Canvas(int w, int h);
		~Canvas();

		void addPoint(const point2& p, double w, const BLRgba32& color);
		void addCircle(const Circle& c, double w, const BLRgba32& color);
		void saveImage(const std::string& path);

	private:
		BLImage img;
		BLContext ctx;
	};
}