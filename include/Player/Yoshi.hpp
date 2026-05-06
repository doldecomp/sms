#ifndef YOSHI_HPP
#define YOSHI_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <MSound/MAnmSound.hpp>
#include <M3DUtil/MActor.hpp>

class TMario;
class TEggYoshi;
class TYoshiTongue;
class J3DModel;
class MAnmSound;
class JAIAnimeSound;
class TRidingInfo;

class TYoshi {
public:
	enum Color { GREEN, ORANGE, PURPLE, PINK };
	enum State {
		STATE_EGG       = 0,
		STATE_UNK1      = 1,
		STATE_UNK2      = 2,
		STATE_DROWNING  = 3,
		STATE_DYING     = 4,
		STATE_UNK5      = 5,
		STATE_UNMOUNTED = 6,
		STATE_UNK7      = 7,
		STATE_MOUNTED   = 8
	};

	bool appearFromEgg(const JGeometry::TVec3<f32>&, f32, TEggYoshi*);
	void calcAnim();
	void changeAnimation(int id);
	void appear();
	bool disappear();
	void doEat(u32 fruitID);
	void doSearch();
	void entry();
	void getEmitPosDir(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*) const;
	J3DFrameCtrl* getFrameCtrl() const;
	MtxPtr getMtxPtrFootL() const;
	MtxPtr getMtxPtrFootR() const;
	void getOff(bool knockedOff);
	void init(TMario*);
	void initInLoadAfter();
	void kill();
	void movement();
	bool onYoshi();
	void ride();
	void setEggYoshiPtr(TEggYoshi*);
	u16 changeHand();
	void thinkAnimation();
	void thinkBtp(int);
	void thinkHoldOut();
	void thinkEat();
	void thinkUpper();
	void thinkJumpEnd(u16, u16*);
	void viewCalc();
	void emitTongue();
	void startVoice(u32);

	// fabricated
	MtxPtr getTongueMtx() const
	{
		return mActor->getModel()->getAnmMtx(mJointIdxTongue);
	}

	BOOL isHatched() const { return mState == STATE_EGG ? FALSE : TRUE; }

public:
	/* 0x0 */ u8 mState;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ s16 unk2;
	/* 0x4 */ s16 unk4;
	/* 0x6 */ s16 unk6;
	/* 0x8 */ s32 unk8;
	/* 0xC */ s32 unkC;
	/* 0x10 */ TMario* mMario;
	/* 0x14 */ JGeometry::TVec3<f32> mLastTranslation;
	/* 0x20 */ JGeometry::TVec3<f32> mTranslation;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ MActorAnmData* unk30;
	/* 0x34 */ MActor* mActor;
	/* 0x38 */ TYoshiTongue* mTongue;
	/* 0x3C */ u16 mJointIdxTongue;
	/* 0x3E */ u16 mJointIdxFootL;
	/* 0x40 */ u16 mJointIdxFootR;
	/* 0x42 */ u16 mJointIdxCenter;
	/* 0x44 */ J3DModel* mMirrorModels[2];
	/* 0x4C */ J3DAnmBase* unk4C;
	/* 0x50 */ J3DAnmBase* unk50;
	/* 0x54 */ J3DMtxCalc* unk54;
	/* 0x58 */ J3DMtxCalc* unk58;
	/* 0x5C */ J3DFrameCtrl unk5C;
	/* 0x70 */ s16 mEggRotSpeed;
	/* 0x72 */ u16 unk72;
	/* 0x74 */ JGeometry::TVec3<f32> unk74;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ JGeometry::TVec3<f32> unk84;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ TRidingInfo* unk94;
	/* 0x98 */ f32 unk98;
	/* 0x9C */ f32 unk9C;
	/* 0xA0 */ f32 unkA0;
	/* 0xA4 */ f32 unkA4;
	/* 0xA8 */ J3DDrawBuffer* unkA8;
	/* 0xAC */ J3DDrawBuffer* unkAC;
	/* 0xB0 */ u32 unkB0;
	/* 0xB4 */ u32 unkB4;
	/* 0xB8 */ u8 mFlutterState;
	/* 0xB9 */ u8 unkB9;
	/* 0xBA */ u16 mFlutterTimer;
	/* 0xBC */ u16 mMaxFlutterTimer;
	/* 0xBE */ u16 unkBE;
	/* 0xC0 */ f32 mMaxVSpdStartFlutter;
	/* 0xC4 */ f32 mFlutterAcceleration;
	/* 0xC8 */ char unkC8[0x8];
	/* 0xD0 */ u8 mType;
	/* 0xD1 */ u8 unkD1;
	/* 0xD2 */ u16 unkD2;
	/* 0xD4 */ u32 unkD4;
	/* 0xD8 */ u32 unkD8;
	/* 0xDC */ u8 unkDC;
	/* 0xDE */ s16 unkDE;
	/* 0xE0 */ s16 unkE0;
	/* 0xE4 */ f32 unkE4;
	/* 0xE8 */ s16 unkE8;
	/* 0xEA */ s16 unkEA;
	/* 0xEC */ f32 unkEC;
	/* 0xF0 */ TEggYoshi* mEgg;
	/* 0xF4 */ u16 mBtpIndex;
	/* 0xF6 */ u16 unkF6;
	/* 0xF8 */ u16 unkF8;
	/* 0xFC */ JGeometry::TVec3<f32> unkFC;
	/* 0x108 */ JGeometry::TVec3<f32> unk108;
	/* 0x114 */ f32 unk114;
	/* 0x118 */ MAnmSound* mBodyAnmSound;
	/* 0x11C */ void** mBodyAnmSoundTable;
	/* 0x120 */ MAnmSound* mTongueAnmSound;
};

#endif
