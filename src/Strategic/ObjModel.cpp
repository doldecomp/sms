#include <Strategic/ObjModel.hpp>
#include <Strategic/LiveManager.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <stdio.h>
#include <types.h>

bool TModelDataNode::isSameName(const char* name, u16 key) const
{
	if (key == mKey && strcmp(name, mName) == 0)
		return true;
	else
		return false;
}

void TModelDataNode::registerDataAndJoinNewNode(SDLModelData* data,
                                                const char* name)
{
	mData = data;
	mName = name;
	mKey  = JDrama::TNameRef::calcKeyCode(name);
	mNext = new TModelDataNode;
}

TModelDataNode::TModelDataNode()
    : mData(nullptr)
    , mName(nullptr)
    , mKey(0)
    , mNext(nullptr)
{
}

SDLModelData* TModelDataKeeper::loadModelData(const char* name, u32 flags,
                                              const char* folder)
{
	char fullPath[256];
	sprintf(fullPath, "%s/%s", folder, name);
	void* res             = JKRGetResource(fullPath);
	J3DModelData* data    = J3DModelLoaderDataBase::load(res, flags);
	SDLModelData* sdlData = new SDLModelData(data);
	return sdlData;
}

SDLModelData* TModelDataKeeper::createAndKeepData(const char* name, u32 flags)
{
	TModelDataNode* node = &mHead;
	while (node->getNext())
		node = node->getNext();

	SDLModelData* data = loadModelData(name, flags, mFolder);
	node->registerDataAndJoinNewNode(data, name);
	return data;
}

SDLModelData* TModelDataKeeper::getNthData(int n) const
{
	const TModelDataNode* node = &mHead;
	for (int i = 0; i < n; ++i)
		node = node->getNext();
	return node->getData();
}

int TModelDataKeeper::getIndex(const char* name) const
{
	u16 key = JDrama::TNameRef::calcKeyCode(name);

	const TModelDataNode* node = &mHead;
	for (u32 i = 0; node && node->getData(); ++i) {
		if (node->isSameName(name, key))
			return i;
		node = node->getNext();
	}
	return -1;
}

SDLModelData* TModelDataKeeper::getDataByName(const char* name) const
{
	int idx = getIndex(name);
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

MActor* TMActorKeeper::getMActor(const char* name) const
{
	if (!getModelDataKeeper())
		return mActors[0];

	int index = getModelDataKeeper()->getIndex(name);
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

MActor* TMActorKeeper::createMActorFromAllBmd(u32 flags)
{
	int num = mModelDataKeeper->getModelDataNum();
	for (int i = 0; i < num; ++i)
		createMActorFromNthData(i, flags);
}

TMActorKeeper::TMActorKeeper(TLiveManager* param_1, u16 param_2)
{
	mModelDataNum          = param_2;
	mActorNum              = 0;
	mActors                = new MActor*[param_2];
	mActorAnmData          = nullptr;
	mActorModelDataIndices = new u16[param_2];
	mModelLoaderFlags      = 0;
	memset(mActors, 0, param_2 * sizeof(mActors[0]));
	memset(mActorModelDataIndices, 0,
	       param_2 * sizeof(mActorModelDataIndices[0]));

	if (param_1) {
		mModelDataKeeper = param_1->getModelDataKeeper();
		mActorAnmData    = param_1->getMActorAnmData();
	}
}

TMActorKeeper::TMActorKeeper(TLiveManager* param_1)
{
	if (param_1) {
		mModelDataKeeper = param_1->getModelDataKeeper();
		mActorAnmData    = param_1->getMActorAnmData();
	}

	mModelDataNum          = mModelDataKeeper->getModelDataNum();
	mActorNum              = 0;
	mActors                = new MActor*[mModelDataNum];
	mActorModelDataIndices = new u16[mModelDataNum];
	mModelLoaderFlags      = 0;
	memset(mActors, 0, mModelDataNum * sizeof(mActors[0]));
	memset(mActorModelDataIndices, 0,
	       mModelDataNum * sizeof(mActorModelDataIndices[0]));
}
