#ifndef STRATEGIC_OBJ_MANAGER_HPP
#define STRATEGIC_OBJ_MANAGER_HPP

#include <JSystem/JDrama/JDRCharacter.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class THitActor;
class MActorAnmData;
class TModelDataKeeper;
struct TModelDataLoadEntry;

class TObjManager : JDrama::TViewObj {
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

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ s32 unk14;
	/* 0x18 */ THitActor** unk18;
	/* 0x1C */ TNameRef* unk1C; // TODO: type should be derived
	/* 0x20 */ MActorAnmData* unk20;
	/* 0x24 */ TModelDataKeeper* unk24;
	/* 0x28 */ s32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
};

class TObjChara : JDrama::TCharacter {
public:
	virtual ~TObjChara();
	virtual void load(JSUMemoryInputStream&);

	virtual void* getRes(const char*) const;
};

#endif
