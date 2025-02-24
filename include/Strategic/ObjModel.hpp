#ifndef STRATEGIC_OBJ_MODEL_HPP
#define STRATEGIC_OBJ_MODEL_HPP

#include <dolphin/types.h>

struct TModelDataLoadEntry {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
};

class TModelDataKeeper {
public:
	TModelDataKeeper(const char*);
	void getModelDataNum() const;
	void getDataByName(const char*) const;
	void getIndex(const char*) const;
	void getNthData(int) const;
	void createAndKeepData(const char*, unsigned long);
	void loadModelData(const char*, unsigned long, const char*);

public:
	/* 0x0 */ char unk0[0x14];
};

#endif
