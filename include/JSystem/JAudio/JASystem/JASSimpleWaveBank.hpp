#ifndef JASSIMPLEWAVEBANK_HPP
#define JASSIMPLEWAVEBANK_HPP

#include <dolphin/types.h>

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

	const TWaveHandle* getWaveHandle(u32) const;
	void setWaveTableSize(u32);
	void setWaveInfo(u32, const TWaveInfo&);
	void setWaveArcFileName(const char*);
};

} // namespace JASystem

#endif // JASSIMPLEWAVEBANK_HPP
