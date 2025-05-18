#include <Map/MapWire.hpp>

TMapWirePoint::TMapWirePoint() { }

void TMapWire::drawLower() const { }

void TMapWire::drawUpper() const { }

void TMapWire::getPointPowerAtReleased(f32) const { }

void TMapWire::getPointPosAtReleased(f32, JGeometry::TVec3<f32>*) const { }

void TMapWire::updatePointAtReleased(int) { }

void TMapWire::updateMovePointAtReleased() { }

void TMapWire::initPointAtJustReleased(f32, TMapWirePoint*) { }

void TMapWire::release() { }

void TMapWire::getPointPosAtHanged(f32, JGeometry::TVec3<f32>*) const { }

void TMapWire::getPointInfoAtHanged(f32, TMapWirePoint*) { }

void TMapWire::setFootPointsAtHanged(MtxPtr) { }

void TMapWire::calcViewAndDBEntry() { }

void TMapWire::move() { }

void TMapWire::getPosInWire(const JGeometry::TVec3<f32>&) const { }

void TMapWire::getPointPosOnLine(f32, JGeometry::TVec3<f32>*) const { }

void TMapWire::getPointPosOnWire(f32, JGeometry::TVec3<f32>*) const { }

void TMapWire::getPointPosDefault(f32, JGeometry::TVec3<f32>*) const { }

void TMapWire::initTipPoints(const TCubeGeneralInfo*) { }

void TMapWire::init(const TCubeGeneralInfo*) { }

TMapWire::TMapWire() { }
