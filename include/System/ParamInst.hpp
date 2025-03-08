#ifndef PARAM_INST_HPP
#define PARAM_INST_HPP

#include "System/BaseParam.hpp"

template <typename T> class TParamT : public TBaseParam {
public:
	TParamT(TParams* params, unsigned short code, const char* paramName,
	        T defaultValue)
	    : TBaseParam(params, code, paramName)
	    , value(defaultValue)
	{
	}

	void load(JSUMemoryInputStream& stream);
	T get() const { return value; }

	T value;
};

template <typename T> class TParamRT : public TParamT<T> {
public:
	TParamRT(TParams* parent, u16 keycode, const char* name, T defaultValue)
	    : TParamT<T>(parent, keycode, name, defaultValue)
	{
	}

	inline void set(T param) {};
};

#endif
