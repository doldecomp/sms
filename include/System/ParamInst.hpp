#ifndef PARAM_INST_HPP
#define PARAM_INST_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <System/BaseParam.hpp>

#define PARAM_INIT(member, value)                                              \
	member(this, JDrama::TNameRef::calcKeyCode("#member"), "#member", value)

template <typename T> class TParamT : public TBaseParam {
public:
	TParamT(TParams* params, unsigned short code, const char* paramName,
	        T defaultValue)
	    : TBaseParam(params, code, paramName)
	    , value(defaultValue)
	{
	}

	virtual void load(JSUMemoryInputStream& stream);

	// fabricated
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
