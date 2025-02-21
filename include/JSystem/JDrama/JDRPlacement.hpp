#ifndef JDR_PLACEMENT_HPP
#define JDR_PLACEMENT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

namespace JDrama {

class TPlacement : public TViewObj {
public:
	TPlacement(const char* name)
	    : TViewObj(name)
	    , mPosition(0, 0, 0)
	    , unk1C(0)
	{
	}
	virtual ~TPlacement() {};
	virtual void load(JSUMemoryInputStream&);

public:
	/* 0x10 */ JGeometry::TVec3<f32> mPosition;
	/* 0x1C */ u16 unk1C;
};

}; // namespace JDrama

#endif
