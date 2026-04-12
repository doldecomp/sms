#ifndef NPC_MANAGER_HPP
#define NPC_MANAGER_HPP

#include <Enemy/EnemyManager.hpp>

class SDLModelData;
class J3DMaterialTable;

class TNPCManager : public TEnemyManager {
public:
	TNPCManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual J3DMaterialTable* getBmt_(bool);

	void makePartsModelData_(u32, u32, TModelDataKeeper*);
	void makeCommonPartsModelDataKeeper_(u32, const char*, TModelDataKeeper**);
	void makeOriginalPartsModelDataKeeper_(u32, u32);
	void changeTextureToPollution_(J3DModelData*);
	SDLModelData* getPartsSDLModelData(const char*) const;

public:
	/* 0x54 */ f32 unk54;
	/* 0x58 */ const f32* unk58;
	/* 0x5C */ TModelDataKeeper* unk5C;
	/* 0x60 */ TModelDataKeeper* unk60;
};

class TBoardNpcManager : public TLiveManager {
public:
	virtual void createModelData();
	virtual void clipActors(JDrama::TGraphics*);
};

class TMareJellyFishManager : public TObjManager {
public:
	TMareJellyFishManager(const char*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
};

extern TMareJellyFishManager* gpMareJellyFishManager;

class TMonteMBaseManager : public TNPCManager {
public:
	TMonteMBaseManager(const char*);

	void changeTextureToStraw_(J3DModelData*);
	void createAnmData();

	static TModelDataKeeper* mStaticCommonKeeper;
};

class TMonteWBaseManager : public TNPCManager {
public:
	TMonteWBaseManager(const char*);

	void changeTextureToStraw_(J3DModelData*);
	void createAnmData();

	static TModelDataKeeper* mStaticCommonKeeper;
};

class TMareBaseManager : public TNPCManager {
public:
	TMareBaseManager(const char*);

	virtual J3DMaterialTable* getBmt_(bool);

	static J3DMaterialTable* mStaticBmtNormal;
	static J3DMaterialTable* mStaticBmtPollution;
};

class TMareMBaseManager : public TMareBaseManager {
public:
	TMareMBaseManager(const char*);

	virtual void createModelData();

	static TModelDataKeeper* mStaticCommonKeeper;
};

class TMareWBaseManager : public TMareBaseManager {
public:
	TMareWBaseManager(const char*);

	virtual void createModelData();

	static TModelDataKeeper* mStaticCommonKeeper;
};

class TMonteMSpecialManager : public TMonteMBaseManager {
public:
	TMonteMSpecialManager();

	void createAnmData();
};

class TMonteWSpecialManager : public TMonteWBaseManager {
public:
	TMonteWSpecialManager();

	void createAnmData();
};

class TMonteMFManager : public TMonteMSpecialManager {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteMGManager : public TMonteMSpecialManager {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteMHManager : public TMonteMSpecialManager {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteWCManager : public TMonteWSpecialManager {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMareMAManager : public TMareMBaseManager {
public:
	TMareMAManager();

	virtual void load(JSUMemoryInputStream&);
};

class TMareMBManager : public TMareMBaseManager {
public:
	TMareMBManager();

	virtual void load(JSUMemoryInputStream&);
};

class TMareMCManager : public TMareMBaseManager {
public:
	TMareMCManager();

	virtual void load(JSUMemoryInputStream&);
};

class TMareMDManager : public TMareMBaseManager {
public:
	TMareMDManager();

	virtual void load(JSUMemoryInputStream&);
};

class TMareWAManager : public TMareWBaseManager {
public:
	TMareWAManager();

	virtual void load(JSUMemoryInputStream&);
};

class TMareWBManager : public TMareWBaseManager {
public:
	TMareWBManager();

	virtual void load(JSUMemoryInputStream&);
};

class TKinopioManager : public TNPCManager {
public:
	TKinopioManager();

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TKinojiiManager : public TNPCManager {
public:
	TKinojiiManager();

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TPeachManager : public TNPCManager {
public:
	TPeachManager();

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TRaccoonDogManager : public TNPCManager {
public:
	TRaccoonDogManager();

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TSunflowerLManager : public TNPCManager {
public:
	TSunflowerLManager();

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TSunflowerSManager : public TNPCManager {
public:
	TSunflowerSManager();

	virtual void createModelData();
};

class TMonteMManager : public TMonteMBaseManager {
public:
	TMonteMManager();

	virtual void createModelData();
};

class TMonteMAManager : public TMonteMBaseManager {
public:
	TMonteMAManager();

	virtual void createModelData();
};

class TMonteMBManager : public TMonteMBaseManager {
public:
	TMonteMBManager();

	virtual void createModelData();
};

class TMonteMCManager : public TMonteMBaseManager {
public:
	TMonteMCManager();

	virtual void createModelData();
};

class TMonteMDManager : public TMonteMBaseManager {
public:
	TMonteMDManager();

	virtual void createModelData();
};

class TMonteMEManager : public TMonteMSpecialManager {
public:
	TMonteMEManager();

	virtual void createModelData();
};

class TMonteWManager : public TMonteWBaseManager {
public:
	TMonteWManager();

	virtual void createModelData();
};

class TMonteWAManager : public TMonteWBaseManager {
public:
	TMonteWAManager();

	virtual void createModelData();
};

class TMonteWBManager : public TMonteWBaseManager {
public:
	TMonteWBManager();

	virtual void createModelData();
};

#endif
