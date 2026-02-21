#ifndef JDR_PLACEMENT_HPP
#define JDR_PLACEMENT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

namespace JDrama {

class TPlacement : public TViewObj {
public:
	TPlacement(const char* name)
	    : TViewObj(name)
	    , mPosition(0.0f, 0.0f, 0.0f)
	    , unk1C(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);

	// fabricated
	const JGeometry::TVec3<f32>& getPosition() const { return mPosition; }

public:
	/* 0x10 */ JGeometry::TVec3<f32> mPosition;
	/* 0x1C */ u16 unk1C;
};

}; // namespace JDrama

#endif
