#include <JSystem/JDrama/JDRDStage.hpp>
#include <JSystem/JDrama/JDREfbCtrl.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

using namespace JDrama;

TDStageDisp::TDStageDisp(const char* name, TFlagT<u16> flag)
    : TViewConnecter(nullptr, nullptr, flag, name)
{
	unk10 = new TEfbCtrlDisp("<EfbCtrlDisp>", flag);
	unk14 = new TViewObjPtrListT<TViewObj>;
}

const TEfbCtrlDisp* TDStageDisp::getEfbCtrlDisp() const
{
	return (TEfbCtrlDisp*)unk10;
}

TEfbCtrlDisp* TDStageDisp::getEfbCtrlDisp() { return (TEfbCtrlDisp*)unk10; }
