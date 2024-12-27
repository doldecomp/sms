#ifndef J3D_ANIMATION_HPP
#define J3D_ANIMATION_HPP

#include <types.h>

class J3DFrameCtrl {
public:
	J3DFrameCtrl() { init(0); };
	virtual ~J3DFrameCtrl() {};

	typedef void IDK;

	void setSpeed(f32 speed) { mSpeed = speed; }
	void setEndFrame(s16 end_frame) { mEndFrame = end_frame; }
	f32 getCurrentFrame() const { return mCurrentFrame; }

	void init(s16);
	bool checkPass(float);
	void update();

	// TODO: probably private and always accessed via getters/setters judging by
	// stack frame padding
	// TODO: Stolen from SMG, might be wrong
	u8 mLoopMode;      // 0x4
	u8 mFlags;         // 0x5
	s16 mStartFrame;   // 0x6
	s16 mEndFrame;     // 0x8
	s16 mLoopFrame;    // 0xA
	f32 mSpeed;        // 0xC
	f32 mCurrentFrame; // 0x10
};

class J3DAnmBase {
public:
	J3DAnmBase();
	J3DAnmBase(s16);

	u16 getMaxFrame() { return mMaxFrame; }

	char unk0[2];
	u16 mMaxFrame;
	float unk4;
	char padding[4];
	// NOTE: vtable is at 0xc
	virtual ~J3DAnmBase();

	typedef void IDK;
	IDK isLegal(char*) const;
};

#endif
