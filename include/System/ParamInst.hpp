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

	// fabricated, but the reference return is settled (header round 24).
	// camerashake measured that an integer parameter read through get() does
	// not reserve the inline temporary an f32 one does, which reads as
	// "retail returned T by value for integer T". Tree-wide that is false:
	// by value for every T is total matched_code 53.73 -> 52.94 (36 units
	// lose, 4 gain), by value for the integer instantiations only (s8..u32,
	// reference kept for f32/TVec3/TFlagT) is 53.73 -> 53.24, and the
	// narrowest split there is any evidence for, s32 alone, is still
	// 53.73 -> 53.61. So the reference return is retail's, for every T, and
	// the few sites that want a by-value read want it at the *site* -- which
	// also matches the address-of uses (`&x.get()` in AnimalManager,
	// NpcManager, MtxUtil), all of them f32 or TVec3. There is no legal
	// C++98 spelling for a per-type return anyway: an explicit member
	// specialisation may not change the return type.
	const T& get() const { return value; }

	// Fabricated, but measured: the extra inline level around the assignment
	// is the 4 bytes of low pool TMario::checkWet's named block needs
	// (research 312), and `value = param` keeps retail's word copy where
	// `value.set(param)` would spell it as three float copies.
	void set(const T& param) { value = param; }

	T value;
};

template <typename T> class TParamRT : public TParamT<T> {
public:
	TParamRT(TParams* parent, T defaultValue, u16 keycode, const char* name)
	    : TParamT<T>(parent, defaultValue, keycode, name)
	{
	}

	// NOTE: this MUST take a reference, as constants passed
	// to it must be stored in sdata!
	inline void set(const T& param) { this->value = param; };

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
