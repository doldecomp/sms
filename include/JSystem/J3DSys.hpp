#ifndef J3DSYS_HPP
#define J3DSYS_HPP

#include "dolphin.h"

#include "types.h"

class J3DSys {
public:
	void setTexCacheRegion(GXTexCacheSize);
	char pad_000[0x34];
	s32 _034;
	char pad_001[0x4];
  void* _03C_4_;
	char pad_002[0x14];
	u32 _054_4_;
	char padd_003[0xb4];
	u32 _10C_4_;
	u32 _110_4_;
	u32 _114_4_;
};

extern J3DSys j3dSys;
// TODO: type
extern void* j3dDefaultViewNo;

#endif
