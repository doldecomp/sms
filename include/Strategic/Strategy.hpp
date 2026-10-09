#ifndef STRATEGIC_STRATEGY_HPP
#define STRATEGIC_STRATEGY_HPP

#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <Strategic/HitActor.hpp>

enum TIdxGroupIndex {
	IDX_GROUP_MAP            = 0,  // マップグループ
	IDX_GROUP_EMPTY          = 1,  // 空グループ
	IDX_GROUP_MANAGER        = 2,  // マネージャーグループ
	IDX_GROUP_OBJECT         = 3,  // オブジェクトグループ
	IDX_GROUP_GRAFFITI       = 4,  // 落書きグループ
	IDX_GROUP_ITEM           = 5,  // アイテムグループ
	IDX_GROUP_PLAYER         = 6,  // プレーヤーグループ
	IDX_GROUP_ENEMY          = 7,  // 敵グループ
	IDX_GROUP_BOSS           = 8,  // ボスグループ
	IDX_GROUP_NPC            = 9,  // ＮＰＣグループ
	IDX_GROUP_WATER_PARTICLE = 10, // 水パーティクルグループ
	IDX_GROUP_INIT           = 11, // 初期化用グループ
	IDX_GROUP_LOADING        = 15, // only set while a group loads
	IDX_GROUP_NUM            = 16,
};

class TIdxGroupObj : public JDrama::TViewObjPtrListT<THitActor> {
public:
	TIdxGroupObj(const char* name = "<IdxGroupObj>")
	    : TViewObjPtrListT(name)
	    , unk20(0)
	{
	}

	virtual void loadSuper(JSUMemoryInputStream&);

	// fabricated
	void add(THitActor* const& obj) { getChildren().push_back(obj); }

public:
	/* 0x20 */ u32 unk20;
};

enum TStrategyHitCheckOffFlag {
	HIT_CHECK_OFF_PLAYER = 0x40,
	HIT_CHECK_OFF_WATER  = 0x80,
	HIT_CHECK_OFF_ENEMY  = 0x100,
	HIT_CHECK_OFF_BOSS   = 0x200,
	HIT_CHECK_OFF_NPC    = 0x400,
	HIT_CHECK_OFF_OBJECT = 0x800,
};

class TStrategy;

extern TStrategy* gpStrategy;

class TStrategy : public JDrama::TViewObj {
public:
	TStrategy(const char* name = "<TStrategy>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// fabricated
	bool isHitCheckOff(u16 flag) const { return mHitCheckOffFlags & flag; }

public:
	/* 0x10 */ TIdxGroupObj* mGroups[IDX_GROUP_NUM];
	/* 0x50 */ u16 mHitCheckOffFlags;
};

#endif
