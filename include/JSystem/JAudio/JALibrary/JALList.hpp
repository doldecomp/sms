#ifndef JALLIST_HPP
#define JALLIST_HPP

#include <JSystem/JSupport/JSUList.hpp>

template <class T> class JALList : public JSULink<T> {
public:
	JALList(T* param_1, bool param_2)
	    : JSULink<T>(param_1)
	{
		if (param_2 == true)
			smList.append(this);
	}
	~JALList() { smList.remove(this); }

	static JSUList<T> smList;
};

template <class T> JSUList<T> JALList<T>::smList;

template <class T, class U> class JALListS : public JALList<T> {
public:
	JALListS(U, T* param_2)
	    : JALList<T>(param_2, true)
	{
	}
	~JALListS() { }

	static T* search(U param_1);
};

template <class T, class U> T* JALListS<T, U>::search(U param_1)
{
	JSUListIterator<T> it = JALList<T>::smList.getFirst();
	for (; it != JALList<T>::smList.getEnd(); ++it) {
		if (param_1 == (u32)it.getObject()->unk10)
			return it.getObject();
	}
	return nullptr;
}

template <class T, class U> class JALListHioNode : public JALListS<T, U> {
public:
	JALListHioNode(const char* param_1, U param_2, T* param_3);
	~JALListHioNode() { }

public:
	/* 0x10 */ U unk10;
};

template <class T, class U>
JALListHioNode<T, U>::JALListHioNode(const char* param_1, U param_2, T* param_3)
    : JALListS<T, U>(param_2, param_3)
    , unk10(param_2)
{
}

template <class T, class U>
class JALListVirtualNode : public JALListS<T, U> {
public:
	JALListVirtualNode(const char* param_1, U param_2, T* param_3)
	    : JALListS<T, U>(param_2, param_3)
	    , unk10(param_2)
	{
	}
	~JALListVirtualNode() { }

public:
	/* 0x10 */ U unk10;
};

template <class T> class JALListFrameLoop {
public:
	virtual void frameLoopDyna() { }
};

#endif
