#ifndef M3DUTIL_SDL_MODEL_HPP
#define M3DUTIL_SDL_MODEL_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/JGadget/std-list.hpp>

class J3DModelData;
class J3DMaterial;
class J3DNode;
class SDLModel;
class SDLDrawBufToken;

class SDLModelData {
public:
	SDLModelData(J3DModelData*);

	void entrySDLModels();
	void registerSDLModel(SDLModel*);
	void recursiveEntry(J3DNode*, SDLDrawBufToken*);
	void entryNode(J3DNode*, SDLDrawBufToken*);
	void entrySameMat(J3DMaterial* material, SDLDrawBufToken* token);

	// used from enemymanager.cpp and NpcManager.cpp
	J3DModelData* getModelData() { return unk0; }

	// fabricated

public:
	/* 0x0 */ J3DModelData* unk0;
	/* 0x4 */ SDLModel* mDlHost;
	/* 0x8 */ JGadget::TList<SDLDrawBufToken*> mDbTokenList;
	/* 0x18 */ u32 unk18;
};

class SDLMatPacket : public J3DMatPacket {
public:
	SDLMatPacket();

	void newSingleDL(u32);
	void beParasiteDL(J3DMatPacket*);
};

//
class SDLModel : public J3DModel {
public:
	SDLModel(J3DModelData* model_data, u32 flags);
	SDLModel(SDLModelData* model_data, u32 flags, u32 mtx_num);

	virtual void viewCalcSimple();
	void entry();
	void entryModelDataSDL(SDLModelData* model_data, u32 flags, u32 mtx_num);

	// fabricated
	// used from MirrorActor.cpp
	SDLModelData* getSDLModelData() { return mSdlModelData; }

	u32 checkSdlFlag(u32 flag) const { return mSdlFlags & flag; }
	void onSdlFlag(u32 flag) { mSdlFlags |= flag; }
	void offSdlFlag(u32 flag) { mSdlFlags &= ~flag; }

public:
	enum {
		FLAG_UNK1 = 0x1,
		FLAG_UNK2 = 0x2,
		FLAG_UNK4 = 0x4,
		FLAG_UNK8 = 0x8,
	};

	/* 0xA0 */ SDLModelData* mSdlModelData;
	/* 0xA4 */ SDLModel* mNextSameMat;
	/* 0xA8 */ u32 mSdlFlags;
};

#endif
