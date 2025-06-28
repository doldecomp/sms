#ifndef M3DUTIL_SDL_MODEL_HPP
#define M3DUTIL_SDL_MODEL_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/JGadget/std-list.hpp>

class J3DModelData;
class J3DMaterial;
class J3DNode;
class SDLModel;

struct SDLDrawBufToken {
	/* 0x0 */ J3DDrawBuffer* unk0[2];
	/* 0x8 */ J3DModel* unk8;
};

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

	// fabricated
	J3DModelData* getModelData() { return unk0; }

public:
	/* 0x0 */ J3DModelData* unk0;
	/* 0x4 */ SDLModel* unk4;
	/* 0x8 */ JGadget::TList<SDLDrawBufToken*> unk8;
	/* 0x18 */ u32 unk18;
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

	// fabricated
	SDLModelData* getSDLModelData() { return unkA0; }

public:
	/* 0xA0 */ SDLModelData* unkA0;
	/* 0xA4 */ J3DDrawBuffer* unkA4;
	/* 0xA8 */ u32 unkA8;
};

#endif
