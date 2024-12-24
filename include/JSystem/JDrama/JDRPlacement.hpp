#ifndef JDR_PLACEMENT_HPP
#define JDR_PLACEMENT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

namespace JDrama {
class TPlacement : public TViewObj {
public:
	virtual ~TPlacement() {};
	virtual void load(JSUMemoryInputStream&);

	virtual void _vt01C() = 0;
	virtual void _vt020() = 0;

	JGeometry::TVec3<f32> position;
	u32 _01C;
};
}; // namespace JDrama

#endif
