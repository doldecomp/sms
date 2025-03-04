#ifndef PARAM_INST_HPP
#define PARAM_INST_HPP

#include "System/BaseParam.hpp"

template <typename T> class TParamT : public TBaseParam {
public:
	void load(JSUMemoryInputStream& stream);
	T get() const { return value; }

	T value;
};

#endif
