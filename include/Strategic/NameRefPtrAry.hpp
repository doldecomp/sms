#ifndef STRATEGIC_NAME_REF_PTR_ARY_HPP
#define STRATEGIC_NAME_REF_PTR_ARY_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGadget/std-vector.hpp>

template <class T, class U = JDrama::TNameRef>
class TNameRefPtrAryT : public U, public JGadget::TVector_pointer<T> {
public:
	TNameRefPtrAryT(const char* name = "<NameRefPtrAryT>")
	    : U(name)
	{
	}

	virtual ~TNameRefPtrAryT() { }

	// fabricated
	JGadget::TVector_pointer<T>& getChildren() { return *this; }

	virtual void load(JSUMemoryInputStream& stream)
	{
		U::load(stream);
		u32 local_44 = stream.readU32();
		getChildren().reserve(local_44);
		for (int i = 0; i < local_44; ++i) {
			JSUMemoryInputStream stream2;
			JDrama::TNameRef* ref
			    = JDrama::TNameRef::genObject(stream, stream2);
			if (ref) {
				getChildren().push_back((T*)ref);
				ref->load(stream2);
			}
		}
	}

	virtual void loadAfter()
	{
		U::loadAfter();

		for (T** it = getChildren().begin(); it != getChildren().end(); ++it)
			(*it)->loadAfter();
	}

	virtual JDrama::TNameRef* searchF(u16 key, char const* name)
	{
		if (JDrama::TNameRef* ref = JDrama::TNameRef::searchF(key, name))
			return ref;

		for (T** it = getChildren().begin(); it != getChildren().end(); ++it)
			if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
				return ref;

		return nullptr;
	}
};

#endif
