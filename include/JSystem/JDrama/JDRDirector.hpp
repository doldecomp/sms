#ifndef JDR_DIRECTOR_HPP
#define JDR_DIRECTOR_HPP

#include <JSystem/JStage/JSGSystem.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>

namespace JDrama {

class TViewObj;

class TDirector : public TNameRef, public JStage::TSystem {
public:
	TDirector()
	    : TNameRef("<TDirector>")
	    , unk10(nullptr)
	    , unk14(nullptr)
	{
	}

	virtual ~TDirector() { }
	virtual TNameRef* searchF(u16, const char*);

	void direct();
	JStage::TObject* JSGFindObject(const char*, JStage::TEObject) const;

public:
	/* 0x10 */ TViewObj* unk10;
	/* 0x14 */ TViewObj* unk14;
};

} // namespace JDrama

#endif
