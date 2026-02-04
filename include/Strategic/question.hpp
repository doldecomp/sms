#ifndef STRATEGIC_QUESTION_HPP
#define STRATEGIC_QUESTION_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class JUTTexture;
class TDLTexQuad;

class TQuestionRequest {
public:
	TQuestionRequest() { }

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
};

class TQuestionManager;

extern TQuestionManager* gpQuestionManager;

class TQuestionManager : public JDrama::TViewObj {
public:
	TQuestionManager() { }

	virtual ~TQuestionManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	bool request(JGeometry::TVec3<f32>, f32);
	void makeDL(JDrama::TGraphics*) const;
	void draw() const;

public:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ JUTTexture* unk18;
	/* 0x1C */ TQuestionRequest* unk1C;
	/* 0x20 */ TDLTexQuad* unk20;
};

#endif
