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

	JGeometry::TVec3<f32> unk00;
	JGeometry::TVec3<f32> unk0C;
	f32 mScale;
	bool unk1C;
	bool unk1D;
	const TBGCheckData* mBGCheckData;
	JGeometry::TVec3<f32> mVtx[12];
	int mVtxCount;
};

class TBeamManager : public JDrama::TViewObj {
public:
	TBeamManager(const char* name = "ビームマネージャ");
	virtual ~TBeamManager() { };

	void setupMaterial();
	void drawAllBeam();
	void requestCone(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	                 f32, bool, bool, bool);
	virtual void perform(u32, JDrama::TGraphics*);

	TConeBeam mBeams[5];
	GXColor mColor;
	int mBeamVtxCount;
	int mBeamCount;
};

#endif /* BEAM_H */
