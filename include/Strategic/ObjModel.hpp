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

	MActor* createMActorFromAllBmd(u32 flags);
	MActor* createMActor(const char* model_data_name, u32 flags);
	MActor* createMActorFromNthData(int n, u32 flags);
	MActor* createMActorFromDefaultBmd(const char* folder, u32 flags);
	MActor* getMActor(const char* name) const;
	MActor* createAndRegister(SDLModelData* model_data, u32 model_flags);
	MActorAnmData* getMActorAnmData() const { return mActorAnmData; }

	const TModelDataKeeper* getModelDataKeeper() const
	{
		return mModelDataKeeper;
	}
	TModelDataKeeper* getModelDataKeeper() { return mModelDataKeeper; }

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
