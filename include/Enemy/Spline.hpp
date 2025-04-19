#ifndef ENEMY_SPLINE_HPP
#define ENEMY_SPLINE_HPP

class TSplinePath {
public:
	TSplinePath(int);
	~TSplinePath();
	void makeTable(float*, float*, float*);
	void calcTable();
	void calcSpline(float, float*, float*, float*);
	void getPoint(float);
};

#endif
