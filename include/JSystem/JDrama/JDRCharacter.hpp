#ifndef JDR_CHARACTER_HPP
#define JDR_CHARACTER_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>

class JKRArchive;

namespace JDrama {

class TCharacter : public TNameRef {
public:
	TCharacter()
	    : TNameRef("<TCharacter>")
	{
	}

	virtual ~TCharacter() { }

	virtual void* getRes(const char*) const = 0;
};

class TSmplChara : public TCharacter {
public:
	TSmplChara() { }
	virtual ~TSmplChara();
	virtual void load(JSUMemoryInputStream&);
	virtual void* getRes(const char*) const;

	void mountArc(const char*);

public:
	JKRArchive* mArchive;
};

} // namespace JDrama

#endif
