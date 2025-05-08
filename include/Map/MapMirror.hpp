#ifndef MAP_MAP_MIRROR_HPP
#define MAP_MAP_MIRROR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <JSystem/JDrama/JDRDrawBufObj.hpp>

class TMirrorCamera : public JDrama::TCamera {
public:
	virtual void perform(u32, JDrama::TGraphics*);

	void makeMirrorViewMtx();
	void drawSetting(MtxPtr);
	void calcEffectMtx(MtxPtr);
	TMirrorCamera(const char*);

public:
	/* 0x30 */ Mtx unk30;
	/* 0x60 */ GXTexObj unk60;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ f32 unk88;
	/* 0x8C */ f32 unk8C;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ void* unk94;
	/* 0x98 */ JGeometry::TVec3<f32> unk98;
};

class TMirrorModel {
	void initPlaneInfo();
	void entry();
	void calcView();
	void getMirrorTexInfo();

	virtual void init(const char*);
	virtual void calc() { }
	virtual void setPlane();
	virtual void getNormalVec() const { }
	virtual void getD() const { }

	TMirrorModel();
};

class TMirrorModelObj : public TMirrorModel {
	virtual void init(const char*);
	virtual void calc();
	virtual void setPlane();
};

class TMirrorModelManager;

extern TMirrorModelManager* gpMirrorModelManager;

class TMirrorModelManager : public JDrama::TViewObj {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void isUpperThanMirrorPlane(const JGeometry::TVec3<f32>&) const;
	bool isInMirror(JGeometry::TVec3<f32>&) const;
	void findMirrorCamera();
	void registerObjMirror(TMirrorModel*);
	TMirrorModelManager(const char*);
};

class TMirrorMapDrawBuf : public JDrama::TDrawBufObj {
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif
