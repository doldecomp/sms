#ifndef JDR_FLAG_HPP
#define JDR_FLAG_HPP

namespace JDrama {

template <class T> class TFlagT {
public:
	TFlagT(T v = T()) { set(v); }

	TFlagT(const TFlagT<T>& other)
	    : mValue(other.mValue)
	{
	}

	void set(T v) { mValue = v; }

public:
	T mValue;
};

}; // namespace JDrama

#endif
