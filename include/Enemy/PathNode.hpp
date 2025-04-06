#ifndef ENEMY_PATH_NODE_HPP
#define ENEMY_PATH_NODE_HPP

#include <dolphin/mtx.h>

class TPathNode {
public:
	~TPathNode();
	TPathNode();
	void getPoint() const;

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ Vec unk4;
};

#endif
