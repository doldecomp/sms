#ifndef JGADGET_POINTER_HPP
#define JGADGET_POINTER_HPP

namespace JGadget {

template <class T> class TPointer {
public:
	TPointer(T* ptr)
	    : mPtr(ptr)
	{
	}
	~TPointer() { }

	void set(T* ptr) { mPtr = ptr; }

	T* mPtr;
};

template <class T> class TPointer_delete : public TPointer<T> {
public:
	TPointer_delete(T* ptr)
	    : TPointer<T>(ptr)
	{
	}
	~TPointer_delete() { delete this->mPtr; }
};

} // namespace JGadget

#endif
