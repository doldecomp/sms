#include <dolphin.h>

#include "types.h"
#include "System/BaseParam.hpp"
#include "JSystem/JSUInputStream.hpp"


template<typename T> class TParamT: public TBaseParam {
public:
	void load(JSUMemoryInputStream&);
	u32 _00C;
	T value;
};
