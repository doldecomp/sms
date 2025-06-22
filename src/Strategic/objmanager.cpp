#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/HitActor.hpp>
#include <System/TimeRec.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <dolphin/os.h>
#include <stdio.h>

#include <M3DUtil/InfectiousStrings.hpp> // TODO: removeme

void TObjChara::load(JSUMemoryInputStream& stream)
{
	JDrama::TCharacter::load(stream);
	mFolder = stream.readString();
}

TObjChara::~TObjChara() { }

void* TObjChara::getRes(const char* name) const
{
	char buffer[256];
	snprintf(buffer, 256, "%s%s", mFolder, name);
	return JKRGetResource(buffer);
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

void TObjManager::manageObj(THitActor* obj)
{
	unk18[unk14] = obj;
	++unk14;
}

void TObjManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	char buffer[0x100];
	stream.readString(buffer, 0x100);
	unk1C = (TObjChara*)JDrama::TNameRefGen::getInstance()
	            ->getRootNameRef()
	            ->search(buffer);
	unk10 = stream.readU32();
	unk18 = new THitActor*[unk10];
}

MActorAnmData* TObjManager::getMActorAnmData()
{
	if (!unk20)
		createAnmData();
	return unk20;
}

void TObjManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk30 & 1)
		TTimeRec::startTimer(0xffffffff);

	for (int i = 0; i < unk14; ++i)
		unk18[i]->testPerform(param_1, param_2);

	if (unk30 & 1)
		TTimeRec::endTimer();
}

void TObjManager::createModelDataArray(const TModelDataLoadEntry* entries)
{
	createModelDataArrayBase(entries, unk1C->getFolder());
}

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

void TObjManager::createModelData()
{
	static const TModelDataLoadEntry entry[2]
	    = { { "default.bmd", 0x10210000, 0 }, { nullptr, 0, 0 } };
	createModelDataArray(entry);
}

void TObjManager::createAnmData()
{
	unk20 = new MActorAnmData;
	unk20->init(unk1C->getFolder(), nullptr);
}

TModelDataKeeper* TObjManager::getModelDataKeeper()
{
	if (!unk24)
		createModelData();
	return unk24;
}

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
