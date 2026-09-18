#ifndef STRATEGIC_NAME_REF_ARY_HPP
#define STRATEGIC_NAME_REF_ARY_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-vector.hpp>

template <class T, class U = JDrama::TNameRef>
class TNameRefAryT : public U, public JGadget::TVector<T> {
public:
	TNameRefAryT(const char* name = "<NameRefAryT>")
	    : U(name)
	{
	}

	// fabricated
	JGadget::TVector<T>& getChildren() { return *this; }

	// Bodies live in NameRefAry.tpp; see the note there.
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual JDrama::TNameRef* searchF(u16 key, const char* name);
};

#endif
