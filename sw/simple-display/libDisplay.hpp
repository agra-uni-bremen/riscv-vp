/*
 * libDisplay.hpp
 *
 *  Created on: Oct 5, 2018
 *      Author: dwd
 */

#pragma once

#include "../../env/basic/vp-display/framebuffer.h"

using namespace frame;	// bug-fix RD (on CentOS7, this only works with this line)

extern Framebuffer* volatile const framebuffer;

inline Color fromRGB(uint8_t r, uint8_t g, uint8_t b) {
	Color ret = 0;
	ret |= (b & 0x0F);
	ret |= (g & 0x0F) << 4;
	ret |= (r & 0x0F) << 8;
	return ret;
}

namespace display {
void setPixel(Framebuffer::Type frame, Point pixel, Color color);

void drawLine(Framebuffer::Type frame, PointF from, PointF to, Color color);

void drawRect(Framebuffer::Type frame, PointF ol, PointF ur, Color color);

void fillRect(Framebuffer::Type frame, PointF ol, PointF ur, Color color);

void applyFrame();

void fillFrame(Framebuffer::Type frame = Framebuffer::Type::foreground, Color color = 0);
};  // namespace display
