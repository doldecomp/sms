#include <Map/PollutionLayer.hpp>
#include <Map/PollutionManager.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/os/OSCache.h>

TPollutionLayer::TPollutionLayer()
    : unk30(0)
    , unk32(0)
    , unk34(0)
    , unk38(0.0f)
    , unk3C(0.0f)
    , unk40(0.0f)
    , unk44(0.0f)
    , unk48(0)
    , unk4C(0)
    , unk50(0)
    , unk54(nullptr)
    , unk58(nullptr)
    , unk80(nullptr)
    , unk84(8)
    , unk85(0)
    , unk8C(0)
    , unk90(0)
    , unk94(0)
    , unk98(nullptr)
    , unk9C(1)
    , unkA0(1000)
    , unkA4(100)
    , unkA8(0)
{
}

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;

void TPollutionLayer::initJointModel(TJointModelManager* param_1,
                                     const char* param_2,
                                     MActorAnmData* param_3)
{
	TJointModel::initJointModel(param_1, param_2, param_3);
	const TPollutionLayerInfo* info
	    = &((TPollutionManager*)mManager)->unk6C[mIndexInParent];
	initLayerInfo(info);
	unk5C.init(this, info->unk8, info->unkC, info->unk28, info->unk20,
	           info->unk22);
	unk88 = info->unk24;

	unk58               = getTexResource(param_2);
	unk58->alphaEnabled = 2;

	unk54 = (u8*)unk58 + unk58->imageDataOffset;
	initTexImage(param_2);
	if ((int)unk30 == 4 && gParticleFlagLoaded[0x6F] == 0) {
		gpResourceManager->load("/scene/map/pollution/ms_thunder_s.jpa", 0x6F);
		gParticleFlagLoaded[0x6F] = 1;
	}

	if (mActor->checkAnmFileExist(param_2, 4))
		mActor->setBtk(param_2);

	if (mActor->checkAnmFileExist(param_2, 2))
		mActor->setBpk(param_2);

	if (mActor->checkAnmFileExist(param_2, 5))
		mActor->setBrk(param_2);

	for (int i = 0; i < mChildrenNum; ++i)
		((TPollutionObj*)mChildren[i])->initAreaInfo(this);
}

void TPollutionLayer::initPollutionTex(const char*) { }

void TPollutionLayer::initLayerInfo(const TPollutionLayerInfo* param_1)
{
	unk30 = param_1->unk0;
	unk32 = param_1->unk2;
	unk38 = param_1->unk10;
	unk40 = param_1->unk14;
	unk3C = param_1->unk18;
	unk44 = param_1->unk1C;
	unk48 = 2;

	if ((int)unk30 == 7) {
		unk50 = 200;
		unk85 = 0xA0;
	} else if ((int)unk30 == 1) {
		unk50 = 0x80;
		unk85 = 0x80;
	} else {
		unk50 = 30;
		unk85 = 50;
	}

	unk94 = 0x1E;
	unk98 = new JGeometry::TVec3<f32>[unk94];
	memset(unk98, 0, unk94 * sizeof(unk98[0]));
}

void TPollutionLayer::initTex(const char*) { }

void TPollutionLayer::initTexImage(const char* param_1)
{
	char fullPath[256];
	snprintf(fullPath, 256, "/scene/map/pollution/%s.bmp", param_1);
	unk80 = (u8*)JKRGetResource(fullPath);

	bool cVar1 = false;
	if (gpMarDirector->mMap == 9)
		cVar1 = true;

	for (int y = 0; y < unk5C.mHeight; ++y) {
		for (int x = 0; x < unk5C.mWidth; ++x) {
			int depth = unk5C.getDepth(x, y);
			if (cVar1
			    && ((x < 0 || unk5C.mWidth <= x || y < 0 || unk5C.mHeight <= y)
			        || depth == 0 || unk5C.isProhibit(x, y))) {
				int degree = unk5C.getEdgeDegree(x, y);
				if (degree == 0) {
					unk54[unk5C.index(x, y)]
					    = depth - degree * TPollutionManager::mEdgeAlpha;
				} else {
					unk54[unk5C.index(x, y)] = depth;
				}
			} else {
				unk54[unk5C.index(x, y)] = 0;
			}
		}
	}
	DCStoreRange(unk54, unk5C.mWidth * unk5C.mHeight);
}

void TPollutionLayer::perform(u32, JDrama::TGraphics*) { }

void TPollutionLayer::subtractFromYMap(f32, f32, f32) const { }

void TPollutionLayer::isPolluted(int, int, f32) const { }

bool TPollutionLayer::isPolluted(f32, f32, f32) const { }

void TPollutionLayer::isProhibit(f32, f32, f32) const { }

void TPollutionLayer::stamp(u16, f32, f32, f32, f32) { }

void TPollutionLayer::cleaned(f32, f32, f32, f32) { }

void TPollutionLayer::appearItem(f32, f32, f32) { }

void TPollutionLayer::stampModel(J3DModel*) { }

TPollutionLayerWallBase::TPollutionLayerWallBase() { }

void TPollutionLayerWallPlusX::initLayerInfo(const TPollutionLayerInfo*) { }

void TPollutionLayerWallPlusX::stamp(u16, f32, f32, f32, f32) { }

void TPollutionLayerWallPlusZ::initLayerInfo(const TPollutionLayerInfo*) { }

void TPollutionLayerWallPlusZ::stamp(u16, f32, f32, f32, f32) { }

void TPollutionLayerWave::initJointModel(TJointModelManager*, const char*,
                                         MActorAnmData*)
{
}

ResTIMG* TPollutionLayerWave::getTexResource(const char*) { }

void TPollutionLayerWave::perform(u32, JDrama::TGraphics*) { }

void TPollutionLayerWave::draw() const { }

void TPollutionLayerWave::initGX() const { }
