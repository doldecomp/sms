#ifndef STRATEGIC_QUESTION_HPP
#define STRATEGIC_QUESTION_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class JUTTexture;
class TDLTexQuad;

struct TQuestionRequest {
	/* 0x0 */ JGeometry::TVec3<f32> mPosition;
	/* 0xC */ f32 mRadius;
};

class TQuestionManager;

extern TQuestionManager* gpQuestionManager;

class TQuestionManager : public JDrama::TViewObj {
public:
	TQuestionManager() { }

	virtual ~TQuestionManager() { }
	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	bool request(JGeometry::TVec3<f32>, f32);
	void makeDL(JDrama::TGraphics*) const;
	void draw() const;

private:
	enum {
		FLAG_UNK2 = 0x2,
	};
	/* 0x10 */ u16 mFlags;
	/* 0x12 */ u16 mRequestNum;
	/* 0x14 */ f32 mRequestClipFar;
	/* 0x18 */ JUTTexture* unk18;
	/* 0x1C */ TQuestionRequest* mRequests;
	/* 0x20 */ TDLTexQuad* mDisplayList;
};

#endif
