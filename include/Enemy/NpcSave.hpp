#ifndef ENEMY_NPC_SAVE_HPP
#define ENEMY_NPC_SAVE_HPP

class TNpcSaveStageFarClip {
public:
	TNpcSaveStageFarClip();
};

class TNpcSaveNormal {
public:
	TNpcSaveNormal();
};

class TNpcSaveIndividual {
public:
	TNpcSaveIndividual(const char*);
};

class TNpcParams {
public:
	TNpcParams();

	/* 0x0 */ TNpcSaveStageFarClip* unk0;
	/* 0x4 */ TNpcSaveNormal* unk4;
	/* 0x8 */ TNpcSaveIndividual* unk8[29]; // TODO: is size right?
};

#endif
