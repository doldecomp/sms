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
		    : unk0(-1)
		    , unk4(0)
		    , unk8(nullptr)
		{
		}
		~TKeymap();

		void setVeloRegionCount(u32);
		TVeloRegion* getVeloRegion(int);
		const TVeloRegion* getVeloRegion(int) const;

	public:
		/* 0x0 */ int unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ TVeloRegion* unk8;
	};

	TKeymap* getKeyRegion(int);
	const TKeymap* getKeyRegion(int) const;

public:
	/* 0x4 */ float unk4;
	/* 0x8 */ float unk8;
	/* 0xC */ TInstEffect** unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ TOscillator::Osc_** unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ TKeymap* unk20;
};

} // namespace JASystem

#endif // JASBASICINST_HPP
