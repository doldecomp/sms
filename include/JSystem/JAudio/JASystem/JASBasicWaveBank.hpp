#ifndef JASBASICWAVEBANK_HPP
#define JASBASICWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TBasicWaveBank : public TWaveBank {
public:
	TBasicWaveBank();

	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		virtual ~TWaveHandle();
		virtual const TWaveInfo* getWaveInfo() const;
		virtual const void* getWavePtr() const;
	};

	virtual ~TBasicWaveBank();
	virtual TWaveHandle* getWaveHandle(u32) const;
	virtual int getType() const;

	void setGroupCount(u32);
	void setWaveTableSize(u32);

	class TWaveInfo {
	public:
		TWaveInfo();
		~TWaveInfo();
	};

	class TWaveGroup {
	public:
		TWaveGroup();
		~TWaveGroup();

		virtual bool* getLoadFlagPtr();
		virtual const char* getWaveArcFileName() const;
		virtual JKRHeap* getHeap();

		void setWaveCount(u32);
		void setWaveInfo(int, u32, const JASystem::TWaveInfo&);
		void setWaveArcFileName(const char*);
	};

	TWaveGroup* getWaveGroup(int);
	void incWaveTable(const TWaveGroup*);
	void decWaveTable(const TWaveGroup*);

private:
};

} // namespace JASystem

#endif // JASBASICWAVEBANK_HPP
