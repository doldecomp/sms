#ifndef M3DUTIL_M_ACTOR_DATA_HPP
#define M3DUTIL_M_ACTOR_DATA_HPP

class J3DModelData;

class MActorAnmData {
public:
	MActorAnmData();
	~MActorAnmData();

	void createSampleModelData(J3DModelData*);
	void addFileTable(const char*);
	void getSimpleName(const char*);
	void addFileNum(const char*);
	void init(const char*, const char**);
	void addIncidentalAnm(const char*, int);
	void partsNameToIdx(const char*);

public:
	/* 0x0 */ char unk0[0x4c];
};

#endif
