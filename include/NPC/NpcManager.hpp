#ifndef NPC_MANAGER_HPP
#define NPC_MANAGER_HPP

#include <Enemy/EnemyManager.hpp>

class SDLModelData;
class J3DMaterialTable;

class TNPCManager : public TEnemyManager {
public:
	TNPCManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual J3DMaterialTable* getBmt_(bool);

	void makePartsModelData_(u32, u32, TModelDataKeeper*);
	void makeCommonPartsModelDataKeeper_(u32, const char*, TModelDataKeeper**);
	void makeOriginalPartsModelDataKeeper_(u32, u32);
	void changeTextureToPollution_(J3DModelData*);
	SDLModelData* getPartsSDLModelData(const char*) const;

public:
	// TODO: names inferred, not from the map. clipEnemies' low region needs
	// both of these reads to go through accessors.
	f32 getUnk54() const { return unk54; }
	const f32* getUnk58() const { return unk58; }

public:
	/* 0x54 */ f32 unk54;
	/* 0x58 */ const f32* unk58;
	/* 0x5C */ TModelDataKeeper* unk5C;
	/* 0x60 */ TModelDataKeeper* unk60;
};

class TBoardNpcManager : public TLiveManager {
public:
	TBoardNpcManager()
	    : TLiveManager("?")
	{
	}

	virtual void createModelData();
	virtual void clipActors(JDrama::TGraphics*);
};

class TMareJellyFishManager : public TObjManager {
public:
	TMareJellyFishManager(const char*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
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
	TMonteMSpecialManager()
	    : TMonteMBaseManager("?")
	{
	}

	void createAnmData();
};

class TMonteWSpecialManager : public TMonteWBaseManager {
public:
	TMonteWSpecialManager()
	    : TMonteWBaseManager("?")
	{
	}

	void createAnmData();
};

class TMonteMFManager : public TMonteMSpecialManager {
public:
	TMonteMFManager()
	    : TMonteMSpecialManager()
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteMGManager : public TMonteMSpecialManager {
public:
	TMonteMGManager()
	    : TMonteMSpecialManager()
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteMHManager : public TMonteMSpecialManager {
public:
	TMonteMHManager()
	    : TMonteMSpecialManager()
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMonteWCManager : public TMonteWSpecialManager {
public:
	TMonteWCManager()
	    : TMonteWSpecialManager()
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TMareMAManager : public TMareMBaseManager {
public:
	TMareMAManager()
	    : TMareMBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TMareMBManager : public TMareMBaseManager {
public:
	TMareMBManager()
	    : TMareMBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TMareMCManager : public TMareMBaseManager {
public:
	TMareMCManager()
	    : TMareMBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TMareMDManager : public TMareMBaseManager {
public:
	TMareMDManager()
	    : TMareMBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TMareWAManager : public TMareWBaseManager {
public:
	TMareWAManager()
	    : TMareWBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TMareWBManager : public TMareWBaseManager {
public:
	TMareWBManager()
	    : TMareWBaseManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
};

class TKinopioManager : public TNPCManager {
public:
	TKinopioManager()
	    : TNPCManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TKinojiiManager : public TNPCManager {
public:
	TKinojiiManager()
	    : TNPCManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TPeachManager : public TNPCManager {
public:
	TPeachManager()
	    : TNPCManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TRaccoonDogManager : public TNPCManager {
public:
	TRaccoonDogManager()
	    : TNPCManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TSunflowerLManager : public TNPCManager {
public:
	TSunflowerLManager()
	    : TNPCManager("?")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TSunflowerSManager : public TNPCManager {
public:
	TSunflowerSManager()
	    : TNPCManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMManager : public TMonteMBaseManager {
public:
	TMonteMManager()
	    : TMonteMBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMAManager : public TMonteMBaseManager {
public:
	TMonteMAManager()
	    : TMonteMBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMBManager : public TMonteMBaseManager {
public:
	TMonteMBManager()
	    : TMonteMBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMCManager : public TMonteMBaseManager {
public:
	TMonteMCManager()
	    : TMonteMBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMDManager : public TMonteMBaseManager {
public:
	TMonteMDManager()
	    : TMonteMBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteMEManager : public TMonteMSpecialManager {
public:
	TMonteMEManager()
	    : TMonteMSpecialManager()
	{
	}

	virtual void createModelData();
};

class TMonteWManager : public TMonteWBaseManager {
public:
	TMonteWManager()
	    : TMonteWBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteWAManager : public TMonteWBaseManager {
public:
	TMonteWAManager()
	    : TMonteWBaseManager("?")
	{
	}

	virtual void createModelData();
};

class TMonteWBManager : public TMonteWBaseManager {
public:
	TMonteWBManager()
	    : TMonteWBaseManager("?")
	{
	}

	virtual void createModelData();
};

#endif
