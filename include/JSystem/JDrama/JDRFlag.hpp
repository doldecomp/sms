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

	// fabricated
	TFlagT& operator=(const TFlagT<T>& other) { set(other.get()); }

	void set(T v) { mValue = v; }
	T get() const { return mValue; }

public:
	T mValue;
};

}; // namespace JDrama

#endif
