#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundStruct.hpp>
#include <MSound/MSound.hpp>

MSSetSound* MSSetSound::smSetSound[9];

void MSSetSound::init()
{
	smSetSound[0] = new MSSetSound(0x6800, "放水着地音", 2, 7, 6, 4, 184.0f, 1,
	                               22.12f, 150.0f, 0.85f, 0.937f, 295.2f, 0,
	                               937.3f, 0xaf, 0.95f, 0.97f, 51.63f, false);

	smSetSound[1] = new MSSetSound(
	    0x6809, "落書き消し音", 2, 4, 7, 5, 100.0f, 1, 11.0f, 17000.0f, 0.52f,
	    0.9f, 221.78f, 0x16, 8000.0f, 0xfa, 0.81f, 1.35f, 153.73f, true);

	smSetSound[2] = new MSSetSound(0x6807, "ヒノクリ汚染着地音", 2, 9, 15, 12,
	                               100.0f, 1, 44.0f, 3.0f, 1.0f, 1.0f, 0.0f,
	                               0xf, 200.0f, 0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[3] = new MSSetSound(0x3803, "火柱", 2, 9, 15, 12, 100.0f, 1,
	                               44.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0xf, 200.0f,
	                               0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[4] = new MSSetSound(0x3805, "電気柱", 2, 9, 15, 12, 100.0f, 1,
	                               44.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0xf, 200.0f,
	                               0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[5] = new MSSetSound(0x804, "水乾燥音", 2, 4, 1, 10, 320.0f, 1,
	                               44.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0xf, 200.0f,
	                               0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[6] = new MSSetSound(
	    0x6802, "水ヒットマーク", 2, 3, 10, 2, 300.0f, 1, 22.12f, 150.0f, 0.85f,
	    0.937f, 295.2f, 0, 937.3f, 0xaf, 0.95f, 0.97f, 51.63f, true);

	smSetSound[7] = new MSSetSound(0x6801, "放水着地音", 2, 7, 6, 4, 184.0f, 1,
	                               22.12f, 150.0f, 0.85f, 0.937f, 295.2f, 0,
	                               937.3f, 0xaf, 0.95f, 0.97f, 51.63f, false);

	smSetSound[8] = new MSSetSound(0x899b, "マンタ襲撃声", 3, 4, 63, 4, 184.0f,
	                               1, 22.12f, 150.0f, 0.94f, 0.815f, 280.2f, 0,
	                               937.3f, 0xaf, 0.95f, 0.97f, 51.63f, false);
}

bool MSSetSound::startSoundSet(u32 param1, const Vec* param2, u32 param3,
                               f32 param4, u32 param5, u32 param6, u8 param7)
{
	if (!MSGMSound->gateCheck(param1))
		return false;

	MSSetSoundTL<MSSetSound>* which = nullptr;
	switch (param1) {
		// clang-format off
	case 0x6800: which = smSetSound[0]; break;
  case 0x6809: which = smSetSound[1]; break;
	case 0x6807: which = smSetSound[2]; break;
	case 0x3803: which = smSetSound[3]; break;
	case 0x3805: which = smSetSound[4]; break;
	case 0x804:  which = smSetSound[5]; break;
	case 0x6802: which = smSetSound[6]; break;
	case 0x6801: which = smSetSound[7]; break;
	case 0x899b: which = smSetSound[8]; break;
		// clang-format on
	}

	if (!which)
		return false;

	return which->startSoundSetDyna(param1, param2, param3, param4, param5,
	                                param6, param7, nullptr);
}

bool MSSetSoundGrp::startSoundSetGrp(u32 param1, const Vec* param2, u32 param3,
                                     f32 param4, u32 param5, u32 param6,
                                     u8 param7)
{
	if (!MSGMSound->gateCheck(param1))
		return false;

	MSSetSoundGrp* grp = MSSetSoundGrp::searchGroup(param1);

	if (!grp)
		return false;

	return grp->startSoundSetDyna(param1, param2, param3, param4, param5,
	                              param6, param7, nullptr);
}
