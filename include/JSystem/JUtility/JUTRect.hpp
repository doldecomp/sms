#ifndef JUT_RECT_HPP
#define JUT_RECT_HPP

class JUTRect {
public:
	void set(int, int, int, int);
	void copy(const JUTRect&);
	void add(int, int);
	bool intersect(const JUTRect&);
	void move(int, int);
	void resize(int, int);
	void reform(int, int, int, int);
	void normalize();
	bool isEmpty() const;

	int x1; // _00
	int y1; // _04
	int x2; // _08
	int y2; // _0C
};

#endif // JUTRECT_HPP
