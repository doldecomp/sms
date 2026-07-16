#ifndef MOVE_BG_MAP_OBJ_SIRENA_HPP
#define MOVE_BG_MAP_OBJ_SIRENA_HPP

#include <MarioUtil/ModelUtil.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <Strategic/HitActor.hpp>

// TODO: mark virtual methods as such

class TCasinoPanelGate;
class TSlotDrum;
class TItemSlotDrum;
class TRouletteSw;

// TODO: TMsRange<T> is only evidenced by two UNUSED dtor symbols in this TU
// (__dt__11TMsRange<f>Fv / __dt__11TMsRange<l>Fv, 0x40 each). Its members and
// whether the dtor is virtual are unknown - the shape below is a guess needed
// so TItemSlotDrum's inlining is right. Needs human review.
template <typename T> class TMsRange {
public:
	TMsRange() { }
	~TMsRange() { }

public:
	/* 0x0 */ T mMin;
	/* 0x4 */ T mMax;
};

class TPictureTelesa : public TWaterHitPictureHideObj {
public:
	TPictureTelesa(const char* name = "テルサの絵")
	    : TWaterHitPictureHideObj(name)
	{
	}
	void control();
	void touchActor(THitActor*);
	void afterFinishedAnim();
};

class TPanelRevolve : public TMapObjBase {
public:
	TPanelRevolve(const char* name = "回転棚")
	    : TMapObjBase(name)
	{
	}
	void control();
	void touchPlayer(THitActor*);
	BOOL receiveMessage(THitActor*, u32);
};

class TChestRevolve : public TMapObjBase {
public:
	TChestRevolve(const char* name = "回転棚")
	    : TMapObjBase(name)
	{
	}
	void control();
	u32 touchWater(THitActor*);
};

class TWarpAreaActor : public THitActor {
public:
	TWarpAreaActor(const char* name = "ワープエリア");
	void load(JSUMemoryInputStream&);
	void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x68 */ s16 unk68;
	/* 0x6A */ s16 unk6A;
};

class TSirenaCasinoRoof : public TMapObjBase {
public:
	TSirenaCasinoRoof(const char* name = "カジノ部屋天井")
	    : TMapObjBase(name)
	    , mMultiBtk(nullptr)
	{
	}
	u32 getSDLModelFlag() const { return 0; }
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void initMapObj();

public:
	/* 0x138 */ TMultiBtk* mMultiBtk;
};

class TSirenabossWall : public TMapObjBase {
public:
	TSirenabossWall(const char* name = "ボステレサ部屋壁")
	    : TMapObjBase(name)
	    , mMultiBtk(nullptr)
	{
	}
	u32 getSDLModelFlag() const { return 0; }
	void drawObject(JDrama::TGraphics*);
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void initMapObj();

public:
	/* 0x138 */ TMultiBtk* mMultiBtk;
};

class TSakuCasino : public TMapObjBase {
public:
	TSakuCasino(const char* name = "パネル柵");
	void calcRootMatrix();
	void loadAfter();
	void initMapObj();

public:
	/* 0x138 */ TMapCollisionWarp* unk138;
	/* 0x13C */ u8 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ TCasinoPanelGate* unk144;
};

class TSirenaRollMapObj : public TMapObjBase {
public:
	TSirenaRollMapObj(const char* name = "SirenaRollMapObj");
	virtual f32 getRollAngX(int) const { return 0.0f; }
	virtual f32 getRollAngY(int) const { return 0.0f; }
	virtual f32 getRollAngZ(int) const { return 0.0f; }

public:
	/* 0x138 */ f32* unk138;
	/* 0x13C */ f32* unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ s32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ s16 unk164;
};

class TCloset : public TSirenaRollMapObj {
public:
	TCloset(const char* name = "クローゼット");
	u32 touchWater(THitActor*);
	void calcRootMatrix();
	void moveObject();
	void initMapObj();
	virtual f32 getRollAngY(int idx) const { return unk13C[idx]; }

public:
	/* 0x168 */ TMapCollisionWarp* mMapCollisionWarp;
	/* 0x16C */ bool unk16C;
	/* 0x16D */ u8 unk16D;
};

class TDonchou : public TMapObjBase {
public:
	TDonchou(const char* name = "パネルカーテン");
	u32 touchWater(THitActor*);
	void calcRootMatrix();
	void loadAfter();
	void initMapObj();

public:
	/* 0x138 */ TMapCollisionWarp* unk138;
	/* 0x13C */ u8 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ TSlotDrum* unk144;
	/* 0x148 */ TItemSlotDrum* unk148;
	/* 0x14C */ u32 unk14C;
};

class TCasinoPanelGate : public TSirenaRollMapObj {
public:
	TCasinoPanelGate(const char* name = "カジノパネルゲート");
	u32 touchWater(THitActor*);
	void calcRootMatrix();
	void moveObject();
	void initMapObj();
	virtual f32 getRollAngX(int idx) const { return unk13C[idx]; }

public:
	/* 0x168 */ TMapCollisionWarp* mMapCollisionWarp;
	/* 0x16C */ bool unk16C;
	/* 0x16D */ u8 unk16D;
};

class TSlotDrum : public TSirenaRollMapObj {
public:
	TSlotDrum(const char* name = "スロットマシーン");
	u32 touchWater(THitActor*);
	void calcRootMatrix();
	void moveObject();
	virtual void initNeonMatColor();
	void initMapObj();
	virtual f32 getRollAngX(int idx) const { return unk13C[idx]; }

public:
	/* 0x168 */ s32 unk168;
	/* 0x16C */ u32 unk16C;
	/* 0x170 */ GXColorS10 unk170[3];
	/* 0x188 */ f32 unk188[3];
	/* 0x194 */ u8 unk194;
};

class TItemSlotDrum : public TSlotDrum {
public:
	TItemSlotDrum(const char* name = "スロットマシーン");
	int getResultFromAng(f32);
	int getForcastResult(int);
	// UNUSED (MAP: size 0x50) - never called, but must exist for inlining
	int getDrumResult(int);
	// UNUSED (MAP: size 0x8C) - never called, but must exist for inlining
	int getSlotResult();
	void generateItem();
	u32 touchWater(THitActor*);
	void calcRootMatrix();
	void moveObject();
	void loadAfter();

public:
	/* 0x198 */ u32 unk198;
	/* 0x19C */ u8 unk19C[3];
	/* 0x19F */ u8 unk19F[3];
	/* 0x1A2 */ u8 unk1A2;
	/* 0x1A4 */ u32 unk1A4;
	/* 0x1A8 */ f32 unk1A8;
};

class TRoulette : public TMapObjBase {
public:
	TRoulette(const char* name = "ルーレット");
	void switchStop();
	void setRollSp(f32);
	void calcRootMatrix();
	void moveObject();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void initMapObj();

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ u8 unk140;
	/* 0x141 */ u8 unk141;
	/* 0x142 */ u8 unk142;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ s16 unk148;
	/* 0x14A */ s16 unk14A;
	/* 0x14C */ s16 unk14C;
	/* 0x14E */ s16 unk14E;
	/* 0x150 */ TRouletteSw* unk150;
};

class TRouletteSw : public THitActor {
public:
	TRouletteSw(const char* name = "ルーレットスイッチ")
	    : THitActor(name)
	{
	}
	void perform(u32 cue, JDrama::TGraphics* graphics);
	BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TRoulette* unk68;
	/* 0x6C */ u8 unk6C;
};

#endif
