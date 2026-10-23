#ifndef BEAM_H
#define BEAM_H

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Strategic/Strategy.hpp>
#include <Map/MapData.hpp>

class TConeBeam {
public:
	TConeBeam();
	void calcVertices(int count);
	void drawConeBeam(const GXColor& color);
	void drawConeBeamAux(const GXColor& color, bool unk);

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk00;
	/* 0xC */ JGeometry::TVec3<f32> unk0C;
	/* 0x18 */ f32 mScale;
	/* 0x1C */ bool unk1C;
	/* 0x1D */ bool unk1D;
	/* 0x20 */ const TBGCheckData* mBGCheckData;
	/* 0x24 */ JGeometry::TVec3<f32> mVtx[12];
	/* 0xB4 */ int mVtxCount;
};

class TBeamManager : public JDrama::TViewObj {
public:
	TBeamManager(const char* name = "ビームマネージャ");
	virtual ~TBeamManager() { };

	void setupMaterial();
	void drawAllBeam();
	void requestCone(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	                 f32, bool, bool, bool);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

private:
	/* 0x10 */ TConeBeam mBeams[5];
	/* 0x3A8 */ GXColor mColor;
	/* 0x3AC */ int mBeamVtxCount;
	/* 0x3B0 */ int mBeamCount;
};

extern TBeamManager* gpBeamManager;

#endif /* BEAM_H */
