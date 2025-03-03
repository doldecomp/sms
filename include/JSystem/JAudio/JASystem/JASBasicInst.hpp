#ifndef JASBASICINST_HPP
#define JASBASICINST_HPP

#include <JSystem/JAudio/JASystem/JASInst.hpp>

namespace JASystem {

class TInstEffect;
class TInstParam;

namespace TOscillator {
	struct Osc_;
}

class TBasicInst : public TInst {
public:
	TBasicInst();
	virtual ~TBasicInst();

	virtual int getType() const;
	virtual int getKeymapIndex(int) const;
	virtual bool getParam(int, int, TInstParam*) const;

	void setOscCount(unsigned long);
	void setOsc(int, TOscillator::Osc_*);
	TOscillator::Osc_* getOsc(int);

	void setEffectCount(unsigned long);
	void setEffect(int, TInstEffect*);
	TInstEffect* getEffect(int);

	void setKeyRegionCount(unsigned long);
	int searchKeymap(int) const;

	class TKeymap {
	public:
		TKeymap();
		~TKeymap();

		void setVeloRegionCount(unsigned long);
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
