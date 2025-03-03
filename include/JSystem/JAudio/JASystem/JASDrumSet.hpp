#ifndef JASDRUMSET_HPP
#define JASDRUMSET_HPP

#include <JSystem/JAudio/JASystem/JASInst.hpp>

namespace JASystem {

class TDrumSet {
public:
	class TPerc {
	public:
		TPerc();
		~TPerc();

		void setRelease(unsigned long);
		void setEffectCount(unsigned long);
		void setEffect(int, TInstEffect*);
		void setVeloRegionCount(unsigned long);
		TInstVeloRegion* getVeloRegion(int);
		const TInstVeloRegion* getVeloRegion(int) const;
		TInstEffect* getEffect(int);
	};

	TDrumSet();
	virtual ~TDrumSet();

	virtual int getType() const;
	virtual bool getParam(int, int, TInstParam*) const;

	TPerc* getPerc(int);
	const TPerc* getPerc(int) const;
};

} // namespace JASystem

#endif // JASDRUMSET_HPP
