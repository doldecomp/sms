#include "dolphin.h"

#include "types.h"
#include "JSystem/JGeometry.hpp"
#include "System/ParamInst.hpp"

template <typename T> void TParamT<T>::load(JSUMemoryInputStream& stream)
{
	u8 discard[16];

	stream.read(&discard[8], 4);
	stream.read(&this->value, sizeof(T));
};
// clang-format off
template class TParamT< JGeometry::TVec3<f32> >;
// clang-format on
template class TParamT<f32>;

template class TParamT<s32>;

template class TParamT<s16>;

template class TParamT<u8>;
