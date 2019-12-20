#include "drawing_utils.h"

namespace goddice {
	Canvas::Canvas(int w, int h) {
		img = BLImage(w, h, BL_FORMAT_PRGB32);
		ctx = BLContext(img);

		// Clear the image.
		ctx.setCompOp(BL_COMP_OP_SRC_COPY);
		ctx.fillAll();
	}

	void Canvas::addPoint(const point2& p, double w, const BLRgba32& color) {
		ctx.setFillStyle(color);
		ctx.fillCircle(p.x, p.y, w);
	}

	void Canvas::addCircle(const Circle& c, double w, const BLRgba32& color) {
		ctx.setStrokeStyle(color);
		ctx.setStrokeWidth(w);
		ctx.strokeCircle(c.center().x, c.center().y, c.radius());
	}

	void Canvas::saveImage(const std::string& path) {
		BLImageCodec codec;
		codec.findByName("BMP");
		img.writeToFile(path.c_str(), codec);
	}

	Canvas::~Canvas() {
		ctx.end();
	}
}