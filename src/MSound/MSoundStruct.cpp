#include <MSound/MSSetSound.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <MSound/MSoundStruct.hpp>
#include <MSound/MSound.hpp>

// rogue
#include <MSound/MSoundBGM.hpp>

MSSetSound* MSSetSound::smSetSound[9];

void MSSetSound::init()
{
	smSetSound[0] = new MSSetSound(0x6800, "放水着地音", 2, 7, 6, 4, 184.0f, 1,
	                               22.12f, 150.0f, 0.85f, 0.937f, 295.2f, 0,
	                               937.3f, 0xaf, 0.95f, 0.97f, 51.63f, false);

	smSetSound[1] = new MSSetSound(
	    0x6809, "落書き消し音", 2, 4, 7, 5, 100.0f, 1, 11.0f, 17000.0f, 0.52f,
	    0.9f, 221.78f, 0x16, 8000.0f, 0xfa, 0.81f, 1.35f, 153.73f, true);

	smSetSound[2] = new MSSetSound(0x6807, "ヒノクリ汚染着地音", 2, 9, 15, 18,
	                               100.0f, 1, 44.0f, 3.0f, 1.0f, 1.0f, 0.0f,
	                               0xf, 200.0f, 0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[3] = new MSSetSound(0x3803, "火柱", 2, 9, 15, 18, 100.0f, 1,
	                               44.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0xf, 200.0f,
	                               0xb4, 1.0f, 1.0f, 0.0f, false);

	smSetSound[4] = new MSSetSound(0x3805, "電気柱", 2, 9, 15, 18, 100.0f, 1,
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

	if (which)
		return which->startSoundSetDyna(param1, param2, param3, param4, param5,
		                                param6, param7, nullptr);

	return false;
}

bool MSSetSoundGrp::startSoundSetGrp(u32 param1, const Vec* param2, u32 param3,
                                     f32 param4, u32 param5, u32 param6,
                                     u8 param7)
{
	if (!MSGMSound->gateCheck(param1))
		return false;

	MSSetSoundGrp* grp = MSSetSoundGrp::searchGroup(param1);

	if (grp)
		return grp->startSoundSetDyna(param1, param2, param3, param4, param5,
		                              param6, param7, grp);

	return false;
}

// Their original include structure was complete nonsense,
// so this shall live here for now to avoid circular includes.
template <typename T>
bool MSSetSoundTL<T>::startSoundSetDyna(u32 param_1, const Vec* param_2,
                                        u32 param_3, f32 param_4, u32 param_5,
                                        u32 param_6, u8 param_7,
                                        MSSetSoundGrp* param_8)
{
	f32 dVar9 = param_4;

	if (unkB8 == 1)
		return true;

	f32 f31 = JALCalc::getDist(&unkAC, (Vec*)param_2);

	if (unk5C[unk5A] == nullptr) {
		f32 f29 = param_4;
		u32 r31 = param_1;

		switch (param_1) {
		case 0x6809:
			if (unk5C[unk5A] != nullptr) {
				if ((f32)unk5C[unk5A]->unk14 < (f32)unk3C.get()
				    && f31 < unk40.get())
					r31 = 0x680A;
			}
			break;
		case 0x6800:
			f29 = MSGMSound->getDistFromCamera((Vec*)param_2);
			break;
		}

		const Vec* ptr;
		if (unkB9 == 1) {
			unk70[unk59] = *param_2;
			ptr          = &unk70[unk59];
		} else {
			ptr = nullptr;
		}

		if (ptr == nullptr)
			ptr = param_2;

		JAIActor local_94(ptr, ptr, ptr, param_5);
		MSoundSESystem::MSoundSE::startSoundActorInner(
		    r31, &unk5C[unk59], &local_94, param_6, param_7);
		JALSystem::processModFunc(unk5C[unk59], f29, 0, 3);

		unkAC = *param_2;
		unkB8 = 1;
	} else {
		bool bVar2;

		u32 bVar1 = unk1D.get();
		u32 uVar5 = JALCalc::getRandom_0_1() * unk1E.get();
		u32 uVar7 = unk5C[unk5A]->unk14;
		if (uVar7 < bVar1 + uVar5) {
			bVar2 = false;
		} else {
			if (unk24.get() == 1 && unk1F.get() < uVar7 && f31 < unk20.get()) {
				bVar2 = false;
			} else {
				// TODO: definitely an inline, non-structured control flow.
				// Probably even two to get searchD to not inline.
				if (param_8 != nullptr) {
					MSSetSoundMember* candidate
					    = param_8->searchD(unk5C[unk5A]->unk8);
					if (!candidate) {
						bVar2 = false;
					} else if (uVar7 < candidate->unk18) {
						bVar2 = false;
					}
				} else {
					bVar2 = true;
				}
			}
		}

		if (!bVar2)
			return true;

		f32 f29 = param_4;
		u32 r26 = param_1;
		switch (param_1) {
		case 0x6809:
			if (unk5C[unk5A] != nullptr) {
				if ((f32)unk5C[unk5A]->unk14 < (f32)unk3C.get()
				    && f31 < unk40.get()) {
					r26 = 0x680A;
				}
			}
			break;
		case 0x6800:
			f29 = MSGMSound->getDistFromCamera((Vec*)param_2);
			break;
		}

		const Vec* ptr;
		if (unkB9 == 1) {
			unk70[unk59] = *param_2;
			ptr          = &unk70[unk59];
		} else {
			ptr = nullptr;
		}

		if (ptr == nullptr)
			ptr = param_2;

		JAIActor local_AC(ptr, ptr, ptr, param_5);
		MSoundSESystem::MSoundSE::startSoundActorInner(
		    r26, &unk5C[unk59], &local_AC, param_6, param_7);
		JALSystem::processModFunc(unk5C[unk59], f29, 0, 3);

		unkAC = *param_2;
		unkB8 = 1;

		if (unk5C[unk59] != nullptr) {
			JAISound* sound = unk5C[unk5A];
			if (unk5C[unk59] != nullptr) {
				u32 uVar7  = sound->unk14;
				f32 unused = 1.0f;
				f32 f29    = 1.0f;
				f32 f30    = 1.0f;

				f32 f1 = unk2C.get();
				if ((f32)uVar7 < (f32)unk28.get() && f31 < f1) {
					unused = JALCalc::linearTransform(uVar7, unk1D.get(),
					                                  unk28.get(), unk38.get(),
					                                  0.0f, false);
					f30    = JALCalc::linearTransform(uVar7, unk1D.get(),
					                                  unk28.get(), unk30.get(),
					                                  1.0f, false);
					f29    = JALCalc::linearTransform(uVar7, unk1D.get(),
					                                  unk28.get(), 1.0f,
					                                  unk34.get(), false);
				}

				f32 f27 = 1.0f;
				f32 f28 = 1.0f;
				if ((f32)uVar7 < (f32)unk3C.get() && f31 < unk40.get()) {
					unk58 = 1;
					JALCalc::linearTransform((f32)unk54, 0.0f, unk44.get(),
					                         0.0f, unk50.get(), false);
					f27 = JALCalc::linearTransform((f32)unk54, 0.0f,
					                               unk44.get(), 1.0f,
					                               unk48.get(), false);
					f28 = JALCalc::linearTransform((f32)unk54, 0.0f,
					                               unk44.get(), 1.0f,
					                               unk4C.get(), false);
				} else {
					unk58 = 0;
					unk54 = 0;
				}
				JAISound* snd = unk5C[unk59];
				snd->setVolume(f30 * f27, 3, 0);
				snd->setPitch(f29 * f28, 3, 0);
			} else {
				// Huh.
				if (unk5C[unk59] != nullptr)
					unk5C[unk59]->setPortData(13, 1);
				unk58 = 0;
				unk54 = 0;
			}
		}
	}

	unk59 = unk59 == unk1C.get() - 1 ? 0 : unk59 + 1;
	unk5A = unk5A == unk1C.get() - 1 ? 0 : unk5A + 1;
	return true;
}
