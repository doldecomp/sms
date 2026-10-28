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
	bool isNowDemo() const;
	void startDemo(const char*, const JGeometry::TVec3<f32>*);
	int getTotalDemoFrames() const;
	bool isDemoFinished() const;
	void endDemo();
	void restartDemo();
	bool updateDemo(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*,
	                JGeometry::TVec3<f32>*, f32*);
	void setFrame(f32);
	void setRate(f32);

private:
	/* 0x0 */ MActor* mActor;
	/* 0x4 */ MActorAnmData* mAnmData;
	/* 0x8 */ u32 mPositionJointIndex;
	/* 0xC */ MtxPtr mPositionJointMtx;
	/* 0x10 */ MtxPtr mLookatJointMtx;
	/* 0x14 */ const JGeometry::TVec3<f32>* mTranslationOffset;
};

#endif
