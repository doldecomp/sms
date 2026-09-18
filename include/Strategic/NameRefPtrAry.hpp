#ifndef STRATEGIC_NAME_REF_PTR_ARY_HPP
#define STRATEGIC_NAME_REF_PTR_ARY_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-vector.hpp>

template <class T, class U = JDrama::TNameRef>
class TNameRefPtrAryT : public U, public JGadget::TVector_pointer<T*> {
public:
	TNameRefPtrAryT(const char* name = "<NameRefPtrAryT>")
	    : U(name)
	{
	}

	virtual ~TNameRefPtrAryT() { }

	// fabricated
	JGadget::TVector_pointer<T*>& getChildren() { return *this; }

	// Bodies live in NameRefPtrAry.tpp; see the note there.
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual JDrama::TNameRef* searchF(u16 key, char const* name);
};

#endif
