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

		void setRelease(u32);
		void setEffectCount(u32);
		void setEffect(int, TInstEffect*);
		void setVeloRegionCount(u32);

		TVeloRegion* getVeloRegion(int);
		const TVeloRegion* getVeloRegion(int) const;

		TInstEffect* getEffect(int);

	public:
		/* 0x0 */ f32 unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ f32 unk8;
		/* 0xC */ u16 unkC;
		/* 0x10 */ TInstEffect** unk10;
		/* 0x14 */ u32 unk14;
		/* 0x18 */ u32 unk18;
		/* 0x1C */ TVeloRegion* unk1C;
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
