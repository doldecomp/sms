#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/HitActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

#pragma opt_strength_reduction off

JDrama::TNameRef* TObjManager::searchF(u16 key, const char* name)
{
	JDrama::TNameRef* res = JDrama::TNameRef::searchF(key, name);
	if (res)
		return res;

	for (int i = 0; i < unk14; ++i) {
		JDrama::TNameRef* r = unk18[i]->searchF(key, name);
		if (r)
			return r;
	}

	return nullptr;
}

TModelDataKeeper* TObjManager::getModelDataKeeper()
{
	if (!unk24)
		createModelData();
	return unk24;
}

void TObjManager::createAnmData()
{
	unk20 = new MActorAnmData;
	unk20->init(nullptr, nullptr);
}

void TObjManager::createModelData() { }

void TObjManager::createModelDataArrayBase(const TModelDataLoadEntry* entries,
                                           const char* param_2)
{
	unk28 = 0;
	unk2C = 0;
	while (entries[unk28].unk0) {
		if (entries[unk28].unk8 & 1)
			++unk2C;
		++unk28;
	}

	unk24 = new TModelDataKeeper(param_2);
	for (int i = 0; i < unk28; ++i)
		unk24->createAndKeepData(entries[i].unk0, entries[i].unk4);
}

void TObjManager::createModelDataArray(const TModelDataLoadEntry* entries)
{
	createModelDataArrayBase(entries, nullptr);
}

void TObjManager::perform(u32 param_1, JDrama::TGraphics* param_2) { }

MActorAnmData* TObjManager::getMActorAnmData()
{
	if (!unk20)
		createAnmData();
	return unk20;
}

void TObjManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	char buffer[0x100];
	stream.readString(buffer, 0x100);
	unk1C
	    = JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(buffer);
	unk10 = stream.readU32();
	unk18 = new THitActor*[unk10];
}

void TObjManager::manageObj(THitActor* obj)
{
	unk18[unk14] = obj;
	++unk14;
}

TObjManager::TObjManager(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
    , unk18(nullptr)
    , unk1C(nullptr)
    , unk20(nullptr)
    , unk24(nullptr)
    , unk28(0)
    , unk2C(0)
    , unk30(0)
{
}

void* TObjChara::getRes(const char*) const { }

TObjChara::~TObjChara() { }

void TObjChara::load(JSUMemoryInputStream&) { }
