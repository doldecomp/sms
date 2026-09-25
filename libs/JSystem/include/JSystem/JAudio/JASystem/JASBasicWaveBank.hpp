#ifndef JASBASICWAVEBANK_HPP
#define JASBASICWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASWaveArcLoader.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>

namespace JASystem {

/**
 * @brief Layered wave bank. Contains multiple groups which can be activated
 * independently and fill in wave ID slots in the wave table. Conflicts are
 * resolved by the most recently activated group overriding the previous wave
 * corresponding to the same ID.
 */
class TBasicWaveBank : public TWaveBank {
public:
	TBasicWaveBank();

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
			if (mHeap->getBase() == 0)
				return nullptr;
			return ((u8*)mHeap->getBase()) + mWaveInfo.mOffset;
		}

	public:
		/* 0x4 */ JASystem::TWaveInfo mWaveInfo;
		/* 0x30 */ Kernel::THeap* mHeap;
	};

	virtual ~TBasicWaveBank();
	virtual TWaveHandle* getWaveHandle(u32) const;
	virtual int getType() const { return 'BSIC'; }

	void setGroupCount(u32);
	void setWaveTableSize(u32);

	class TWaveInfo {
	public:
		TWaveInfo()
		    : mWaveID(0)
		    , mPrev(nullptr)
		    , mNext(nullptr)
		{
		}
		~TWaveInfo() { }

	public:
		/* 0x0 */ TWaveHandle mWaveHandle;
		/* 0x34 */ u32 mWaveID;
		/* 0x38 */ TWaveInfo* mPrev;
		/* 0x3C */ TWaveInfo* mNext;
	};

	class TWaveGroup : public WaveArcLoader::TObject {
	public:
		TWaveGroup();
		~TWaveGroup();

		virtual u32* getLoadFlagPtr() { return &mLoadFlag; }
		virtual const char* getWaveArcFileName() const
		{
			return mWaveArcFileName;
		}
		virtual Kernel::THeap* getHeap() { return &mHeap; }

		void setWaveCount(u32);
		u32 getWaveCount() const { return mWaveCount; }
		void setWaveInfo(int, u32, const JASystem::TWaveInfo&);
		void setWaveArcFileName(const char*);

	public:
		/* 0x4 */ Kernel::THeap mHeap;
		/* 0x30 */ u32 mLoadFlag;
		/* 0x34 */ TWaveInfo* mWaves;
		/* 0x38 */ u32 mWaveCount;
		/* 0x3C */ char* mWaveArcFileName;
	};

	TWaveGroup* getWaveGroup(int index);

	/**
	 * @brief Activates the wave group, overriding current IDs with it's waves.
	 *
	 * @param group The wave group to activate.
	 */
	void incWaveTable(const TWaveGroup* group);

	/**
	 * @brief Deactivates the wave group, removing it's wave ID overrides.
	 *
	 * @param group The wave group to deactivate.
	 */
	void decWaveTable(const TWaveGroup* group);

private:
	/* 0x4 */ TWaveInfo** mWaveTable;
	/* 0x8 */ int mWaveTableSize;
	/* 0xC */ TWaveGroup* mWaveGroups;
	/* 0x10 */ u32 mWaveGroupCount;
};

} // namespace JASystem

#endif // JASBASICWAVEBANK_HPP
