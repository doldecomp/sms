#ifndef JASBASICWAVEBANK_HPP
#define JASBASICWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/types.h>

namespace JASystem {

class TWaveInfo;
class TWaveHandle {
public:
	virtual ~TWaveHandle();
	virtual const void* getWavePtr() const       = 0;
	virtual const TWaveInfo* getWaveInfo() const = 0;
};

class TBasicWaveBank : public TWaveBank {
public:
	TBasicWaveBank();
	virtual ~TBasicWaveBank();

	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		virtual ~TWaveHandle();
		virtual const void* getWavePtr() const;
		virtual const TWaveInfo* getWaveInfo() const;
	};

	virtual int getType() const;
	virtual TWaveHandle* getWaveHandle(u32) const;

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
		virtual ~TWaveGroup();

		void setWaveCount(u32);
		void setWaveInfo(int, u32, const JASystem::TWaveInfo&);
		void setWaveArcFileName(const char*);

		const char* getWaveArcFileName() const;
		JKRHeap* getHeap();
		bool* getLoadFlagPtr();
	};

	TWaveGroup* getWaveGroup(int);
	void incWaveTable(const TWaveGroup*);
	void decWaveTable(const TWaveGroup*);

private:
};

} // namespace JASystem

#endif // JASBASICWAVEBANK_HPP
