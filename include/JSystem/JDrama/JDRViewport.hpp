#ifndef JDR_VIEWPORT_HPP
#define JDR_VIEWPORT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

namespace JDrama {

class TViewport : public TViewObj {
public:
	TViewport(const TRect&, const char*);

	virtual ~TViewport();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, TGraphics*);
};

}; // namespace JDrama

#endif
