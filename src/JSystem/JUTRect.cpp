#include "JSystem/JUTRect.hpp"

void JUTRect::set(int _x1, int _y1, int _x2, int _y2)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}

void JUTRect::copy(const JUTRect& rRect)
{
	x1 = rRect.x1;
	y1 = rRect.y1;
	x2 = rRect.x2;
	y2 = rRect.y2;
}

void JUTRect::add(int a1, int a2)
{
	x1 += a1;
	x2 += a1;
	y1 += a2;
	y2 += a2;
}

bool JUTRect::intersect(const JUTRect& rRect)
{
	if (x1 < rRect.x1)
		x1 = rRect.x1;

	if (y1 < rRect.y1)
		y1 = rRect.y1;

	if (x2 > rRect.x2)
		x2 = rRect.x2;

	if (y2 > rRect.y2)
		y2 = rRect.y2;

	bool ret = !isEmpty();
	return ret;
}

void JUTRect::move(int a1, int a2)
{
	int width  = x2 - x1;
	int height = y2 - y1;

	x1 = a1;
	y1 = a2;

	x2 = x1 + width;
	y2 = y1 + height;
}

void JUTRect::resize(int a1, int a2)
{
	x2 = x1 + a1;
	y2 = y1 + a2;
}

void JUTRect::reform(int _x1, int _y1, int _x2, int _y2)
{
	x1 += _x1;
	y1 += _y1;
	x2 += _x2;
	y2 += _y2;
}

void JUTRect::normalize()
{
	if (x1 > x2) {
		int _x2 = x2;
		x2      = x1;
		x1      = _x2;
	}

	int _y1 = y1;
	int _y2 = y2;

	if (_y1 > _y2) {
		y2 = _y1;
		y1 = _y2;
	}
}

bool JUTRect::isEmpty() const
{
	bool ret = true;

	if (x1 < x2 && y1 < y2)
		ret = false;

	return ret;
}
