#ifndef JPA_DRAW_SETUP_TEV_HPP
#define JPA_DRAW_SETUP_TEV_HPP

#include <dolphin/types.h>

class JPABaseShape;
class JPAExTexShape;

class JPADrawSetupTev {
public:
	void setupTev(JPABaseShape*, JPAExTexShape*);
	~JPADrawSetupTev() { }

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ u8 unk2;
};

#endif
