#ifndef JDR_DSTAGE_HPP
#define JDR_DSTAGE_HPP

#include <JSystem/JDrama/JDRViewConnecter.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>

namespace JDrama {

class TEfbCtrlDisp;

class TDStageDisp : public TViewConnecter {
public:
	TDStageDisp(const char* name = "<DStageDisp>", TFlagT<u16> = 0);

	const TEfbCtrlDisp* getEfbCtrlDisp() const;
	TEfbCtrlDisp* getEfbCtrlDisp();

	TViewObjPtrListT<TViewObj>* getUnk14()
	{
		return (TViewObjPtrListT<TViewObj>*)unk14;
	}
};

} // namespace JDrama

#endif
