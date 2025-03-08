#ifndef JADPRM_HPP
#define JADPRM_HPP

#include <dolphin/types.h>

template <class T> class JADPrm {
public:
	JADPrm();
	JADPrm(T val, const char*);

	T unk0;
};

template <class T>
JADPrm<T>::JADPrm(T val, const char*)
    : unk0(val)
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
