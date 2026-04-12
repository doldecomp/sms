#ifndef NPC_INIT_ANM_DATA_HPP
#define NPC_INIT_ANM_DATA_HPP

#include <M3DUtil/LodAnm.hpp>

// fabricated
struct TNpcInitAnmInfo {
	/* 0x0 */ const TLodAnmIndex* unk0;
	/* 0x4 */ const TAnmBtpMapping* unk4;
};

const TNpcInitAnmInfo* SMSGetNpcInitAnmData(u32 index);

#endif
