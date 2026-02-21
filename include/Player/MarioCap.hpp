#ifndef MARIOCAP_HPP
#define MARIOCAP_HPP

#include <Player/MarioMain.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Strategic/MirrorActor.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>

class TMarioCap {
public:
	enum E_CAP_MODEL {
		E_CAP_MODEL_NOTHING    = 0,
		E_CAP_MODEL_HAT        = 1,
		E_CAP_MODEL_HELMET     = 2,
		E_CAP_MODEL_SUNGLASSES = 4,
	};

	TMarioCap(TMario*);

	virtual void perform(unsigned long, JDrama::TGraphics*);

	void createMirrorCap();
	void mtxEffectHide();
	void mtxEffectShow();

	// Unused
	void addDirty();

	// Fabricated
	bool isModelActive(u16 type) { return unk4 & type ? true : false; }

	// Fabricated
	void setModelActive(u16 type) { unk4 |= type; }

	// Fabricated
	void setModelInactive(u16 type) { unk4 &= ~type; }

	// Fabricated
	void onFlagAllShapes(J3DModelData* modelData)
	{
		for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
			modelData->getShapeNodePointer(i)->onFlag(1);
		}
	}

	// Fabricated
	void offFlagAllShapes(J3DModelData* modelData)
	{
		for (u16 i = 0; i < modelData->getShapeNum(); ++i) {
			modelData->getShapeNodePointer(i)->offFlag(1);
		}
	}

public:
	/* 0x4 */ u16 unk4; // E_CAP_MODEL flag
	/* 0x6 */ u16 unk6;
	/* 0x8 */ TMario* mMario;
	/* 0xC */ J3DModel* unkC;      // current model?
	/* 0x10 */ J3DModel* unk10[4]; // ma_cap1.bmd model
	/* 0x20 */ TMultiMtxEffect* unk20;
	/* 0x24 */ TMultiMtxEffect* unk24;
	/* 0x28 */ TMirrorActor* unk28[2];
	/* 0x30 */ TTrembleModelEffect* unk30;
	/* 0x34 */ f32 unk34;
};

#endif
