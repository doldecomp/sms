#ifndef JADPRM_HPP
#define JADPRM_HPP

#include <dolphin/types.h>

template <class T> class JADPrm {
public:
	JADPrm(T val, const char*)
	    : value(val)
	{
	}

	T value;
};

template <class T> class JADPrmS {
public:
	JADPrmS& operator=(const JADPrmS& other)
	{
		unk0 = other.unk0;
		return *this;
	}

	T unk0;
};

#endif
