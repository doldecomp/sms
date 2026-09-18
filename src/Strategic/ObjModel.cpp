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

	SDLModelData* data = loadModelData(name, flags, getFolder());
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

	const TModelDataNode* node = getHead();
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
		keeper->keepModelData(model_data_name, getModelLoaderFlags());
		index = keeper->getIndex(model_data_name);
	}

	// createMActorFromNthData's body, duplicated in the original: routing this
	// through the method leaves createAndRegister at depth 2, where MWCC
	// refuses it and the whole allocator ranking shifts (77.5% vs 100%).
	mActorModelDataIndices[getActorNum()] = index;
	SDLModelData* data                = keeper->getNthData(index);
	return createAndRegister(data, flags);
}

// TODO: frame and instruction sequence are exact; the only residue is that
// retail runs the inlined getModelDataNum node walk entirely in r3
// (`lwz r3, 0(r3)` / `addi r3, r3, 4` / `lwz r3, 0xc(r3)`) where we keep the
// keeper in r5 and the node in r4, i.e. MWCC does not coalesce the inlined
// callee's `this` binding with its first local. The emitted out-of-line
// getModelDataNum is byte-exact, so the body is right and the lever is at the
// call site. Measured, all leaving the same five operand differences: a named
// TModelDataKeeper* local, a static_cast to the const receiver, `getHead()`
// for `&mHead`, a for-loop or split declaration in the callee, and a
// while-loop here. `mModelDataKeeper` raw is -8 of frame and 18 differences; a
// TU-local forwarder above getModelDataNum and spelling the walk out here
// (getHead() or &...->mHead) are both far worse (24 / 38 differences).
MActor* TMActorKeeper::createMActorFromAllBmd(u32 flags)
{
	int num = getModelDataKeeper()->getModelDataNum();
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

	mModelDataNum          = getModelDataKeeper()->getModelDataNum();
	mActorNum              = 0;
	mActors                = new MActor*[mModelDataNum];
	mActorModelDataIndices = new u16[mModelDataNum];
	mModelLoaderFlags      = 0;
	memset(mActors, 0, mModelDataNum * sizeof(mActors[0]));
	memset(mActorModelDataIndices, 0,
	       mModelDataNum * sizeof(mActorModelDataIndices[0]));
}
