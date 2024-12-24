#ifndef JDR_NAME_REF_HPP
#define JDR_NAME_REF_HPP

#include <types.h>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

namespace JDrama {
class TNameRef {
public:
	static void calcKeyCode(char const*);
	static void getType(JSUMemoryInputStream&, JSUMemoryInputStream&);
	static void genObject(JSUMemoryInputStream&, JSUMemoryInputStream&);

	virtual ~TNameRef();
	virtual void getType() const;
	virtual void load(JSUMemoryInputStream&);
	virtual void save(JSUMemoryOutputStream&);
	virtual void loadAfter();
	virtual void searchF(u16, char const*);

private:
	char* mName;
	u16 mKeyCode;
};
} // namespace JDrama

#endif
