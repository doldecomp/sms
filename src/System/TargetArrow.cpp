#include <System/TargetArrow.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

#include <M3DUtil/InfectiousStrings.hpp> // TODO: removeme

TTargetArrow* gpTargetArrow;

const char* cTargetArrowBmdFileName
    = "/scene/map/targetArrow/targetArrow_red.bmd";

void TTargetArrow::loadAfter()
{
	void* res = JKRFileLoader::getGlbResource(cTargetArrowBmdFileName);
	if (!res)
		return;

	unk10 = SMS_MakeMActor("/scene/map/targetArrow", cTargetArrowBmdFileName, 3,
	                       0x10010000);
	unk10->setBck("target_arrow_spin");
}

void TTargetArrow::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk10 && unk14)
		unk10->perform(param_1, param_2);
}

void TTargetArrow::setPos(const JGeometry::TVec3<f32>& pos)
{
	if (!unk10)
		return;

	MtxPtr mat = unk10->getModel()->mBaseMtx;
	mat[0][3]  = pos.x;
	mat[1][3]  = pos.y;
	mat[2][3]  = pos.z;
}
