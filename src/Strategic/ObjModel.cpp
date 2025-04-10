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

MActor* TMActorKeeper::createAndRegister(SDLModelData* param_1, u32 param_2)
{
	SDLModel* model = new SDLModel(param_1, param_2, 1);
	MActor* actor   = new MActor(unkC);
	actor->setModel(model, param_2);
	unk8[unk6] = actor;
	++unk6;
	return actor;
}

MActor* TMActorKeeper::getMActor(const char* name) const
{
	if (!getKeeper())
		return unk8[0];

	int index = getKeeper()->getIndex(name);
	for (int i = 0; i < unk6; ++i) {
		if (index == unk10[i])
			return unk8[i];
	}

	return nullptr;
}

MActor* TMActorKeeper::createMActorFromDefaultBmd(const char* param_1,
                                                  u32 param_2)
{
	SDLModelData* data = unk0->loadModelData("default.bmd", unk14, param_1);
	unkC               = new MActorAnmData;
	unkC->init(param_1, nullptr);
	return createAndRegister(data, param_2);
}

MActor* TMActorKeeper::createMActorFromNthData(int param_1, u32 param_2)
{
	TModelDataKeeper* keeper = unk0;
	unk10[unk6]              = param_1;
	SDLModelData* data       = keeper->getNthData(param_1);
	return createAndRegister(data, param_2);
}

MActor* TMActorKeeper::createMActor(const char* param_1, u32 param_2)
{
	TModelDataKeeper* keeper = getKeeper();

	int index = keeper->getIndex(param_1);

	if (index < 0) {
		keeper->createAndKeepData(param_1, unk14);
		index = keeper->getIndex(param_1);
	}

	return createMActorFromNthData(index, param_2);
}

MActor* TMActorKeeper::createMActorFromAllBmd(u32 param_1)
{
	int num = unk0->getModelDataNum();
	for (int i = 0; i < num; ++i)
		createMActorFromNthData(i, param_1);
}

TMActorKeeper::TMActorKeeper(TLiveManager* param_1, u16 param_2)
{
	unk4  = param_2;
	unk6  = 0;
	unk8  = new MActor*[param_2];
	unkC  = nullptr;
	unk10 = new u16[param_2];
	unk14 = 0;
	memset(unk8, 0, param_2 * sizeof(unk8[0]));
	memset(unk10, 0, param_2 * sizeof(unk10[0]));

	if (param_1) {
		unk0 = param_1->getModelDataKeeper();
		unkC = param_1->getMActorAnmData();
	}
}

TMActorKeeper::TMActorKeeper(TLiveManager* param_1)
{
	if (param_1) {
		unk0 = param_1->getModelDataKeeper();
		unkC = param_1->getMActorAnmData();
	}

	unk4  = unk0->getModelDataNum();
	unk6  = 0;
	unk8  = new MActor*[unk4];
	unk10 = new u16[unk4];
	unk14 = 0;
	memset(unk8, 0, unk4 * sizeof(unk8[0]));
	memset(unk10, 0, unk4 * sizeof(unk10[0]));
}
