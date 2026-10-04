#ifndef JASDRUMSET_HPP
#define JASDRUMSET_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASInstEffect.hpp>

namespace JASystem {

class TInstParam;

class TDrumSet : public TInst {
public:
	class TPerc {
	public:
		TPerc();
		~TPerc();

		void setVolume(f32 volume) { mVolume = volume; }
		void setPitch(f32 pitch) { mPitch = pitch; }
		void setPan(f32 pan) { mPan = pan; }

		void setRelease(u32);
		void setEffectCount(u32);
		void setEffect(int, TInstEffect*);
		void setVeloRegionCount(u32);

		TVeloRegion* getVeloRegion(int);
		const TVeloRegion* getVeloRegion(int) const;

		TInstEffect* getEffect(int);

	public:
		/* 0x0 */ f32 mVolume;
		/* 0x4 */ f32 mPitch;
		/* 0x8 */ f32 mPan;
		/* 0xC */ u16 mRelease;
		/* 0x10 */ TInstEffect** mEffects;
		/* 0x14 */ u32 mEffectCount;
		/* 0x18 */ u32 mVeloRegionCount;
		/* 0x1C */ TVeloRegion* mVeloRegions;
	};

	TDrumSet() { }

	virtual ~TDrumSet() { }
	virtual int getType() const { return 'PERC'; }
	virtual bool getParam(int, int, TInstParam*) const;

	TPerc* getPerc(int);
	const TPerc* getPerc(int) const;

public:
	/* 0x04 */ TPerc unk4[128];
};

} // namespace JASystem

#endif // JASDRUMSET_HPP
