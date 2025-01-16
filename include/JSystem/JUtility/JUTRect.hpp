#ifndef JUT_RECT_HPP
#define JUT_RECT_HPP

class JUTRect {
public:
	JUTRect() { set(0, 0, 0, 0); }
	JUTRect(int x1, int y1, int x2, int y2) { set(x1, y1, x2, y2); }

	JUTRect(const JUTRect& other) { copy(other); }

	void set(int, int, int, int);
	void copy(const JUTRect&);
	void add(int, int);
	bool intersect(const JUTRect&);
	void move(int, int);
	void resize(int, int);
	void reform(int, int, int, int);
	void normalize();
	bool isEmpty() const;
	int getWidth() const { return x2 - x1; }
	int getHeight() const { return y2 - y1; }

	int x1; // _00
	int y1; // _04
	int x2; // _08
	int y2; // _0C
};

#endif // JUTRECT_HPP
