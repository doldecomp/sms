#ifndef ENEMY_CHUU_HANA_HPP
#define ENEMY_CHUU_HANA_HPP

#include <Enemy/WalkerEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TChuuHana : public TWalkerEnemy {
public:
	// static members (map: .sdata)
	static u32 mCheckOnPanelTimeRoll;
	static u32 mCheckOnPanelTime;
	static u8 mBodyJntIndex;
	static u8 mEyeJntIndex;
	static u8 mFootJntIndex;
	static u8 mNewSw;
	static u8 mCompareHeight;
	static f32 mSmallMirrorR;
	static f32 mMediumMirrorR;
	static f32 mLargeMirrorR;
	static u8 mAttackVersion;
	static u8 mDamageSw;
};

#endif
