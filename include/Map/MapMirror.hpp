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
