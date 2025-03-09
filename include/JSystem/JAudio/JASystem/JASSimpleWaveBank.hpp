#ifndef JASSIMPLEWAVEBANK_HPP
#define JASSIMPLEWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <dolphin/types.h>
#include <types.h>

namespace JASystem {

class TSimpleWaveBank : public TWaveBank, public WaveArcLoader::TObject {
public:
	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		TWaveHandle()
		    : mHeap(nullptr)
		{
		}
		virtual ~TWaveHandle() { }

		virtual const TWaveInfo* getWaveInfo() const { return &mWaveInfo; }
		virtual const void* getWavePtr() const
		{
			if (mHeap->unk8 == 0)
				return nullptr;
			return ((u8*)mHeap->unk8) + mWaveInfo.unk8;
		}

	public:
		/* 0x04 */ TWaveInfo mWaveInfo;
		/* 0x30 */ Kernel::THeap* mHeap;
	};

	TSimpleWaveBank();

	virtual ~TSimpleWaveBank();
	virtual TWaveHandle* getWaveHandle(u32) const;
	virtual int getType() const { return 'SMPL'; }

	virtual u32* getLoadFlagPtr() { return &unk40; }
	virtual const char* getWaveArcFileName() const { return unk10; }
	virtual Kernel::THeap* getHeap() { return &unk14; }

	void setWaveTableSize(u32);
	void setWaveInfo(u32, const TWaveInfo&);
	void setWaveArcFileName(const char*);

public:
	/* 0x8 */ TWaveHandle* unk8;
	/* 0xC */ u32 unkC;
	/* 0x10*/ char* unk10;
	/* 0x14 */ Kernel::THeap unk14;
	/* 0x40 */ u32 unk40;
};

} // namespace JASystem

#endif // JASSIMPLEWAVEBANK_HPP
