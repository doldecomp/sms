#ifndef JASBASICINST_HPP
#define JASBASICINST_HPP

#include <dolphin/types.h>

namespace JASystem {

class TInstEffect;
class TInstParam;

namespace TOscillator {
	struct Osc_;
}

class TInst {
public:
	virtual ~TInst();

	virtual bool getParam(int, int, TInstParam*) const = 0;
	virtual int getType() const                        = 0;
};

class TBasicInst : public TInst {
public:
	TBasicInst();
	virtual ~TBasicInst();

	virtual bool getParam(int, int, TInstParam*) const;
	virtual int getType() const;
	virtual int getKeymapIndex(int) const;

	void setOscCount(u32);
	void setOsc(int, TOscillator::Osc_*);
	TOscillator::Osc_* getOsc(int);

	void setEffectCount(u32);
	void setEffect(int, TInstEffect*);
	TInstEffect* getEffect(int);

	void setKeyRegionCount(u32);
	int searchKeymap(int) const;

	class TKeymap {
	public:
		TKeymap();
		~TKeymap();

		void setVeloRegionCount(u32);
		void* getVeloRegion(int);
		const void* getVeloRegion(int) const;
	};

	TKeymap* getKeyRegion(int);
	const TKeymap* getKeyRegion(int) const;

private:
	// Implementation details not shown
};

} // namespace JASystem

#endif // JASBASICINST_HPP
