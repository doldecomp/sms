#include <dolphin/mtx.h>
struct MockData {
	u16 mWEvlpMtxNum; u8* unk88; u16* unk8C; f32* unk90; Mtx* unk94;
	u16 getWEvlpMtxNum() const { return mWEvlpMtxNum; }
	u8 getWEvlpMixMtxNum(u16 idx) const { return unk88[idx]; }
	u16* getWEvlpMixMtxIndex() const { return unk8C; }
	f32* getWEvlpMixWeight() const { return unk90; }
	MtxPtr getInvJointMtx(u16 idx) const { return unk94[idx]; }
};
struct MockModel {
	MockData* mModelData; u8* mScaleFlagArr; u8* mEvlpScaleFlagArr; Mtx* mNodeMatrices; Mtx* mWeightEvlpMatrices;
	void calcWeightEnvelopeMtx();
};
static f32 J3DUnit01[] = { 0.0f, 1.0f };

#define qr0 0

void MockModel::calcWeightEnvelopeMtx()
{
	register MtxPtr weightAnmMtx;
	register Mtx* worldMtx;
	register MtxPtr invMtx;
	register f32 weight;
	int idx;
	int j;
	int mixNum;
	int i;
	int max;
	u8* pScale;
	u16* indices;
	f32* weights;

	register f32 var_f0;
	register f32 var_f1;
	register f32 var_f2;
	register f32 var_f3;
	register f32 var_f4;
	register f32 var_f5;
	register f32 var_f6;
	register f32 var_f7;
	register f32 var_f8;
	register f32 var_f9;
	register f32 var_f10;
	register f32 var_f11;
	register f32 var_f12;
	register f32 var_f13;
	register f32 var_f31;
	register f32 var_f30;
	register f32 var_f29;
	register f32 var_f28;
	register f32 var_f27;
	register f32* var_r7 = J3DUnit01;

	i       = -1;
	max     = mModelData->getWEvlpMtxNum();
	indices = mModelData->getWEvlpMixMtxIndex() - 1;
	weights = mModelData->getWEvlpMixWeight() - 1;

#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l var_f27, 0x0(var_r7), 0, qr0
		ps_merge00 var_f10, var_f27, var_f27
		ps_merge00 var_f12, var_f27, var_f27
		ps_merge00 var_f31, var_f27, var_f27
	}
#else
	// Portable fallback: weightAnmMtx accumulates
	// sum(weight * (worldMtx * invMtx)) over each envelope's mix matrices.
	Mtx acc;
#endif // clang-format on

	while (++i < max)
	{
		pScale       = &mEvlpScaleFlagArr[i];
		*pScale      = 1;
		weightAnmMtx = mWeightEvlpMatrices[i];

#ifdef __MWERKS__ // clang-format off
		asm {
			ps_merge00 var_f9, var_f27, var_f27
			ps_merge00 var_f11, var_f27, var_f27
			ps_merge00 var_f13, var_f27, var_f27
		}
#else
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 4; c++)
				acc[r][c] = 0.0f;
#endif // clang-format on

		j      = 0;
		mixNum = mModelData->getWEvlpMixMtxNum(i);

		do {
			idx      = *++indices;
			invMtx   = mModelData->getInvJointMtx((u16)idx);
			worldMtx = &mNodeMatrices[idx];

#ifdef __MWERKS__ // clang-format off
			asm {
				psq_l var_f0, 0x0(invMtx), 0, qr0
				psq_l var_f1, 0x0(worldMtx), 0, qr0
				psq_l var_f3, 0x10(worldMtx), 0, qr0
				psq_l var_f5, 0x20(worldMtx), 0, qr0
				ps_muls0 var_f8, var_f0, var_f1
				psq_l var_f6, 0x10(invMtx), 0, qr0
				ps_muls0 var_f30, var_f0, var_f3
				ps_muls0 var_f29, var_f0, var_f5
				psq_l var_f7, 0x20(invMtx), 0, qr0
				ps_madds1 var_f8, var_f6, var_f1, var_f8
				psq_l var_f2, 0x8(worldMtx), 0, qr0
				ps_madds1 var_f30, var_f6, var_f3, var_f30
				psq_l var_f4, 0x18(worldMtx), 0, qr0
				ps_madds1 var_f29, var_f6, var_f5, var_f29
				psq_l var_f6, 0x28(worldMtx), 0, qr0
				ps_madds0 var_f8, var_f7, var_f2, var_f8
			}
#else
			(void)invMtx;
#endif // clang-format on

			weight = *++weights;

#ifdef __MWERKS__ // clang-format off
			asm {
				ps_madds0 var_f30, var_f7, var_f4, var_f30
				ps_madds0 var_f29, var_f7, var_f6, var_f29
				psq_l var_f7, 0x8(invMtx), 0, qr0
				ps_madds0 var_f9, var_f8, weight, var_f9
				ps_madds0 var_f11, var_f30, weight, var_f11
				ps_madds0 var_f13, var_f29, weight, var_f13
				psq_l var_f8, 0x18(invMtx), 0, qr0
				ps_muls0 var_f30, var_f7, var_f1
				ps_muls0 var_f29, var_f7, var_f3
				ps_muls0 var_f28, var_f7, var_f5
				psq_l var_f7, 0x28(invMtx), 0, qr0
				psq_st var_f9, 0x0(weightAnmMtx), 0, qr0
				ps_madds1 var_f30, var_f8, var_f1, var_f30
				ps_madds1 var_f29, var_f8, var_f3, var_f29
				ps_madds1 var_f28, var_f8, var_f5, var_f28
				ps_madds0 var_f30, var_f7, var_f2, var_f30
				ps_madds0 var_f29, var_f7, var_f4, var_f29
				ps_madds0 var_f28, var_f7, var_f6, var_f28
				psq_st var_f11, 0x10(weightAnmMtx), 0, qr0
				psq_st var_f13, 0x20(weightAnmMtx), 0, qr0
				ps_madd var_f30, var_f27, var_f2, var_f30
				ps_madd var_f29, var_f27, var_f4, var_f29
				ps_madd var_f28, var_f27, var_f6, var_f28
				ps_madds0 var_f10, var_f30, weight, var_f10
				ps_madds0 var_f12, var_f29, weight, var_f12
				ps_madds0 var_f31, var_f28, weight, var_f31
			}
#else
			for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 4; c++) {
					f32 v = (*worldMtx)[r][0] * invMtx[0][c]
					        + (*worldMtx)[r][1] * invMtx[1][c]
					        + (*worldMtx)[r][2] * invMtx[2][c];
					if (c == 3)
						v += (*worldMtx)[r][3];
					acc[r][c] += v * weight;
				}
			}
#endif // clang-format on

			*pScale &= mScaleFlagArr[idx];
		} while (++j < mixNum);

#ifdef __MWERKS__ // clang-format off
		asm {
			psq_st var_f10, 0x8(weightAnmMtx), 0, qr0
			ps_merge00 var_f10, var_f27, var_f27
			psq_st var_f12, 0x18(weightAnmMtx), 0, qr0
			ps_merge00 var_f12, var_f27, var_f27
			psq_st var_f31, 0x28(weightAnmMtx), 0, qr0
			ps_merge00 var_f31, var_f27, var_f27
		}
#else
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 4; c++)
				weightAnmMtx[r][c] = acc[r][c];
#endif // clang-format on
	}
}

