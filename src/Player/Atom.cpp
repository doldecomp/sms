#include <Player/Atom.hpp>

#include <Map/MapCollisionEntry.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

// NOTE: file with debug stuff?

static JGeometry::TVec3<f32> cDeformedTerrainCenter(0.0f, 5000.0f, 0.0f);

void dummy(TMapCollisionBase* col) { col->setUp(); }

TAtom::TAtom() { }

void TRopeManager::create(u16, const JGeometry::TVec3<f32>&, f32,
                          JGeometry::TVec3<f32>*)
{
}

void TRopeManager::movement(JGeometry::TVec3<f32>*,
                            const JGeometry::TVec3<f32>&)
{
}

void TRopeManager::draw(JDrama::TGraphics*) { }

void TDeformedTerrain::init() { }

void TDeformedTerrain::movement() { }

void TDeformedTerrain::draw(JDrama::TGraphics*) { }

void TDeformedTerrain::perform(u32, JDrama::TGraphics*) { }

TDeformedTerrain::~TDeformedTerrain() { }
