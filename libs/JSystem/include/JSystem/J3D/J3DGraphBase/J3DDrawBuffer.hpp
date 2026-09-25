#ifndef J3D_DRAW_BUFFER_HPP
#define J3D_DRAW_BUFFER_HPP

#include <dolphin/mtx.h>

class J3DPacket;
class J3DMatPacket;
class J3DCallBackPacket;

class J3DDrawBuffer {
public:
	typedef bool (J3DDrawBuffer::*sortFunc)(J3DMatPacket*);
	typedef void (J3DDrawBuffer::*drawFunc)() const;

	enum EDrawType {
		DRAW_HEAD,
		DRAW_TAIL,
	};

	enum ESortType {
		SORT_MAT,
		SORT_MAT_ANM,
		SORT_Z,
		SORT_MODEL,
		SORT_INVALID,
		SORT_NON,
	};

	J3DDrawBuffer(u32 size);
	~J3DDrawBuffer();
	void frameInit();
	bool entryMatSort(J3DMatPacket* packet);
	bool entryMatAnmSort(J3DMatPacket* packet);
	bool entryZSort(J3DMatPacket* packet);
	bool entryModelSort(J3DMatPacket* packet);
	bool entryInvalidSort(J3DMatPacket* packet);
	bool entryNonSort(J3DMatPacket* packet);
	bool entryImm(J3DPacket* packet, u16 index);
	void draw() const;
	void drawHead() const;
	void drawTail() const;
	void setCallBackPacket(J3DCallBackPacket* callback);

	// from TP
	int getSortMode() { return mSortType; }
	void setMatSort() { mSortType = SORT_MAT; }
	void setMatAnmSort() { mSortType = SORT_MAT_ANM; }
	void setZSort() { mSortType = SORT_Z; }
	void setNonSort() { mSortType = SORT_NON; }
	void setZMtx(MtxPtr mtx) { mZMtx = mtx; }

	static int entryNum;
	static sortFunc sortFuncTable[6];

private:
	static drawFunc drawFuncTable[2];

	/* 0x00 */ J3DPacket** mBuffer;
	/* 0x04 */ u32 mSize;
	/* 0x08 */ EDrawType mDrawType;
	/* 0x0C */ ESortType mSortType;
	/* 0x10 */ f32 mZNear;
	/* 0x14 */ f32 mZFar;
	/* 0x18 */ f32 mZRatio;
	/* 0x1C */ MtxPtr mZMtx;
	/* 0x20 */ J3DPacket* mCallBackPacket;
};

#endif
