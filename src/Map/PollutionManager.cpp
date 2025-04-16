#include <Map/PollutionManager.hpp>
#include <Map/PollutionLayer.hpp>
#include <System/MarDirector.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TPollutionManager* gpPollution;

void TPollutionManager::stampModel(J3DModel* model)
{
	for (int i = 0; i < getJointModelNum(); ++i)
		if (getLayer(i)->getPlaneType() == 0)
			getLayer(i)->stampModel(model);
}

void TPollutionManager::stamp(u16 param_1, f32 param_2, f32 param_3,
                              f32 param_4, f32 param_5)
{
	for (int i = 0; i < getJointModelNum(); ++i) {
		TPollutionLayer* layer = getLayer(i);
		layer->stamp(param_1, param_2, param_3, param_4, param_5);
	}
}

void TPollutionManager::clean(f32 param_1, f32 param_2, f32 param_3,
                              f32 param_4)
{
	if (gpMarDirector->map == 1 && param_2 < -10.0f)
		return;

	stamp(0, param_1, param_2, param_3, param_4);
}

void TPollutionManager::stampGround(u16 param_1, f32 param_2, f32 param_3,
                                    f32 param_4, f32 param_5)
{
	for (int i = 0; i < getJointModelNum(); ++i)
		if (getLayer(i)->getPlaneType() == 0)
			getLayer(i)->stamp(param_1, param_2, param_3, param_4, param_5);
}

u16 TPollutionManager::getPollutionType(f32 param_1, f32 param_2,
                                        f32 param_3) const
{
	for (int i = 0; i < getJointModelNum(); ++i)
		if (getLayer(i)->isInArea(param_1, param_2, param_3))
			return getLayer(i)->getPollutionType();

	return 0xA;
}

u32 TPollutionManager::getPollutionDegree() const
{
	u32 totalDegree = 0;
	for (int i = 0; i < getJointModelNum(); ++i) {
		TPollutionLayer* layer = getLayer(i);
		totalDegree += layer->getPollutionDegree();
	}
	return totalDegree;
}

void TPollutionManager::isProhibit(f32, f32, f32) const { }

bool TPollutionManager::isPolluted(f32 param_1, f32 param_2, f32 param_3) const
{
	for (int i = 0; i < getJointModelNum(); ++i) {
		TPollutionLayer* layer = getLayer(i);
		if (layer->isPolluted(param_1, param_2, param_3))
			return true;
	}

	return false;
}

void TPollutionManager::subtractFromYMap(f32, f32, f32) const { }

static void dummy()
{
	(Vec) { 0.0f, 0.0f, 0.0f };
	(Vec) { 1.0f, 1.0f, 1.0f };
}

// TODO: move to it's proper place
static u32 mCleanedDegree;

bool TPollutionManager::cleanedAll() const
{
	return getPollutionDegree() < mCleanedDegree ? true : false;
}

void TPollutionManager::draw() { }

void TPollutionManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x1000000) {
		getCounterObj().countObjDegree();
	} else if (param_1 & 0x2000000) {
		u8 uVar1 = param_1 >> 0x10;
		if (uVar1 == 0)
			getCounterLayer().calcViewMtx();

		getCounterLayer().countTexDegree(uVar1);

		int last = getJointModelNum() - 1;
		if (uVar1 == last)
			getCounterLayer().resetTask();
	} else {
		TJointModelManager::perform(param_1, param_2);
	}
}

TJointModel* TPollutionManager::newJointModel(int param_1) const
{
	switch (getLayerInfo(param_1)->unk4) {
	case 0:
		return new TPollutionLayer;
		break;
	case 1:
		return new TPollutionLayer;
		break;
	case 4:
		return new TPollutionLayerWallPlusZ;
		break;
	case 5:
		return new TPollutionLayerWallMinusZ;
		break;
	case 2:
		return new TPollutionLayerWallPlusX;
		break;
	case 3:
		return new TPollutionLayerWallMinusX;
		break;
	case 6:
		return new TPollutionLayerWave;
		break;
	default:
		return nullptr;
	}
}

void TPollutionManager::setDataAddress(TPollutionManager::TPollutionInfo* info)
{
	// pointer patching ewwww
	info->unk4 = (TPollutionLayerInfo*)((u8*)info->unk4 + (u32)info);
	unk6C      = info->unk4;
	for (int i = 0; i < getJointModelNum(); ++i)
		unk6C[i].unk28 += (u32)info;
}

void TPollutionManager::initPollutionInfo()
{
	TPollutionInfo* info
	    = (TPollutionInfo*)JKRFileLoader::getGlbResource("/scene/map/ymap.ymp");

	if (!info)
		return;

	mJointModelNum = info->unk0;
	setDataAddress(info);

	if (gpMarDirector->map == 0x9 && gpMarDirector->_07D != 0x7) {
		static const char* mare_name_table[] = {
			"pollution00", "pollution01", "pollution02", "pollution03",
			"pollution04", "pollution05", "pollution06", "pollutionA",
			"pollutionB",  nullptr,
		};
		initJointModel("scene/map/pollution", mare_name_table);
	} else {
		static const char* name_table[] = {
			"pollution00", "pollution01", "pollution02", "pollution03",
			"pollution04", "pollution05", "pollution06", "pollution07",
			"pollution08", "pollution09", "pollution10", "pollution11",
			"pollution12", "pollution13", "pollution14", "pollution15",
			"pollution16", "pollution17", "pollution18", "pollution19",
			nullptr,
		};
		initJointModel("scene/map/pollution", name_table);
	}
}

void TPollutionManager::load(JSUMemoryInputStream& stream)
{
	TJointModelManager::load(stream);

	initPollutionInfo();

	if (mJointModelNum != 0) {
		unk204 = (ResTIMG*)JKRGetResource("/common/map/pollute.bti");
		unk208 = (ResTIMG*)JKRGetResource("/common/map/clean.bti");

		getCounterLayer().init(getJointModelNum(), 0xf, 5);

		for (int i = 0; i < getJointModelNum(); ++i)
			getCounterLayer().registerLayer(getLayer(i), &getLayer(i)->unk34);

		gpPollution->getCounterObj().init(0x1E);

		getCounterLayer().registerTexStamp(0, 0xff, unk208);
		getCounterLayer().registerTexStamp(1, 0xff, unk204);
	}
}

TPollutionManager::TPollutionManager(const char* name)
    : TJointModelManager(name)
    , unk6C(0)
    , unk204(0)
    , unk208(0)
    , unk20C(0)
{
	gpPollution = this;
}
