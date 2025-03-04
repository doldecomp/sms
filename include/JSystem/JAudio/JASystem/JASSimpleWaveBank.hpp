#ifndef JASSIMPLEWAVEBANK_HPP
#define JASSIMPLEWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TSimpleWaveBank : public TWaveBank, public WaveArcLoader::TObject {
public:
	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		TWaveHandle();
		virtual ~TWaveHandle();

		virtual const void* getWavePtr() const;
		virtual const TWaveInfo* getWaveInfo() const;
	};

	TSimpleWaveBank();

	virtual ~TSimpleWaveBank();
	virtual TWaveHandle* getWaveHandle(u32) const;
	virtual int getType() const;

	virtual bool* getLoadFlagPtr();
	virtual const char* getWaveArcFileName() const;
	virtual void* getHeap();

	void setWaveTableSize(u32);
	void setWaveInfo(u32, const TWaveInfo&);
	void setWaveArcFileName(const char*);
};

} // namespace JASystem

#endif // JASSIMPLEWAVEBANK_HPP
