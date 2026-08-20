#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

using namespace JDrama;

TDStageDisp::TDStageDisp(const char* name, TFlagT<u16> flag)
    : TViewConnecter(nullptr, nullptr, 0x1, name)
{
	unk10 = new TEfbCtrlDisp("<EfbCtrlDisp>", flag);
	unk14 = new TViewObjPtrListT<TViewObj>;
}

const TEfbCtrlDisp* TDStageDisp::getEfbCtrlDisp() const
{
	return (TEfbCtrlDisp*)unk10;
}

TEfbCtrlDisp* TDStageDisp::getEfbCtrlDisp() { return (TEfbCtrlDisp*)unk10; }

TDStageTex::TDStageTex(const char*, JDrama::TFlagT<u16>) { }

void TDStageTex::getClearColor() { }

void TDStageTex::setClearColor(JUtility::TColor) { }

void TDStageTex::getClearZ() { }

void TDStageTex::setClearZ(u32) { }

void TDStageTex::getSrcRect() const { }

void TDStageTex::setSrcRect(const JDrama::TRect&) { }

void TDStageTex::getImgPtr() const { }

void TDStageTex::setImgPtr(void*) { }

void TDStageTex::getTexFmt() const { }

void TDStageTex::setTexFmt(GXTexFmt) { }

void TDStageTex::getDstSize() const { }

void TDStageTex::setDstSize(const JDrama::TSize&) { }

void TDStageTex::setTexAttb(const GXTexObj&) { }
