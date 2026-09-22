#ifndef JDR_FLAG_HPP
#define JDR_FLAG_HPP

namespace JDrama {

template <class T> class TFlagT {
public:
	TFlagT(T v = T()) { mValue = v; }

	// Retail calls this copy constructor (weak 0xc from MarDirectorDirect.o)
	// for the by-value parameter of operator= below; taking that parameter by
	// value is what produces the call. Defining this constructor out of class
	// instead breaks the DOL (JDRDStage, JDREfbCtrl, JDRViewConnecter).
	TFlagT(const TFlagT<T>& other)
	    : mValue(other.mValue)
	{
	}

	// fabricated
	TFlagT& operator=(TFlagT<T> other)
	{
		set(other.mValue);
		return *this;
	}

	void set(T v) { mValue = v; }
	T get() const { return mValue; }

	void setBit(T bit, bool on)
	{
		if (on)
			mValue |= bit;
		else
			mValue &= ~bit;
	}

	bool check(T bit) const { return (mValue & bit) != 0; }
	void on(T bit) { mValue |= bit; }
	void off(T bit) { mValue &= ~bit; }

public:
	T mValue;
};

}; // namespace JDrama

#endif
