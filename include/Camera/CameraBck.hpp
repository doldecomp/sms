#ifndef CAMERA_CAMERA_BCK_HPP
#define CAMERA_CAMERA_BCK_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class MActor;
class MActorAnmData;

class TCameraBck {
public:
	TCameraBck();
	bool isFileExist(const char*) const;
	void getDemoFileName() const;
	void isNowDemo() const;
	void startDemo(const char*, const JGeometry::TVec3<f32>*);
	int getTotalDemoFrames() const;
	void isDemoFinished() const;
	void endDemo();
	void restartDemo();
	bool updateDemo(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*,
	                JGeometry::TVec3<f32>*, f32*);
	void setFrame(f32);

public:
	/* 0x0 */ MActor* unk0;
	/* 0x4 */ MActorAnmData* unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ MtxPtr unkC;
	/* 0x10 */ MtxPtr unk10;
	/* 0x14 */ const JGeometry::TVec3<f32>* unk14;
};

#endif
