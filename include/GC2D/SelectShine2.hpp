#ifndef GC2D_SELECT_SHINE_2_HPP
#define GC2D_SELECT_SHINE_2_HPP

#include <GC2D/Option.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

class TSelectShineManager : public JDrama::TViewObj {
public:
	TSelectShineManager(const char*);
	virtual ~TSelectShineManager();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initData(u8*, u8, u8, JPAEmitterManager*);
	void startClose();
	void startIncrease(int);
	void startDecrease(int);

public:
	/* 0x10 */ TOptionRumbleUnit* mRumbleOption[8];
	/* 0x30 */ u8 unk10[0xF0];
};

#endif // GC2D_SELECT_SHINE_2_HPP
