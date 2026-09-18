#ifndef STRATEGIC_OBJ_MODEL_HPP
#define STRATEGIC_OBJ_MODEL_HPP

#include <dolphin/types.h>

class SDLModelData;

class TModelDataNode {
public:
	TModelDataNode();
	void registerDataAndJoinNewNode(SDLModelData* data, const char* name);
	bool isSameName(const char* name, u16 key) const;

	// fabricated
	SDLModelData* getData() const { return mData; }
	TModelDataNode* getNext() const { return mNext; }

private:
	/* 0x0 */ SDLModelData* mData;
	/* 0x4 */ const char* mName;
	/* 0x8 */ u16 mKey;
	/* 0xC */ TModelDataNode* mNext;
};

class TModelDataKeeper {
public:
	TModelDataKeeper(const char* folder);
	int getModelDataNum() const;
	// fabricated
	const char* getFolder() const { return mFolder; }
	const TModelDataNode* getHead() const { return &mHead; }
	SDLModelData* getDataByName(const char* name) const;
	int getIndex(const char* name) const;
	SDLModelData* getNthData(int n) const;
	SDLModelData* createAndKeepData(const char* name, u32 flags);
	// fabricated. One inline level above createAndKeepData, and the only
	// user is TMActorKeeper::createMActor: it puts loadModelData and
	// registerDataAndJoinNewNode at depth 3 there, where MWCC refuses them
	// exactly as retail does, while createAndKeepData's own emitted copy
	// still expands both at depth 1. Without the level createMActor inlines
	// loadModelData (frame 0x1a8 vs 0xa0) and then refuses createAndRegister.
	// Fully inlined everywhere, so the map has no symbol to name it from.
	SDLModelData* keepModelData(const char* name, u32 flags)
	{
		return createAndKeepData(name, flags);
	}
	static SDLModelData* loadModelData(const char* name, u32 flags,
	                                   const char* folder);

public:
	/* 0x0 */ const char* mFolder;
	/* 0x4 */ TModelDataNode mHead;
};

class TLiveManager;
class MActor;
class MActorAnmData;

class TMActorKeeper {
public:
	TMActorKeeper(TLiveManager*);
	TMActorKeeper(TLiveManager*, u16);

	void createMActorFromAllBmd(u32 flags);
	MActor* createMActor(const char* model_data_name, u32 flags);
	MActor* createMActorFromNthData(int n, u32 flags);
	MActor* createMActorFromDefaultBmd(const char* folder, u32 flags);
	MActor* getMActor(const char* name) const;
	MActor* createAndRegister(SDLModelData* model_data, u32 model_flags);
	MActorAnmData* getMActorAnmData() const { return mActorAnmData; }
	// The map links the const overload (weak, 0x8, from bossgesso.cpp) and has
	// no non-const one, and retail `bl`s it at every
	// getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr() site in
	// bosswanwan and bossgesso while a single accessor expands there. This
	// pair is the inline level that accounts for it: every one of those sites
	// holds a non-const keeper, so it goes through this forwarder and the
	// const one lands one level deeper, out of line, without pushing
	// getUnk2C() out with it. Spelling the conversion as a named
	// `const TMActorKeeper*` local instead of the cast costs
	// TSirenabossWall::initMapObj and TSirenaCasinoRoof::initMapObj their
	// exact match, and putting the level on TLiveActor instead is worth about
	// two points less everywhere.
	MActorAnmData* getMActorAnmData()
	{
		return static_cast<const TMActorKeeper*>(this)->getMActorAnmData();
	}

	const TModelDataKeeper* getModelDataKeeper() const
	{
		return mModelDataKeeper;
	}
	TModelDataKeeper* getModelDataKeeper() { return mModelDataKeeper; }

	MActor* getMActor(int index) { return mActors[index]; }

	u16 getActorNum() const { return mActorNum; }
	u32 getModelLoaderFlags() const { return mModelLoaderFlags; }


public:
	/* 0x0 */ TModelDataKeeper* mModelDataKeeper;
	/* 0x4 */ u16 mModelDataNum;
	/* 0x6 */ u16 mActorNum;
	/* 0x8 */ MActor** mActors;
	/* 0xC */ MActorAnmData* mActorAnmData;
	/* 0x10 */ u16* mActorModelDataIndices;
	/* 0x14 */ u32 mModelLoaderFlags;
};

#endif
