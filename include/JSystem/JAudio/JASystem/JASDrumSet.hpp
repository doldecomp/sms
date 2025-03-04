#ifndef JASDRUMSET_HPP
#define JASDRUMSET_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>

namespace JASystem {

class TInstVeloRegion;
class TInstParam;

class TDrumSet : public TInst {
public:
	class TPerc {
	public:
		TPerc();
		~TPerc();

		void setRelease(u32);
		void setEffectCount(u32);
		void setEffect(int, TInstEffect*);
		void setVeloRegionCount(u32);
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
