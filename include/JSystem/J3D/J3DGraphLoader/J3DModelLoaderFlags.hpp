#ifndef J3D_MODEL_LOADER_FLAGS_HPP
#define J3D_MODEL_LOADER_FLAGS_HPP

#include <types.h>

enum J3DModelLoaderFlag {
	J3DMLF_None = 0x00000000,

	J3DMLF_MtxCalcMask      = 0x0000000F,
	J3DMLF_MtxCalcBasic     = 0x00000000,
	J3DMLF_MtxCalcSoftImage = 0x00000001,
	J3DMLF_MtxCalcMaya      = 0x00000002,

	J3DMLF_TevStageNumShift = 16,
	J3DMLF_TevStageNumMask  = 0x001F0000,

	J3DMLF_UseUniqueMaterials   = 0x00200000,
	J3DMLF_MaterialUseIndirect  = 0x01000000,
	J3DMLF_MaterialTexGenFull   = 0x04000000,
	J3DMLF_MaterialPEFull       = 0x10000000,
	J3DMLF_MaterialColorLightOn = 0x40000000,
};

#endif
