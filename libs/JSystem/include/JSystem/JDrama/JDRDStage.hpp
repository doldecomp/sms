#ifndef JDR_DSTAGE_HPP
#define JDR_DSTAGE_HPP

#include <JSystem/JDrama/JDRViewConnecter.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRSize.hpp>

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

class TDStageTex : public TViewObj {
public:
	TDStageTex(const char*, JDrama::TFlagT<u16>);

	void getClearColor();
	void setClearColor(JUtility::TColor);
	void getClearZ();
	void setClearZ(u32);
	void getSrcRect() const;
	void setSrcRect(const JDrama::TRect&);
	void getImgPtr() const;
	void setImgPtr(void*);
	void getTexFmt() const;
	void setTexFmt(GXTexFmt);
	void getDstSize() const;
	void setDstSize(const JDrama::TSize&);
	void setTexAttb(const GXTexObj&);
};

} // namespace JDrama

#endif
