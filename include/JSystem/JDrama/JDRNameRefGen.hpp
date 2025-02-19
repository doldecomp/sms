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

	static TNameRefGen* instance;

public:
	TNameRef* mRootNameRef;
};

} // namespace JDrama

#endif
