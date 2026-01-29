#ifndef PARAM_INST_HPP
#define PARAM_INST_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <System/BaseParam.hpp>

#define PARAM_INIT(member, ...)                                                \
	member(this, JDrama::TNameRef::calcKeyCode(#member), #member, __VA_ARGS__)

template <typename T> class TParamT : public TBaseParam {
public:
	TParamT(TParams* params, u16 code, const char* paramName, T defaultValue)
	    : TBaseParam(params, code, paramName)
	    , value(defaultValue)
	{
	}

	virtual void load(JSUMemoryInputStream& stream);

	// fabricated
	const T& get() const { return value; }

	T value;
};

template <typename T> class TParamRT : public TParamT<T> {
public:
	TParamRT(TParams* parent, u16 keycode, const char* name, T defaultValue)
	    : TParamT<T>(parent, keycode, name, defaultValue)
	{
	}

	inline void set(T param) { value = param; };

	// Fabricated
	TParamRT<T>& operator=(const TParamRT<T>& other)
	{
		this->keyCode = other.keyCode;
		this->name    = other.name;
		this->next    = other.next;
		this->value   = other.value;
		return *this;
	}
};

class TParamVec : public TParamT<JGeometry::TVec3<f32> > {
public:
	// fabricated AND wrong
	TParamVec(TParams* parent, u16 keycode, const char* name,
	          JGeometry::TVec3<f32> defaultValue)
	    : TParamT<JGeometry::TVec3<f32> >(parent, keycode, name, defaultValue)
	{
	}
};

#endif
