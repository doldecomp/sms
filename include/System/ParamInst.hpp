#include "dolphin.h"

#include "types.h"
#include "System/BaseParam.hpp"

template<typename T> class TParamT: public TBaseParam {
public:
	void load(JSUMemoryInputStream& stream);

	T value;
};
