#ifndef SYSTEM_SELECT_DIR_HPP
#define SYSTEM_SELECT_DIR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;

class TSelectDir : public JDrama::TDirector {
public:
	TSelectDir();

	virtual int direct();

	void setup(JDrama::TDisplay*, TMarioGamePad*, unsigned char);
	void setupThreadFunc(void*);
	void rsetup();
	void changeOrder();
};

#endif
