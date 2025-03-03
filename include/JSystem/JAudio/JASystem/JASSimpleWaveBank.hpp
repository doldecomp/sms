#ifndef JASSIMPLEWAVEBANK_HPP
#define JASSIMPLEWAVEBANK_HPP

#include "types.h"

namespace JASystem {

class TWaveInfo;

class TSimpleWaveBank {
public:
	class TWaveHandle {
	public:
		TWaveHandle();
		virtual ~TWaveHandle();

		virtual const void* getWavePtr() const;
		virtual const TWaveInfo& getWaveInfo() const;
	};

	TSimpleWaveBank();
	virtual ~TSimpleWaveBank();

	virtual void* getHeap();
	virtual const char* getWaveArcFileName() const;
	virtual bool* getLoadFlagPtr();
	virtual int getType() const;

	const TWaveHandle* getWaveHandle(unsigned long) const;
	void setWaveTableSize(unsigned long);
	void setWaveInfo(unsigned long, const TWaveInfo&);
	void setWaveArcFileName(const char*);
};

} // namespace JASystem

#endif // JASSIMPLEWAVEBANK_HPP
