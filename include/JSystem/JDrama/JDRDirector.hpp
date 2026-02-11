#ifndef JDR_DIRECTOR_HPP
#define JDR_DIRECTOR_HPP

#include <JSystem/JStage/JSGSystem.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRDStageGroup.hpp>

namespace JDrama {

class TViewObj;

class TDirector : public TNameRef, public JStage::TSystem {
public:
	TDirector(const char* name = "<TDirector>")
	    : TNameRef(name)
	    , unk10(nullptr)
	    , unk14(nullptr)
	{
	}

	virtual TNameRef* searchF(u16, const char*);

	virtual int direct();
	virtual JStage::TObject* JSGFindObject(const char*, JStage::TEObject) const;

public:
	/* 0x10 */ TViewObj* unk10;
	/* 0x14 */ TDStageGroup* unk14;
};

} // namespace JDrama

#endif
