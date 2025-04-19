#ifndef JDR_NAME_REF_GEN_HPP
#define JDR_NAME_REF_GEN_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>

class JSUMemoryInputStream;

namespace JDrama {

class TNameRefGen {
public:
	virtual TNameRef* load(JSUMemoryInputStream&);
	virtual TNameRef* getNameRef(const char*) const;

	TNameRef* getRootNameRef() { return mRootNameRef; }
	static TNameRefGen* getInstance() { return instance; }

	// fabricated
	template <class T> static T* search(const char* name)
	{
		return static_cast<T*>(
		    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(name));
	}

	static TNameRefGen* instance;

public:
	TNameRef* mRootNameRef;
};

} // namespace JDrama

#endif
