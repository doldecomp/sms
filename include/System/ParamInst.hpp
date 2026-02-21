#ifndef PARAM_INST_HPP
#define PARAM_INST_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <System/BaseParam.hpp>

#define PARAM_INIT(member, defaultValue)                                       \
	member(this, defaultValue, JDrama::TNameRef::calcKeyCode(#member), #member)

template <typename T> class TParamT : public TBaseParam {
public:
	TParamT(TParams* params, T defaultValue, u16 code, const char* paramName)
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
	TParamRT(TParams* parent, T defaultValue, u16 keycode, const char* name)
	    : TParamT<T>(parent, defaultValue, keycode, name)
	{
	}

	inline void set(T param) { this->value = param; };

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
	TParamVec(TParams* parent, JGeometry::TVec3<f32> defaultValue, u16 keycode,
	          const char* name)
	    : TParamT<JGeometry::TVec3<f32> >(parent, defaultValue, keycode, name)
	{
	}
};

#endif
