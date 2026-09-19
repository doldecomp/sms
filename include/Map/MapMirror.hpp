#ifndef MAP_MAP_MIRROR_HPP
#define MAP_MAP_MIRROR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>

struct ResTIMG;
class MActor;
class MActorAnmData;
class J3DModel;

class TMirrorCamera : public JDrama::TCamera {
public:
	TMirrorCamera(const char* name = "鏡用カメラ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void makeMirrorViewMtx();
	void drawSetting(MtxPtr);
	void calcEffectMtx(MtxPtr);

	MtxPtr getUnk30() { return unk30; }
	f32 getUnk80() const { return unk80; }
	void setUnk84AndUnk90(f32 x, f32 y, f32 z, f32 dot)
	{
		unk84.set(x, y, z);
		unk90 = dot;
	}

	const ResTIMG* getUnk94() const { return unk94; }

public:
	/* 0x30 */ Mtx unk30;
	/* 0x60 */ GXTexObj unk60;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ JGeometry::TVec3<f32> unk84;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ ResTIMG* unk94;
	/* 0x98 */ JGeometry::TVec3<f32> unk98;
};

class TMirrorModel {
public:
	void initPlaneInfo();
	void entry();
	void calcView();
	void getMirrorTexInfo();

	virtual void init(const char*);
	virtual void calc() { }
	virtual void setPlane();
	virtual const JGeometry::TVec3<f32>& getNormalVec() const { return unk18; }
	virtual f32 getD() const { return unk24; }

	TMirrorModel();

	// fabricated
	MActor* getUnk4() { return unk4; }

public:
	/* 0x4 */ MActor* unk4;
	/* 0x8 */ TMirrorCamera* unk8;
	/* 0xC */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ f32 unk24;
};

class TMirrorModelObj : public TMirrorModel {
public:
	TMirrorModelObj() { unk28 = nullptr; }

	virtual void init(const char*);
	virtual void calc();
	virtual void setPlane();

public:
	/* 0x28 */ J3DModel* unk28;
};

class TMirrorModelManager;

extern TMirrorModelManager* gpMirrorModelManager;

class TMirrorModelManager : public JDrama::TViewObj {
public:
	TMirrorModelManager(const char* name = "鏡表示モデル管理");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	bool isUpperThanMirrorPlane(const JGeometry::TVec3<f32>&) const;
	bool isInMirror(JGeometry::TVec3<f32>&) const;
	void findMirrorCamera();
	void registerObjMirror(TMirrorModel*);

	// fabricated
	MActorAnmData* getUnk20() { return unk20; }
	// TMirrorActor::checkIsInMirror reads the mirror number through a
	// reference (`addi r5, r5, 0x18` then `lwz r4, 0(r4)`), so the accessor
	// returns `const int&`.
	const int& getUnk18() const { return unk18; }
	// One level only: writing this as `getUnk18() != -1` costs 8 bytes of
	// frame at every expansion and takes TMirrorActor::checkIsInMirror from
	// 0x68 to 0x70 (100 -> 99.88, instructions unchanged); the one-level form
	// alone gives 0x60. Retail's +8 there is the *parameter binding* of an
	// inlined free function taking the manager by pointer, not a second
	// accessor level -- which is why MirrorActor.cpp still parks
	// MirrorActor_isMirrorNoPresent() instead of using a member predicate.
	bool isUnk18Present() const { return unk18 != -1 ? true : false; }

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ TMirrorModel** unk1C;
	/* 0x20 */ MActorAnmData* unk20;
	/* 0x24 */ TMirrorCamera* unk24;
	/* 0x28 */ u16 unk28;
};

class TMirrorMapDrawBuf : public JDrama::TDrawBufObj {
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif
