#ifndef SYSTEM_MENU_DIR_HPP
#define SYSTEM_MENU_DIR_HPP

#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRDirector.hpp>

class TMarioGamePad;

class TMenuDirector : public JDrama::TDirector {
public:
	TMenuDirector();
	~TMenuDirector();
	void setupThreadFunc(void*);
	void setup(JDrama::TDisplay*, TMarioGamePad*);
	void rsetup();
	int direct();
	void setFixedStageValue();
};

#endif
