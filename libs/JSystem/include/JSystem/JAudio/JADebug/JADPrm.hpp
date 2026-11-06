#ifndef JADPRM_HPP
#define JADPRM_HPP

#include <dolphin/types.h>

template <class T> class JADPrm {
public:
	JADPrm();
	JADPrm(T val, const char* name);

	T get() const { return data; }

	T data;
};

template <class T>
JADPrm<T>::JADPrm(T val, const char* name)
    : data(val)
{
}

template <class T> class JADPrmS : public JADPrm<T> {
public:
	JADPrmS();

	JADPrmS(T val, const char* name)
	    : JADPrm<T>(val, name)
	{
	}
};

#endif
