#ifndef STRATEGIC_OBJ_MANAGER_HPP
#define STRATEGIC_OBJ_MANAGER_HPP

#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class THitActor;
class MActorAnmData;
class TModelDataKeeper;

struct TModelDataLoadEntry {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
};

class TObjChara : JDrama::TCharacter {
public:
	virtual ~TObjChara();
	virtual void load(JSUMemoryInputStream&);

	virtual void* getRes(const char*) const;

	// fabricated
	const char* getFolder() const { return mFolder; }

public:
	/* 0xC */ const char* mFolder;
};

class TObjManager : public JDrama::TViewObj {
public:
	TObjManager(const char*);

	virtual ~TObjManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual void createAnmData();
	virtual void createModelDataArray(const TModelDataLoadEntry*);

	TModelDataKeeper* getModelDataKeeper();
	void createModelDataArrayBase(const TModelDataLoadEntry*, const char*);
	MActorAnmData* getMActorAnmData();
	void manageObj(THitActor*);
	void initObjArray(int);

	// fabricated
	int objNum() { return unk14; }
	THitActor* getObj(int i) { return unk18[i]; }
	const THitActor* getObj(int i) const { return unk18[i]; }

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ s32 unk14;
	/* 0x18 */ THitActor** unk18;
	/* 0x1C */ TObjChara* unk1C;
	/* 0x20 */ MActorAnmData* unk20;
	/* 0x24 */ TModelDataKeeper* unk24;
	/* 0x28 */ s32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
};

#endif
