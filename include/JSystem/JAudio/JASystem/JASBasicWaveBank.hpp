#ifndef JASBASICWAVEBANK_HPP
#define JASBASICWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TBasicWaveBank : public TWaveBank {
public:
	TBasicWaveBank();

	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		TWaveHandle()
		    : unk30(nullptr)
		{
		}

		virtual ~TWaveHandle() { }
		virtual const TWaveInfo* getWaveInfo() const { return &unk4; }
		virtual const void* getWavePtr() const
		{
			if (unk30->unk8 == 0)
				return nullptr;
			return ((u8*)unk30->unk8) + unk4.unk8;
		}

	public:
		/* 0x4 */ JASystem::TWaveInfo unk4;
		/* 0x30 */ Kernel::THeap* unk30;
	};

	virtual ~TBasicWaveBank();
	virtual TWaveHandle* getWaveHandle(u32) const;
	virtual int getType() const { return 'BSIC'; }

	void setGroupCount(u32);
	void setWaveTableSize(u32);

	class TWaveInfo {
	public:
		TWaveInfo()
		    : unk34(0)
		    , mPrev(nullptr)
		    , mNext(nullptr)
		{
		}
		~TWaveInfo() { }

	public:
		/* 0x0 */ TWaveHandle mWaveHandle;
		/* 0x34 */ u32 unk34;
		/* 0x38 */ TWaveInfo* mPrev;
		/* 0x3C */ TWaveInfo* mNext;
	};

	class TWaveGroup : public WaveArcLoader::TObject {
	public:
		TWaveGroup();
		~TWaveGroup();

		virtual u32* getLoadFlagPtr() { return &unk30; }
		virtual const char* getWaveArcFileName() const { return unk3C; }
		virtual Kernel::THeap* getHeap() { return &unk4; }

		void setWaveCount(u32);
		void setWaveInfo(int, u32, const JASystem::TWaveInfo&);
		void setWaveArcFileName(const char*);

	public:
		/* 0x4 */ Kernel::THeap unk4;
		/* 0x30 */ u32 unk30;
		/* 0x34 */ TWaveInfo* unk34;
		/* 0x38 */ u32 unk38;
		/* 0x3C */ char* unk3C;
	};

	TWaveGroup* getWaveGroup(int);
	void incWaveTable(const TWaveGroup*);
	void decWaveTable(const TWaveGroup*);

private:
	/* 0x4 */ TWaveInfo** unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ TWaveGroup* unkC;
	/* 0x10 */ u32 unk10;
};

} // namespace JASystem

#endif // JASBASICWAVEBANK_HPP
