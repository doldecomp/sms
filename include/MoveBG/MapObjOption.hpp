#ifndef MOVE_BG_MAP_OBJ_OPTION_HPP
#define MOVE_BG_MAP_OBJ_OPTION_HPP

#include <MoveBG/MapObjBase.hpp>

class TFileLoadBlock : public TMapObjBase {
public:
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void makeBlockNoCard();
	void makeBlockNormal();
	void makeBlockRock();
	void pushed();

	// fabricated: the binding (a named pointer local rather than a direct
	// `return &unk144;`) is the whole point -- it is one 8-byte low-region
	// slot in every function that inlines pushed(), and it is needed at this
	// one emit site only.  See the TODO at pushed().
	const JGeometry::TVec3<f32>* getEffectPos() const
	{
		const JGeometry::TVec3<f32>* position = &unk144;
		return position;
	}
	TFileLoadBlock(const char* name = "ファイル読み込みブロック");

	enum {
		STATE_ROCKING = 2,
		// TODO: guessed; only used by the UNUSED makeBlockNoCard
		STATE_NO_CARD = 3,
	};

public:
	/* 0x138 */ u8 unk138;
	/* 0x13C */ TFileLoadBlock* unk13C;
	/* 0x140 */ TFileLoadBlock* unk140;
	/* 0x144 */ JGeometry::TVec3<f32> unk144;
};

class TMapObjOptionWall : public THitActor {
public:
	void onCollision();
	void offCollision();
	void init();
	TMapObjOptionWall(const char*);

public:
	/* 0x68 */ TMapCollisionWarp* unk68;
};

#endif
