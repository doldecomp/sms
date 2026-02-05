#ifndef MSSETSOUNDTL_HPP
#define MSSETSOUNDTL_HPP

#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <JSystem/JAudio/JADebug/JADPrm.hpp>
#include <JSystem/JAudio/JALibrary/JALList.hpp>
#include <dolphin/types.h>
#include <dolphin/mtx.h>

class MSSetSoundMember : public JALLinkD<MSSetSoundMember, u32> {
public:
	MSSetSoundMember(u32 param_1, JALListD<MSSetSoundMember, u32>* param_2,
	                 float param_3)
	    : JALLinkD<MSSetSoundMember, u32>(this, param_1, param_2)
	    , unk18(param_3)
	{
	}

public:
	/* 0x18 */ float unk18;
};

class MSSetSoundGrp;

template <typename T>
class MSSetSoundTL : public JALListHioNode<T, u32>, public JALListFrameLoop<T> {
public:
	MSSetSoundTL(u32 param_1, const char* param_2, T* param_3, u8 param_4,
	             u8 param_5, u8 param_6, u8 param_7, f32 param_8, u8 param_9,
	             f32 param_10, f32 param_11, f32 param_12, f32 param_13,
	             f32 param_14, s32 param_15, f32 param_16, s32 param_17,
	             f32 param_18, f32 param_19, f32 param_20, bool param_21)
	    : JALListHioNode<T, u32>(param_2, param_1, param_3)
	    , unk1C(param_4, "再生バッファ数")
	    , unk1D(param_5, "最小時間間隔")
	    , unk1E(param_6, "ランダムシフト")
	    , unk1F(param_7, "単位音時間")
	    , unk20(param_8, "単位音距離")
	    , unk24(param_9, "単位音間引き")
	    , unk28(param_10, "単位変調時間")
	    , unk2C(param_11, "単位変調距離")
	    , unk30(param_12, "ボリューム変調最低値")
	    , unk34(param_13, "ピッチ変調最低値")
	    , unk38(param_14, "アタック最大値")
	    , unk3C(param_15, "連続性を定義する音毎の時間差")
	    , unk40(param_16, "連続性を定義する距離")
	    , unk44(param_17, "最大連続時間")
	    , unk48(param_18, "ボリューム連続変調最小値")
	    , unk4C(param_19, "ピッチ連続変調変化値")
	    , unk50(param_20, "アタック連続変調最大値")
	    , unk54(0)
	    , unk58(0)
	{
		unkAC.x = 0.0f;
		unkAC.y = 0.0f;
		unkAC.z = 0.0f;

		unkB8 = 0;
		unkB9 = param_21;
		unk5A = 0;
		unk59 = 1;
		unk5C = 0;
		unk60 = 0;
		unk64 = 0;
		unk68 = 0;
		unk6C = 0;
	}
	~MSSetSoundTL();

	virtual void frameLoopDyna()
	{
		if (unk58 != 0)
			unk54 = unk54 == -1 ? 0 : unk54 + 1;

		unkB8 = 0;
	}

	bool startSoundSetDyna(u32 param1, const Vec* param2, u32 param3,
	                       f32 param4, u32 param5, u32 param6, u8 param7,
	                       MSSetSoundGrp* param8)
	{
		// TODO: stuff
	}

public:
	/* 0x1C */ JADPrm<u8> unk1C;
	/* 0x1D */ JADPrm<u8> unk1D;
	/* 0x1E */ JADPrm<u8> unk1E;
	/* 0x1F */ JADPrm<u8> unk1F;
	/* 0x20 */ JADPrm<f32> unk20;
	/* 0x24 */ JADPrm<u8> unk24;
	/* 0x28 */ JADPrm<f32> unk28;
	/* 0x2C */ JADPrm<f32> unk2C;
	/* 0x30 */ JADPrm<f32> unk30;
	/* 0x34 */ JADPrm<f32> unk34;
	/* 0x38 */ JADPrm<f32> unk38;
	/* 0x3C */ JADPrm<s32> unk3C;
	/* 0x40 */ JADPrm<f32> unk40;
	/* 0x44 */ JADPrm<s32> unk44;
	/* 0x48 */ JADPrm<f32> unk48;
	/* 0x4C */ JADPrm<f32> unk4C;
	/* 0x50 */ JADPrm<f32> unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u8 unk58;
	/* 0x59 */ u8 unk59;
	/* 0x5A */ u8 unk5A;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ char unk70[0xAC - 0x70];
	/* 0xAC */ Vec unkAC;
	/* 0xB8 */ u8 unkB8;
	/* 0xB9 */ bool unkB9;
};

class MSSetSound : public MSSetSoundTL<MSSetSound> {
public:
	MSSetSound(u32 param_1, const char* param_2, u8 param_3, u8 param_4,
	           u8 param_5, u8 param_6, f32 param_7, u8 param_8, f32 param_9,
	           f32 param_10, f32 param_11, f32 param_12, f32 param_13,
	           s32 param_14, f32 param_15, s32 param_16, f32 param_17,
	           f32 param_18, f32 param_19, bool param_20)
	    : MSSetSoundTL<MSSetSound>(param_1, param_2, this, param_3, param_4,
	                               param_5, param_6, param_7, param_8, param_9,
	                               param_10, param_11, param_12, param_13,
	                               param_14, param_15, param_16, param_17,
	                               param_18, param_19, param_20)
	{
	}

	static MSSetSound* smSetSound[9];
	static void init();
	static bool startSoundSet(u32 param1, const Vec* param2, u32 param3,
	                          f32 param4, u32 param5, u32 param6, u8 param7);
};

class MSSetSoundGrp : public MSSetSoundTL<MSSetSoundGrp>,
                      public JALListGrp<MSSetSoundGrp, u32, MSSetSoundMember> {
public:
	MSSetSoundGrp(u32 param_1, const char* param_2, u8 param_3, u8 param_4,
	              u8 param_5, u8 param_6, f32 param_7, u8 param_8, f32 param_9,
	              f32 param_10, f32 param_11, f32 param_12, f32 param_13,
	              s32 param_14, f32 param_15, s32 param_16, f32 param_17,
	              f32 param_18, f32 param_19, bool param_20)
	    : MSSetSoundTL<MSSetSoundGrp>(param_1, param_2, this, param_3, param_4,
	                                  param_5, param_6, param_7, param_8,
	                                  param_9, param_10, param_11, param_12,
	                                  param_13, param_14, param_15, param_16,
	                                  param_17, param_18, param_19, param_20)
	{
	}

	static bool startSoundSetGrp(u32 param1, const Vec* param2, u32 param3,
	                             f32 param4, u32 param5, u32 param6, u8 param7);
};

#endif // MSSETSOUNDTL_HPP
