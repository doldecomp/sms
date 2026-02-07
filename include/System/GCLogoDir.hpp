#ifndef SYSTEM_GC_LOGO_DIR_HPP
#define SYSTEM_GC_LOGO_DIR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;

class TGCLogoDir : public JDrama::TDirector {
public:
	TGCLogoDir();

	virtual int direct();

	void setup(JDrama::TDisplay*, TMarioGamePad*);
	void direct_nlogo();
	void direct_dolby();
};

#endif
