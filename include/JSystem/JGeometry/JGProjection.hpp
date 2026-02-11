#ifndef JG_PROJECTION_HPP
#define JG_PROJECTION_HPP

#include <JSystem/JGeometry/JGPosition3.hpp>

namespace JGeometry {

// This class is from SMG. Inheritance is a wild guess on my part,
// but it makes stuff match and kind of makes sense conceptually?
// A projective transform (TProjection) IS an affine transform (TPosition)
// IS alinear transform (TRotation).
template <class T> class TProjection3 : public TPosition3<T> {
public:
	TProjection3() { }
};

} // namespace JGeometry

#endif
