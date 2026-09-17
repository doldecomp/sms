#ifndef JASBASICINST_HPP
#define JASBASICINST_HPP

#include <JSystem/JAudio/JASystem/JASInst.hpp>
#include <types.h>

namespace JASystem {

class TInstEffect;

class TBasicInst : public TInst {
public:
	class TKeymap;

	TBasicInst();
	virtual ~TBasicInst();

	virtual bool getParam(int, int, TInstParam*) const;
	virtual int getType() const { return 'BSIC'; }
	virtual int getKeymapIndex(int) const;

	void setVolume(f32 volume) { mVolume = volume; }
	void setPitch(f32 pitch) { mPitch = pitch; }

	void setOscCount(u32);
	void setOsc(int, TOscillator::Osc_*);
	TOscillator::Osc_* getOsc(int);

	void setEffectCount(u32);
	void setEffect(int, TInstEffect*);
	TInstEffect* getEffect(int);

	void setKeyRegionCount(u32);
	TKeymap* searchKeymap(int) const;

	class TKeymap {
	public:
		TKeymap()
		    : mHighKey(-1)
		    , mVeloRegionCount(0)
		    , mVeloRegions(nullptr)
		{
		}
		~TKeymap();

		void setHighKey(int key) { mHighKey = key; }

		void setVeloRegionCount(u32);
		u32 getVeloRegionCount() const { return mVeloRegionCount; }
		TVeloRegion* getVeloRegion(int);
		const TVeloRegion* getVeloRegion(int) const;

	public:
		/* 0x0 */ int mHighKey;
		/* 0x4 */ u32 mVeloRegionCount;
		/* 0x8 */ TVeloRegion* mVeloRegions;
	};

	TKeymap* getKeyRegion(int);
	const TKeymap* getKeyRegion(int) const;

public:
	/* 0x4 */ f32 mVolume;
	/* 0x8 */ f32 mPitch;
	/* 0xC */ TInstEffect** mEffects;
	/* 0x10 */ u32 mEffectCount;
	/* 0x14 */ TOscillator::Osc_** mOscillators;
	/* 0x18 */ u32 mOscillatorCount;
	/* 0x1C */ u32 mKeyRegionCount;
	/* 0x20 */ TKeymap* mKeyRegions;
};

} // namespace JASystem

#endif // JASBASICINST_HPP
