#ifndef MAP_MAP_WIRE_MANAGER_HPP
#define MAP_MAP_WIRE_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Strategic/TakeActor.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

class TMapWireActorManager;
class TMapWire;

class TMapWireActor : public TTakeActor {
public:
	MtxPtr getTakingMtx() { return nullptr; }
	void checkTakingActor();
	void getPosInWire() const;
	void getTipPoints(JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*) const;
	BOOL receiveMessage(THitActor* sender, u32 message);
	void init(TMapWireActorManager*);
	TMapWireActor(const char*);

	static f32 mCommonAttackRadius;
	static f32 mCommonAttackHeight;

public:
	/* 0x70 */ u8 unk70;
	/* 0x74 */ void* unk74;
};

class TMapWireActorManager {
public:
	void doActorToWire();
	void doWireToActor();
	TMapWireActorManager(TTakeActor*);

public:
	/* 0x0 */ TTakeActor* unk0;
	/* 0x4 */ TMapWireActor unk4;
	/* 0x7C */ u32 unk7C;
};

class TMapWireManager;

TMapWireManager* gpMapWireManager;

class TMapWireManager : public JDrama::TViewObj {
public:
	TMapWire* getWire(int index) const;
	u32 getWireNo(const JGeometry::TVec3<f32>&) const;
	void getPointPosInNthWire(int, const JGeometry::TVec3<f32>&,
	                          JGeometry::TVec3<f32>*) const;
	void getPointPosInWire(const JGeometry::TVec3<f32>&,
	                       JGeometry::TVec3<f32>*) const;
	void perform(u32, JDrama::TGraphics*);
	void entry(TTakeActor*);
	void loadAfter();
	void load(JSUMemoryInputStream&);
	TMapWireManager(const char*);

	static JUtility::TColor mUpperSurface;
	static JUtility::TColor mLowerSurface;

	// fabricated
	static TMapWire* getGlobalWire(int index)
	{
		return gpMapWireManager->unk18[index];
	}

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ TMapWire** unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ TMapWireActorManager** unk24;
	/* 0x28 */ u16 unk28;
};

#endif
