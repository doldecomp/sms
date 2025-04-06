#ifndef STRATEGIC_QUESTION_HPP
#define STRATEGIC_QUESTION_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TQuestionManager;

extern TQuestionManager* gpQuestionManager;

class TQuestionManager : JDrama::TViewObj {
public:
	TQuestionManager();

	virtual ~TQuestionManager();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, JDrama::TGraphics*);

	void request(JGeometry::TVec3<f32>, f32);
	void makeDL(JDrama::TGraphics*) const;
	void draw() const;
};

#endif
