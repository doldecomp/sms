#ifndef JALLIST_HPP
#define JALLIST_HPP

#include <JSystem/JSupport/JSUList.hpp>

template <class T> class JALList : public JSULink<T> {
public:
	JALList(T* param_1, bool param_2)
	    : JSULink<T>(param_1)
	{
		if (param_2)
			smList.append(this);
	}
	~JALList();

	static JSUList<T> smList;
};

template <class T> JSUList<T> JALList<T>::smList;

template <class T, class U> class JALListHioNode : public JALList<T> {
public:
	JALListHioNode(const char* param_1, U param_2, T* param_3)
	    : JALList<T>(param_3, true)
	    , unk10(param_2)
	{
	}
	~JALListHioNode() { }

public:
	/* 0x10 */ U unk10;
};

template <class T> class JALListFrameLoop : public JALListHioNode<T, u32> {
public:
	JALListFrameLoop(const char* param_1, u32 param_2, T* param_3)
	    : JALListHioNode<T, u32>(param_1, param_2, param_3)
	{
	}

	virtual void frameLoopDyna() { }
};

#endif
