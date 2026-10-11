#ifndef PLAYER_ATOM_HPP
#define PLAYER_ATOM_HPP

#include <dolphin/types.h>
#include <JSystem/JGeometry.hpp>

namespace JDrama {
class TGraphics;
}

class TAtom {
public:
	TAtom();
};

class TRopeManager {
public:
	void create(u16, const JGeometry::TVec3<f32>&, f32, JGeometry::TVec3<f32>*);
	void movement(JGeometry::TVec3<f32>*, const JGeometry::TVec3<f32>&);
	void draw(JDrama::TGraphics*);
};

class TDeformedTerrain {
public:
	~TDeformedTerrain();
	void init();
	void movement();
	void draw(JDrama::TGraphics*);
	void perform(u32, JDrama::TGraphics*);
};

#endif
