#ifndef JUT_POINT_HPP
#define JUT_POINT_HPP

class JUTPoint {
public:
	JUTPoint() { set(0, 0); }
	JUTPoint(int x, int y) { set(x, y); }

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x; // _00
	int y; // _04
};

#endif
