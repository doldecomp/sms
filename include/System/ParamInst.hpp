#include "dolphin.h"

#include "types.h"
#include "System/BaseParam.hpp"
#include "JSystem/JSUMemoryStream.hpp"

template<typename T> class TParamT: public TBaseParam {
public:
	void load(JSUMemoryInputStream&);
	u32 _00C;
	T value;
};
