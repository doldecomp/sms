#include <JSystem/JGeometry.hpp>

// NOTE: hack for matching setLength
static void dummy(JGeometry::TVec3<f32>& v) { v.setLength(v, 1.0f); }
static void dummy2(JGeometry::TVec3<f32>& v) { dummy(v); }
static void dummy3(JGeometry::TVec3<f32>& v) { dummy2(v); }
static void dummy4(JGeometry::TVec3<f32>& v) { dummy3(v); }
