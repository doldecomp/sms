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
	SDLModelData* getDataByName(const char* name) const;
	int getIndex(const char* name) const;
	SDLModelData* getNthData(int n) const;
	SDLModelData* createAndKeepData(const char* name, u32 flags);
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

	MActor* createMActorFromAllBmd(u32);
	MActor* createMActor(const char*, u32);
	MActor* createMActorFromNthData(int, u32);
	MActor* createMActorFromDefaultBmd(const char*, u32);
	MActor* getMActor(const char*) const;
	MActor* createAndRegister(SDLModelData*, u32);
	MActorAnmData* getMActorAnmData() const { return unkC; }

	const TModelDataKeeper* getKeeper() const { return unk0; }
	TModelDataKeeper* getKeeper() { return unk0; }

public:
	/* 0x0 */ TModelDataKeeper* unk0;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ MActor** unk8;
	/* 0xC */ MActorAnmData* unkC;
	/* 0x10 */ u16* unk10;
	/* 0x14 */ u32 unk14;
};

#endif
