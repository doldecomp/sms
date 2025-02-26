#ifndef M3DUTIL_SDL_MODEL_HPP
#define M3DUTIL_SDL_MODEL_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>

class J3DModelData;
class J3DMaterial;
class J3DNode;
class SDLModel;

struct SDLDrawBufToken { };

// 0x1CU
class SDLModelData {
public:
	SDLModelData(J3DModelData*);
	~SDLModelData();

	void entrySDLModels();
	void registerSDLModel(SDLModel*);
	void recursiveEntry(J3DNode*, SDLDrawBufToken*);
	void entryNode(J3DNode*, SDLDrawBufToken*);
	void entrySameMat(J3DMaterial*, SDLDrawBufToken*);

public:
	char padding[0x1CU];
};

class SDLMatPacket : public J3DMatPacket {
public:
	SDLMatPacket();
	~SDLMatPacket() { }

	void newSingleDL(u32);
	void beParasiteDL(J3DMatPacket*);
};

//
class SDLModel : public J3DModel {
public:
	SDLModel(J3DModelData*, u32);
	SDLModel(SDLModelData*, u32, u32);
	virtual ~SDLModel() { }

	virtual void viewCalcSimple();
	void entry();
	void entryModelDataSDL(SDLModelData*, u32, u32);

public:
	char padding[0xACU];
};

#endif
