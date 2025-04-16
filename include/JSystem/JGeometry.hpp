#ifndef J_GEOMETRY_HPP
#define J_GEOMETRY_HPP

#include <JSystem/JGeometry/JGBox.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>
#include <JSystem/JGeometry/JGMatrix44.hpp>
#include <JSystem/JGeometry/JGPosition3.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

typedef JGeometry::SMatrix34C<f32> TSMtx34f;
typedef JGeometry::SMatrix44C<f32> TSMtx44f;
typedef JGeometry::TMatrix34<TSMtx34f> TMtx34f;
typedef JGeometry::TMatrix44<TSMtx44f> TMtx44f;

typedef JGeometry::TRotation3<TMtx34f> TRotation3f;
typedef JGeometry::TPosition3<TMtx34f> TPosition3f;

#endif
