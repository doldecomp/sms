#include <Strategic/ObjModel.hpp>
#include <Strategic/LiveManager.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <stdio.h>
#include <dolphin/types.h>

bool TModelDataNode::isSameName(const char* file_name, u16 key) const
{
	if (key == mKey && strcmp(file_name, mFileName) == 0)
		return true;
	else
		return false;
}

void TModelDataNode::registerDataAndJoinNewNode(SDLModelData* data,
                                                const char* bmd_file)
{
	mData     = data;
	mFileName = bmd_file;
	mKey      = JDrama::TNameRef::calcKeyCode(bmd_file);
	mNext     = new TModelDataNode;
}

TModelDataNode::TModelDataNode()
    : mData(nullptr)
    , mFileName(nullptr)
    , mKey(0)
    , mNext(nullptr)
{
}

SDLModelData* TModelDataKeeper::loadModelData(const char* file_name, u32 flags,
                                              const char* folder)
{
	char fullPath[256];
	sprintf(fullPath, "%s/%s", folder, file_name);
	void* res             = JKRGetResource(fullPath);
	J3DModelData* data    = J3DModelLoaderDataBase::load(res, flags);
	SDLModelData* sdlData = new SDLModelData(data);
	return sdlData;
}

SDLModelData* TModelDataKeeper::createAndKeepData(const char* file_name,
                                                  u32 flags)
{
	TModelDataNode* node = &mHead;
	while (node->getNext())
		node = node->getNext();

	SDLModelData* data = loadModelData(file_name, flags, mFolder);
	node->registerDataAndJoinNewNode(data, file_name);
	return data;
}

SDLModelData* TModelDataKeeper::getNthData(int n) const
{
	const TModelDataNode* node = &mHead;
	for (int i = 0; i < n; ++i)
		node = node->getNext();
	return node->getData();
}

int TModelDataKeeper::getIndex(const char* file_name) const
{
	u16 key = JDrama::TNameRef::calcKeyCode(file_name);

	const TModelDataNode* node = &mHead;
	for (u32 i = 0; node && node->getData(); ++i) {
		if (node->isSameName(file_name, key))
			return i;
		node = node->getNext();
	}
	return -1;
}

SDLModelData* TModelDataKeeper::getDataByName(const char* file_name) const
{
	int idx = getIndex(file_name);
	if (idx < 0)
		return nullptr;
	return getNthData(idx);
}

int TModelDataKeeper::getModelDataNum() const
{
	int count = 0;

	const TModelDataNode* node = &mHead;
	while (node && node->getData()) {
		++count;
		node = node->getNext();
	}

	return count;
}

TModelDataKeeper::TModelDataKeeper(const char* folder)
    : mFolder(folder)
{
}

MActor* TMActorKeeper::createAndRegister(SDLModelData* model_data,
                                         u32 model_flags)
{
	SDLModel* model = new SDLModel(model_data, model_flags, 1);
	MActor* actor   = new MActor(mActorAnmData);
	actor->setModel(model, model_flags);
	mActors[mActorNum] = actor;
	++mActorNum;
	return actor;
}

MActor* TMActorKeeper::getMActor(const char* model_data_name) const
{
	if (!getModelDataKeeper())
		return mActors[0];

	int index = getModelDataKeeper()->getIndex(model_data_name);
	for (int i = 0; i < mActorNum; ++i) {
		if (index == mActorModelDataIndices[i])
			return mActors[i];
	}

	return nullptr;
}

MActor* TMActorKeeper::createMActorFromDefaultBmd(const char* folder, u32 flags)
{
	SDLModelData* data = mModelDataKeeper->loadModelData(
	    "default.bmd", mModelLoaderFlags, folder);
	mActorAnmData = new MActorAnmData;
	mActorAnmData->init(folder, nullptr);
	return createAndRegister(data, flags);
}

MActor* TMActorKeeper::createMActorFromNthData(int n, u32 flags)
{
	TModelDataKeeper* keeper          = mModelDataKeeper;
	mActorModelDataIndices[mActorNum] = n;
	SDLModelData* data                = keeper->getNthData(n);
	return createAndRegister(data, flags);
}

MActor* TMActorKeeper::createMActor(const char* model_data_name, u32 flags)
{
	TModelDataKeeper* keeper = getModelDataKeeper();

	int index = keeper->getIndex(model_data_name);

	if (index < 0) {
		keeper->createAndKeepData(model_data_name, mModelLoaderFlags);
		index = keeper->getIndex(model_data_name);
	}

	return createMActorFromNthData(index, flags);
}

void TMActorKeeper::createMActorFromAllBmd(u32 flags)
{
	int num = mModelDataKeeper->getModelDataNum();
	for (int i = 0; i < num; ++i)
		createMActorFromNthData(i, flags);
}

TMActorKeeper::TMActorKeeper(TLiveManager* manager, u16 max_mactors)
{
	mMActorCapacity        = max_mactors;
	mActorNum              = 0;
	mActors                = new MActor*[max_mactors];
	mActorAnmData          = nullptr;
	mActorModelDataIndices = new u16[max_mactors];
	mModelLoaderFlags      = 0;
	memset(mActors, 0, max_mactors * sizeof(mActors[0]));
	memset(mActorModelDataIndices, 0,
	       max_mactors * sizeof(mActorModelDataIndices[0]));

	if (manager) {
		mModelDataKeeper = manager->getModelDataKeeper();
		mActorAnmData    = manager->getMActorAnmData();
	}
}

TMActorKeeper::TMActorKeeper(TLiveManager* manager)
{
	if (manager) {
		mModelDataKeeper = manager->getModelDataKeeper();
		mActorAnmData    = manager->getMActorAnmData();
	}

	mMActorCapacity        = mModelDataKeeper->getModelDataNum();
	mActorNum              = 0;
	mActors                = new MActor*[mMActorCapacity];
	mActorModelDataIndices = new u16[mMActorCapacity];
	mModelLoaderFlags      = 0;
	memset(mActors, 0, mMActorCapacity * sizeof(mActors[0]));
	memset(mActorModelDataIndices, 0,
	       mMActorCapacity * sizeof(mActorModelDataIndices[0]));
}
