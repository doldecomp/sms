#ifndef JALLIST_HPP
#define JALLIST_HPP

#include <JSystem/JSupport/JSUList.hpp>

template <class T> class JALList : public JSULink<T> {
public:
	JALList(T* owner, bool addToList)
	    : JSULink<T>(owner)
	{
		if (addToList == true)
			smList.append(this);
	}
	~JALList() { smList.remove(this); }

	static JSUList<T> smList;
};

template <class T> JSUList<T> JALList<T>::smList;

template <class T, class U> class JALListS : public JALList<T> {
public:
	JALListS(U, T* owner)
	    : JALList<T>(owner, true)
	{
	}
	~JALListS() { }

	static T* search(U key);
};

template <class T, class U> T* JALListS<T, U>::search(U key)
{
	JSUListIterator<T> it = JALList<T>::smList.getFirst();
	for (; it != JALList<T>::smList.getEnd(); ++it) {
		if (key == (u32)it.getObject()->mKey)
			return it.getObject();
	}
	return nullptr;
}

template <class T, class U> class JALListHioNode : public JALListS<T, U> {
public:
	JALListHioNode(const char* name, U key, T* owner);
	~JALListHioNode() { }

public:
	/* 0x10 */ U mKey;
};

template <class T, class U>
JALListHioNode<T, U>::JALListHioNode(const char* name, U key, T* owner)
    : JALListS<T, U>(key, owner)
    , mKey(key)
{
}

template <class T, class U> class JALListVirtualNode : public JALListS<T, U> {
public:
	JALListVirtualNode(const char* name, U key, T* owner)
	    : JALListS<T, U>(key, owner)
	    , mKey(key)
	{
	}
	~JALListVirtualNode() { }

public:
	/* 0x10 */ U mKey;
};

template <class T> class JALListFrameLoop {
public:
	virtual void frameLoopDyna() { }
};

#endif
