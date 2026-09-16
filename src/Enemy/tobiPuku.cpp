#include <Enemy/TobiPuku.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Animation slots shared by both variants.
enum {
	PUKU_ANM_ATTACK        = 0,
	PUKU_ANM_DEAD          = 1,
	PUKU_ANM_DOWN_AIR      = 2,
	PUKU_ANM_DOWN_LAND     = 3,
	PUKU_ANM_FALL          = 4,
	PUKU_ANM_FALL_END_LAND = 5,
	PUKU_ANM_JUMP          = 6,
	PUKU_ANM_JUMP_START    = 7,
	PUKU_ANM_PICHI         = 8,
	PUKU_ANM_SWIM          = 9,
};

void TTobiPuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }
void TTobiPuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }
void TTobiPuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }
void TTobiPuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }
void TTobiPuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }
void TTobiPuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }
void TTobiPuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }
void TTobiPuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}
void TTobiPuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }
void TTobiPuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

BOOL TTobiPuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? TRUE : FALSE; }
BOOL TTobiPuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }
BOOL TTobiPuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}
BOOL TTobiPuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? TRUE : FALSE; }
BOOL TTobiPuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? TRUE : FALSE;
}
bool TTobiPuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }

void TMoePuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }
void TMoePuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }
void TMoePuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }
void TMoePuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }
void TMoePuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }
void TMoePuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }
void TMoePuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }
void TMoePuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}
void TMoePuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }
void TMoePuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

BOOL TMoePuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? TRUE : FALSE; }
BOOL TMoePuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }
BOOL TMoePuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}
BOOL TMoePuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? TRUE : FALSE; }
BOOL TMoePuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? TRUE : FALSE;
}
bool TMoePuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }
