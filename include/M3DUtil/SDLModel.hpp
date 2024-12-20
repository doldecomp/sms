#ifndef SDL_MODEL_HPP
#define SDL_MODEL_HPP

#include <dolphin/types.h>

class J3DModelData;

// 0x1CU
struct SDLModelData {
	SDLModelData(J3DModelData*);

	char padding[0x1CU];
};

//
struct SDLModel {
	SDLModel(SDLModelData*, u32, u32);

	char padding[0xACU];
};

#endif
