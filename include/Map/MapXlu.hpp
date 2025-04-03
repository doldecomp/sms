#ifndef MAP_MAP_XLU_HPP
#define MAP_MAP_XLU_HPP

class JSUMemoryInputStream;

class TMapXlu {
public:
	TMapXlu();
	void init(JSUMemoryInputStream&);
	bool changeXluJoint(int);
	void changeNormalJoint();
};

#endif
