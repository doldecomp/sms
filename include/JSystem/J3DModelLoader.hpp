#ifndef J3D_MODEL_LOADER_HPP
#define J3D_MODEL_LOADER_HPP

#include <dolphin/types.h>
#include <JSystem/J3DModel.hpp>

struct J3DModelLoaderDataBase {
	// NOTE: I have no idea why, but it seems like they pass in the
	// J3DModelLoaderDataBase through a void* pointer here?!
	static J3DModelData* load(const void*, u32);
	static void* loadMaterialTable(const void*);
};

#endif
