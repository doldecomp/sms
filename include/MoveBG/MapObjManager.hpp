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

public:
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
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

	J3DMaterialTable* loadMatTable(const char*);
	void initDrawBuffer();
	void entryStaticDrawBufferSun(J3DModel*);
	void entryStaticDrawBufferShadow(J3DModel*);

	// fabricated
	MActorAnmData* getUnk40() { return unk40; }
	J3DMaterialTable* getUnk68() { return unk68; }
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

public:
	/* 0x40 */ MActorAnmData* unk40;
	/* 0x44 */ JGeometry::TVec3<f32> unk44;
	/* 0x50 */ JDrama::TDrawBufObj* mDrawBufferSunOpa;
	/* 0x54 */ JDrama::TDrawBufObj* mDrawBufferSunXlu;
	/* 0x58 */ JDrama::TDrawBufObj* mDrawBufferShadowOpa;
	/* 0x5C */ JDrama::TDrawBufObj* mDrawBufferShadowXlu;
	/* 0x60 */ JDrama::TDrawBufObj* mDrawBufferAfterIndirectOpa;
	/* 0x64 */ JDrama::TDrawBufObj* mDrawBufferAfterIndirectXlu;
	/* 0x68 */ J3DMaterialTable* unk68;
	/* 0x6C */ J3DMaterialTable* unk6C;
	/* 0x70 */ J3DMaterialTable* unk70;
	/* 0x74 */ J3DMaterialTable* unk74;
	/* 0x78 */ J3DMaterialTable* unk78;
	/* 0x7C */ J3DMaterialTable* unk7C;
	/* 0x80 */ J3DMaterialTable* unk80;
	/* 0x84 */ J3DMaterialTable* unk84;
	/* 0x88 */ J3DMaterialTable* unk88;
	/* 0x8C */ J3DMaterialTable* unk8C;
	/* 0x90 */ J3DMaterialTable* unk90;
	/* 0x94 */ J3DMaterialTable* unk94;
	/* 0x98 */ SDLModelData* mSurfGessoModelData;
	/* 0x9C */ MActor* mRedGesso;
	/* 0xA0 */ MActor* mYellowGesso;
	/* 0xA4 */ MActor* mGreenGesso;
	/* 0xA8 */ GXColorS10 unkA8;
	/* 0xB0 */ GXColorS10 unkB0;
	/* 0xB8 */ GXColorS10 unkB8;
	/* 0xC0 */ J3DMaterialTable* unkC0;
	/* 0xC4 */ J3DMaterialTable* unkC4;
	/* 0xC8 */ ResTIMG* unkC8;
	/* 0xCC */ ResTIMG* unkCC;
	/* 0xD0 */ Vec unkD0;
};

#endif
