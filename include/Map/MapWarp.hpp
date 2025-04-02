#ifndef MAP_MAP_WARP_HPP
#define MAP_MAP_WARP_HPP

class JSUMemoryInputStream;

class TMapWarp {
public:
	TMapWarp();

	void init(JSUMemoryInputStream&);
	void initModel();
	void watchToWarp();
	void warp(int);
	void changeModel(int);
};

#endif
