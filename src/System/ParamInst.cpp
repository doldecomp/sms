#include <System/ParamInst.hpp>
#include <JSystem/JGeometry.hpp>

template <typename T> void TParamT<T>::load(JSUMemoryInputStream& stream)
{
	u8 discard[16];

	stream.read(&discard[8], 4);
	stream.read(&value, sizeof(T));
};

template class TParamT<u8>;
template class TParamT<s16>;
template class TParamT<s32>;
template class TParamT<f32>;
template class TParamT<JGeometry::TVec3<f32> >;
