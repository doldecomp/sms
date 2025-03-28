#ifndef JPA_DRAW_HPP
#define JPA_DRAW_HPP

#include <dolphin/mtx.h>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/gx/GXEnum.h>

class JPABaseEmitter;
class JPATextureResource;
class JPABaseParticle;

class JPADraw {
public:
	/* 0x0 */ char unk0[0xC4];

public:
	class JPADrawVisitorDefFlags;

	void initialize(JPABaseEmitter*, JPATextureResource*);
	void draw(MtxPtr);
	void calc();
	void calcParticle(JPABaseParticle*);
	void calcChild(JPABaseParticle*);
	void initParticle(JPABaseParticle*);
	void initChild(JPABaseParticle*, JPABaseParticle*);
	void swapImage(const ResTIMG*, short);
	void loadTexture(unsigned char, GXTexMapID);
	void setDrawExecVisitorsBeforeCB(const JPADraw::JPADrawVisitorDefFlags&);
	void setDrawExecVisitorsAfterCB(const JPADraw::JPADrawVisitorDefFlags&);
	void setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags&);
	void setParticleClipBoard();
	void setChildClipBoard();
	void drawParticle();
	void drawChild();
	void zDraw();
	void zDrawParticle();
	void zDrawChild();
	void getMainTextureID(unsigned char);
	void getIndTextureID();
	void getIndSubTextureID();
	void getSecondTextureID();
	void loadYBBMtx(MtxPtr);
};

class JPADrawClipBoard {
public:
	~JPADrawClipBoard();
};

class JPADrawVisitorContainer {
public:
	~JPADrawVisitorContainer();
};

#endif
