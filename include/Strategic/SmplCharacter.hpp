#ifndef STRATEGIC_SMPL_CHARACTER_HPP
#define STRATEGIC_SMPL_CHARACTER_HPP

#include <JSystem/JDrama/JDRCharacter.hpp>

class TSMSSmplChara : public JDrama::TCharacter {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void* getRes(const char*) const;

	~TSMSSmplChara() { delete[] mPath; }

public:
	/* 0xC */ char* mPath;
};

#endif
