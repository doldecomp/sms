#ifndef JASBASICWAVEBANK_HPP
#define JASBASICWAVEBANK_HPP

#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

class TWaveInfo;

class TBasicWaveBank : public TWaveBank {
public:
	TBasicWaveBank();
	virtual ~TBasicWaveBank();

	virtual int getType() const;
	virtual TWaveHandle* getWaveHandle(unsigned long) const;

	void setGroupCount(unsigned long);
	void setWaveTableSize(unsigned long);

	class TWaveInfo {
	public:
		TWaveInfo();
		~TWaveInfo();
	};

	class TWaveHandle : public JASystem::TWaveHandle {
	public:
		virtual ~TWaveHandle();
		virtual const void* getWavePtr() const;
		virtual const TWaveInfo* getWaveInfo() const;
	};

	class TWaveGroup {
	public:
		TWaveGroup();
		virtual ~TWaveGroup();

		void setWaveCount(unsigned long);
		void setWaveInfo(int, unsigned long, const TWaveInfo&);
		void setWaveArcFileName(const char*);

		const char* getWaveArcFileName() const;
		JKRHeap* getHeap();
		bool* getLoadFlagPtr();
	};

	TWaveGroup* getWaveGroup(int);
	void incWaveTable(const TWaveGroup*);
	void decWaveTable(const TWaveGroup*);

private:
	// Implementation details not shown
};

namespace WaveArcLoader {
	class TObject {
		// Implementation details not shown
	};
} // namespace WaveArcLoader

} // namespace JASystem

#endif // JASBASICWAVEBANK_HPP
