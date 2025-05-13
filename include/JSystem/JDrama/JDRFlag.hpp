#ifndef JDR_FLAG_HPP
#define JDR_FLAG_HPP

namespace JDrama {

template <class T> class TFlagT {
public:
	TFlagT(T v = T()) { set(v); }

	void set(T v) { mValue = v; }
	TFlagT(const TFlagT<T>& other)
	    : mValue(other.mValue)
	{
	}

public:
	T mValue;
};

}; // namespace JDrama

#endif
