#ifndef MOVE_BG_MOVE_OBJ_MANAGER_HPP
#define MOVE_BG_MOVE_OBJ_MANAGER_HPP

#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <Strategic/LiveManager.hpp>

class TMapObjBase;
class J3DMaterialTable;
class J3DModel;
class SDLModelData;
class MActor;
struct ResTIMG;
namespace JDrama {
class TDrawBufObj;
}

class TMapObjBaseManager : public TLiveManager {
public:
	TMapObjBaseManager(const char* name = "地形基底オブジェ管理");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void clipActors(JDrama::TGraphics*);

	int getObjNumWithActorType(u32) const;
	static u32 getActorTypeByEventID(u32);
	static TMapObjBase* newAndRegisterObjByEventID(u32, const char*);
	static TMapObjBase*
	newAndRegisterObj(const char* name,
	                  const JGeometry::TVec3<f32>& position
	                  = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	                  const JGeometry::TVec3<f32>& rotation
	                  = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	                  const JGeometry::TVec3<f32>& scale
	                  = JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	TMapObjBase* makeObjAppeared(u32);
	TMapObjBase* makeObjAppear(u32);
	TMapObjBase* makeObjAppear(f32, f32, f32, u32, bool);
	bool canAppear(const TMapObjBase*, u32) const;

protected:
	/* 0x38 */ f32 mClipFar;
	/* 0x3C */ f32 mActorRadius;
};

class TMapObjManager;

extern TMapObjManager* gpMapObjManager;

class TMapObjManager : public TMapObjBaseManager {
public:
	void initKeyCode();

	TMapObjManager(const char* name = "地形オブジェ管理");
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL hasMapCollision() const { return true; }

	static J3DMaterialTable* loadMatTable(const char*);
	void initDrawBuffer();
	void entryStaticDrawBufferSun(J3DModel*);
	void entryStaticDrawBufferShadow(J3DModel*);

	// fabricated
	MActorAnmData* getUnk40() { return unk40; }
	TMapObjBase* getObj(int i) { return (TMapObjBase*)unk18[i]; }
	J3DDrawBuffer* getDrawBufferAfterIndirectOpa()
	{
		return mDrawBufferAfterIndirectOpa->getDrawBuffer();
	}
	J3DDrawBuffer* getDrawBufferAfterIndirectXlu()
	{
		return mDrawBufferAfterIndirectXlu->getDrawBuffer();
	}
	const JGeometry::TVec3<f32>& getUnk44() { return unk44; }

	SDLModelData* getSurfGessoModelData() { return mSurfGessoModelData; }
	J3DMaterialTable* getSkyMatTable() { return mSkyMatTable; }
	J3DMaterialTable* getNozzleItemMatTable() { return mNozzleItemMatTable; }
	J3DMaterialTable* getNozzleBoxMatTable() { return mNozzleBoxMatTable; }
	J3DMaterialTable* getFlowerMatTable() { return mFlowerMatTable; }
	J3DMaterialTable* getArrowBoardMatTable() { return mArrowBoardMatTable; }
	J3DMaterialTable* getWoodBoxMatTable() { return mWoodBoxMatTable; }
	J3DMaterialTable* getBarrelMatTable() { return mBarrelMatTable; }
	J3DMaterialTable* getBrickBlockMatTable() { return mBrickBlockMatTable; }
	J3DMaterialTable* getWaterMelonBlockMatTable()
	{
		return mWaterMelonBlockMatTable;
	}
	J3DMaterialTable* getBiancoMatTable() { return mBiancoMatTable; }
	J3DMaterialTable* getLeafBoatMatTable() { return mLeafBoatMatTable; }
	J3DMaterialTable* getRiccoShipMatTable() { return mRiccoShipMatTable; }
	J3DMaterialTable* getSandBombBaseMatTable()
	{
		return mSandBombBaseMatTable;
	}
	J3DMaterialTable* getMirrorMatTable() { return mMirrorMatTable; }
	ResTIMG* getCogwheelRopeTexture() { return mCogwheelRopeTexture; }
	ResTIMG* getBridgeRopeTexture() { return mBridgeRopeTexture; }

public:
	/* 0x40 */ MActorAnmData* unk40;
	/* 0x44 */ JGeometry::TVec3<f32> unk44;
	/* 0x50 */ JDrama::TDrawBufObj* mDrawBufferSunOpa;
	/* 0x54 */ JDrama::TDrawBufObj* mDrawBufferSunXlu;
	/* 0x58 */ JDrama::TDrawBufObj* mDrawBufferShadowOpa;
	/* 0x5C */ JDrama::TDrawBufObj* mDrawBufferShadowXlu;
	/* 0x60 */ JDrama::TDrawBufObj* mDrawBufferAfterIndirectOpa;
	/* 0x64 */ JDrama::TDrawBufObj* mDrawBufferAfterIndirectXlu;
	/* 0x68 */ J3DMaterialTable* mSkyMatTable;
	/* 0x6C */ J3DMaterialTable* mNozzleItemMatTable;
	/* 0x70 */ J3DMaterialTable* mNozzleBoxMatTable;
	/* 0x74 */ J3DMaterialTable* mFlowerMatTable;
	/* 0x78 */ J3DMaterialTable* mArrowBoardMatTable;
	/* 0x7C */ J3DMaterialTable* mWoodBoxMatTable;
	/* 0x80 */ J3DMaterialTable* mBarrelMatTable;
	/* 0x84 */ J3DMaterialTable* mBrickBlockMatTable;
	/* 0x88 */ J3DMaterialTable* mWaterMelonBlockMatTable;
	/* 0x8C */ J3DMaterialTable* mBiancoMatTable;
	/* 0x90 */ J3DMaterialTable* mLeafBoatMatTable;
	/* 0x94 */ J3DMaterialTable* mRiccoShipMatTable;
	/* 0x98 */ SDLModelData* mSurfGessoModelData;
	/* 0x9C */ MActor* mRedGesso;
	/* 0xA0 */ MActor* mYellowGesso;
	/* 0xA4 */ MActor* mGreenGesso;
	/* 0xA8 */ GXColorS10 unkA8;
	/* 0xB0 */ GXColorS10 unkB0;
	/* 0xB8 */ GXColorS10 unkB8;
	/* 0xC0 */ J3DMaterialTable* mSandBombBaseMatTable;
	/* 0xC4 */ J3DMaterialTable* mMirrorMatTable;
	/* 0xC8 */ ResTIMG* mCogwheelRopeTexture;
	/* 0xCC */ ResTIMG* mBridgeRopeTexture;
	/* 0xD0 */ JGeometry::TVec3<f32> unkD0;
};

#endif
