#include <JSystem/JParticle/JPADrawVisitor.hpp>
#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAExtraShape.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include <macros.h>

JPADrawClipBoard* JPADrawContext::pcb;

// from tww, fabricated?
static inline u32 JPA_U8_THRE(u32 a, u32 b)
{
	return ((a * (b + 1)) * 0x10000) >> 24;
}

void JPADrawExecLoadExTex::exec(const JPADrawContext* dc)
{
	GXTexCoordID coord = GX_TEXCOORD1;
	GXTexMapID mapId   = GX_TEXMAP1;
	switch (dc->mExTexShape->getIndTexMode()) {
	case 1: {
		GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
		dc->mTexResource->load(
		    dc->mTexIndices[dc->mExTexShape->getIndTextureID()], GX_TEXMAP1);
		coord = GX_TEXCOORD2;
		mapId = GX_TEXMAP2;
		break;
	}
	case 2: {
		GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
		GXEnableTexOffsets(GX_TEXCOORD2, GX_TRUE, GX_TRUE);
		dc->mTexResource->load(
		    dc->mTexIndices[dc->mExTexShape->getIndTextureID()], GX_TEXMAP1);
		dc->mTexResource->load(
		    dc->mTexIndices[dc->mExTexShape->getSubTextureID()], GX_TEXMAP2);
		coord = GX_TEXCOORD3;
		mapId = GX_TEXMAP3;
		break;
	}
	}

	if (dc->mExTexShape->isEnableSecondTex()) {
		GXSetTexCoordGen(coord, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(coord, GX_TRUE, GX_TRUE);
		dc->mTexResource->load(
		    dc->mTexIndices[dc->mExTexShape->getSecondTexIndex()], mapId);
	}
}

void JPADrawExecGenPrjMtx::exec(const JPADrawContext* dc)
{
	Mtx mtx;
	C_MTXLightPerspective(mtx, dc->mBaseEmitter->getFovy(),
	                      dc->mBaseEmitter->getAspect(), 0.5f, -0.5f, 0.5f,
	                      0.5f);
	MTXConcat(mtx, dc->pcb->unk68, mtx);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX3x4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecGenPrjTexMtx::exec(const JPADrawContext* dc)
{
	Mtx projMtx;
	C_MTXLightPerspective(projMtx, dc->mBaseEmitter->getFovy(),
	                      dc->mBaseEmitter->getAspect(), 0.5f, -0.5f, 0.5f,
	                      0.5f);

	f32 tick   = dc->mBaseEmitter->unk10.getFrame();
	f32 transX = tick * dc->mBaseShape->getTexScrollTransX()
	             + dc->mBaseShape->getTexStaticTransX();
	f32 transY = tick * dc->mBaseShape->getTexScrollTransY()
	             + dc->mBaseShape->getTexStaticTransY();
	f32 scaleX = tick * dc->mBaseShape->getTexScrollScaleX()
	             + dc->mBaseShape->getTexStaticScaleX();
	f32 scaleY = tick * dc->mBaseShape->getTexScrollScaleY()
	             + dc->mBaseShape->getTexStaticScaleY();
	s16 angle = tick * dc->mBaseShape->getTexScrollRotate() * 32768;
	f32 sin   = JMASSin(angle);
	f32 cos   = JMASCos(angle);

	Mtx mtx;
	mtx[0][0] = scaleX * cos;
	mtx[0][1] = -scaleX * sin;
	mtx[0][2] = scaleX * (sin * (transY + 0.5f) - cos * (transX + 0.5f)) + 0.5f;
	mtx[0][3] = 0.0f;

	mtx[1][0] = scaleY * sin;
	mtx[1][1] = scaleY * cos;
	mtx[1][2]
	    = -scaleY * (sin * (transX + 0.5f) + cos * (transY + 0.5f)) + 0.5f;
	mtx[1][3] = 0.0f;

	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[2][3] = 0.0f;

	MTXConcat(mtx, projMtx, projMtx);
	MTXConcat(projMtx, dc->pcb->unk68, mtx);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX3x4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecGenTexMtx0::exec(const JPADrawContext* dc)
{
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecGenIdtMtx::exec(const JPADrawContext*)
{
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecSetTexMtx::exec(const JPADrawContext* dc)
{
	s32 tick = dc->mBaseEmitter->unk10.getFrame();

	f32 tilingX = 0.5f * dc->mBaseShape->getTilingX();
	f32 tilingY = 0.5f * dc->mBaseShape->getTilingY();
	f32 transX  = tick * dc->mBaseShape->getTexScrollTransX()
	             + dc->mBaseShape->getTexStaticTransX();
	f32 transY = tick * dc->mBaseShape->getTexScrollTransY()
	             + dc->mBaseShape->getTexStaticTransY();
	f32 scaleX = tick * dc->mBaseShape->getTexScrollScaleX()
	             + dc->mBaseShape->getTexStaticScaleX();
	f32 scaleY = tick * dc->mBaseShape->getTexScrollScaleY()
	             + dc->mBaseShape->getTexStaticScaleY();
	s16 angle = tick * dc->mBaseShape->getTexScrollRotate() * 32768;
	f32 sin   = JMASSin(angle);
	f32 cos   = JMASCos(angle);

	Mtx mtx;
	mtx[0][0] = scaleX * cos;
	mtx[0][1] = -scaleX * sin;
	mtx[0][2] = 0.0f;
	mtx[0][3]
	    = tilingX
	      + scaleX * (sin * (tilingY + transY) - cos * (tilingX + transX));

	mtx[1][0] = scaleY * sin;
	mtx[1][1] = scaleY * cos;
	mtx[1][2] = 0.0f;
	mtx[1][3]
	    = tilingY
	      + -scaleY * (sin * (tilingX + transX) + cos * (tilingY + transY));

	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[2][3] = 0.0f;

	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecLoadDefaultTexture::exec(const JPADrawContext* dc)
{
	dc->mTexResource->loadDefaultTexture(GX_TEXMAP0);
}

void JPADrawExecLoadTexture::exec(const JPADrawContext* dc)
{
	dc->mTexResource->load(dc->unk14->unkC0, GX_TEXMAP0);
}

void JPADrawExecSetPointSize::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	GXSetPointSize(dc->pcb->unk4 * particle->getDrawParamPPtr()->unk10,
	               GX_TO_ONE);
}

void JPADrawExecSetLineWidth::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	GXSetLineWidth(dc->pcb->unk4 * particle->getDrawParamPPtr()->unk10,
	               GX_TO_ONE);
}

void JPADrawExecRegisterPrmColorAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = params->unk2C;
	prm.r       = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g       = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b       = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a       = params->unk20 * JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterPrmAlphaAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = dc->unk14->mPrmColor;
	prm.r       = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g       = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b       = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a       = params->unk20 * JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterEnvColorAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor env = params->unk30;
	env.r       = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g       = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b       = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterPrmCEnv::exec(const JPADrawContext* dc,
                                      JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = params->unk2C;
	GXColor env = params->unk30;

	prm.r = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	env.r = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterPrmAEnv::exec(const JPADrawContext* dc,
                                      JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = dc->unk14->mPrmColor;
	GXColor env = params->unk30;

	prm.r = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	env.r = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecSetTexMtx::exec(const JPADrawContext* dc,
                                JPABaseParticle* particle)
{
	s32 tick    = particle->getAge();
	f32 tilingX = 0.5f * dc->mBaseShape->getTilingX();
	f32 tilingY = 0.5f * dc->mBaseShape->getTilingY();
	f32 transX  = tick * dc->mBaseShape->getTexScrollTransX()
	             + dc->mBaseShape->getTexStaticTransX();
	f32 transY = tick * dc->mBaseShape->getTexScrollTransY()
	             + dc->mBaseShape->getTexStaticTransY();
	f32 scaleX = tick * dc->mBaseShape->getTexScrollScaleX()
	             + dc->mBaseShape->getTexStaticScaleX();
	f32 scaleY = tick * dc->mBaseShape->getTexScrollScaleY()
	             + dc->mBaseShape->getTexStaticScaleY();
	s16 angle = tick * dc->mBaseShape->getTexScrollRotate() * 32768;
	f32 sin   = JMASSin(angle);
	f32 cos   = JMASCos(angle);

	Mtx mtx;
	mtx[0][0] = scaleX * cos;
	mtx[0][1] = -scaleX * sin;
	mtx[0][2] = 0.0f;
	mtx[0][3] = tilingX
	            + (scaleX
	               * ((sin * (tilingY + transY)) - (cos * (tilingX + transX))));

	mtx[1][0] = scaleY * sin;
	mtx[1][1] = scaleY * cos;
	mtx[1][2] = 0.0f;
	mtx[1][3] = tilingY
	            + (-scaleY
	               * ((sin * (tilingX + transX)) + (cos * (tilingY + transY))));

	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = 1.0f;
	mtx[2][3] = 0.0f;

	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
}

void JPADrawExecLoadTexture::exec(const JPADrawContext* dc,
                                  JPABaseParticle* particle)
{
	dc->mTexResource->load(particle->getDrawParamPPtr()->unk3A, GX_TEXMAP0);
}

void JPADrawExecBillBoard::exec(const JPADrawContext* dc,
                                JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	f32 scaleX = particle->getDrawParamPPtr()->unk10;
	f32 scaleY = particle->getDrawParamPPtr()->unk14;

	f32 x1 = scaleX * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 x0 = scaleX * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = scaleY * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 y1 = scaleY * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec2<f32> offs[4];
	offs[0].set(-x0, y0);
	offs[1].set(x1, y0);
	offs[2].set(x1, -y1);
	offs[3].set(-x0, -y1);

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(dc->pcb->unk34, &pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecRotBillBoard::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec2<f32> offs[4];
	offs[0].set((x0 * cos - y0 * sin), (x0 * sin + y0 * cos));
	offs[1].set((x1 * cos - y0 * sin), (x1 * sin + y0 * cos));
	offs[2].set((x1 * cos - y1 * sin), (x1 * sin + y1 * cos));
	offs[3].set((x0 * cos - y1 * sin), (x0 * sin + y1 * cos));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(dc->pcb->unk34, &pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecYBillBoard::exec(const JPADrawContext* dc,
                                 JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	f32 scaleX = particle->getDrawParamPPtr()->unk10;
	f32 scaleY = particle->getDrawParamPPtr()->unk14;

	f32 x0 = scaleX * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = scaleY * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = scaleX * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = scaleY * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[4];
	offs[0].set(-x0, +y0, 0.0f);
	offs[1].set(+x1, +y0, 0.0f);
	offs[2].set(+x1, -y1, 0.0f);
	offs[3].set(-x0, -y1, 0.0f);

	MTXMultVecArray(dc->pcb->unk38, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVecSR(dc->pcb->unk34, &pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}
void JPADrawExecRotYBillBoard::exec(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = +params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[4];
	offs[0].set((x0 * cos - y0 * sin), (x0 * sin + y0 * cos), 0.0f);
	offs[1].set((x1 * cos - y0 * sin), (x1 * sin + y0 * cos), 0.0f);
	offs[2].set((x1 * cos - y1 * sin), (x1 * sin + y1 * cos), 0.0f);
	offs[3].set((x0 * cos - y1 * sin), (x0 * sin + y1 * cos), 0.0f);

	MTXMultVecArray(dc->pcb->unk38, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(dc->pcb->unk34, &pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void dirTypeVel(JPABaseParticle* particle, JPABaseEmitter*,
                JGeometry::TVec3<f32>& result)
{
	result.set(particle->mVelocity);
}

void dirTypePos(JPABaseParticle* particle, JPABaseEmitter*,
                JGeometry::TVec3<f32>& result)
{
	particle->getLocalPosition(result);
}

void dirTypePosInv(JPABaseParticle* particle, JPABaseEmitter*,
                   JGeometry::TVec3<f32>& result)
{
	particle->getLocalPosition(result);
	result.negate();
}

void dirTypeEmtrDir(JPABaseParticle*, JPABaseEmitter* emitter,
                    JGeometry::TVec3<f32>& result)
{
	result.set(emitter->mEmitterDirection);
}

void dirTypePrevPtcl(JPABaseParticle* particle, JPABaseEmitter* emitter,
                     JGeometry::TVec3<f32>& result)
{
	JGeometry::TVec3<f32> pos;
	particle->getGlobalPosition(pos);
	JSULink<JPABaseParticle>* prev = particle->getLinkBufferPtr()->getPrev();
	if (prev != nullptr)
		prev->getObject()->getGlobalPosition(result);
	else
		emitter->getEmitterGlobalTranslation(result);

	result -= pos;
}

void rotTypeY(f32 sin, f32 cos, Mtx& result)
{
	result[0][0] = cos;
	result[0][1] = 0.0f;
	result[0][2] = -sin;
	result[0][3] = 0.0f;

	result[1][0] = 0.0f;
	result[1][1] = 1.0f;
	result[1][2] = 0.0f;
	result[1][3] = 0.0f;

	result[2][0] = sin;
	result[2][1] = 0.0f;
	result[2][2] = cos;
	result[2][3] = 0.0f;
}

void rotTypeX(f32 sin, f32 cos, Mtx& out)
{
	out[0][0] = 1.0f;
	out[0][1] = 0.0f;
	out[0][2] = 0.0f;
	out[0][3] = 0.0f;

	out[1][0] = 0.0f;
	out[1][1] = cos;
	out[1][2] = -sin;
	out[1][3] = 0.0f;

	out[2][0] = 0.0f;
	out[2][1] = sin;
	out[2][2] = cos;
	out[2][3] = 0.0f;
}

void rotTypeZ(f32 sin, f32 cos, Mtx& out)
{
	out[0][0] = cos;
	out[0][1] = -sin;
	out[0][2] = 0.0f;
	out[0][3] = 0.0f;

	out[1][0] = sin;
	out[1][1] = cos;
	out[1][2] = 0.0f;
	out[1][3] = 0.0f;

	out[2][0] = 0.0f;
	out[2][1] = 0.0f;
	out[2][2] = 1.0f;
	out[2][3] = 0.0f;
}

void rotTypeXYZ(f32 sin, f32 cos, Mtx& out)
{
	f32 d = (1.0f - cos) * 0.333333f;
	f32 a = d + cos;
	f32 b = d - (0.57735f * sin);
	f32 c = d + (0.57735f * sin);

	out[0][0] = a;
	out[0][1] = b;
	out[0][2] = c;
	out[0][3] = 0.0f;

	out[1][0] = c;
	out[1][1] = a;
	out[1][2] = b;
	out[1][3] = 0.0f;

	out[2][0] = b;
	out[2][1] = c;
	out[2][2] = a;
	out[2][3] = 0.0f;
}

void rotTypeYJiggle(f32 sin, f32 cos, Mtx& out)
{
	out[0][0] = cos;
	out[0][1] = -sin * 0.207912f;
	out[0][2] = -sin * 0.978148f;
	out[0][3] = 0.0f;

	out[1][0] = 0.0f;
	out[1][1] = 0.978148f;
	out[1][2] = -0.207912f;
	out[1][3] = 0.0f;

	out[2][0] = sin;
	out[2][1] = cos * 0.207912f;
	out[2][2] = cos * 0.978148f;
	out[2][3] = 0.0f;
}

void JPADrawExecDirectional::exec(const JPADrawContext* dc,
                                  JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = +params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[4];
	offs[0].set(x0, y0, 0.0f);
	offs[1].set(x1, y0, 0.0f);
	offs[2].set(x1, y1, 0.0f);
	offs[3].set(x0, y1, 0.0f);

	JGeometry::TVec3<f32> local_BC;
	dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_BC);
	if (local_BC.isZero())
		return;
	local_BC.normalize();

	JGeometry::TVec3<f32> f29_f30_f31;
	f29_f30_f31.cross(params->unk0, local_BC);
	if (f29_f30_f31.isZero())
		return;
	f29_f30_f31.normalize();

	params->unk0.cross(local_BC, f29_f30_f31);
	params->unk0.normalize();

	Mtx local_80;
	local_80[0][0] = params->unk0.x;
	local_80[0][1] = local_BC.x;
	local_80[0][2] = f29_f30_f31.x;
	local_80[0][3] = 0.0f;

	local_80[1][0] = params->unk0.y;
	local_80[1][1] = local_BC.y;
	local_80[1][2] = f29_f30_f31.y;
	local_80[1][3] = 0.0f;

	local_80[2][0] = params->unk0.z;
	local_80[2][1] = local_BC.z;
	local_80[2][2] = f29_f30_f31.z;
	local_80[2][3] = 0.0f;

	MTXMultVecArray(local_80, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecRotDirectional::exec(const JPADrawContext* dc,
                                     JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = +params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[4];
	offs[0].set(x0, y0, 0.0f);
	offs[1].set(x1, y0, 0.0f);
	offs[2].set(x1, y1, 0.0f);
	offs[3].set(x0, y1, 0.0f);

	Mtx local_120;
	dc->pcb->mRotTypeFunc(sin, cos, local_120);

	JGeometry::TVec3<f32> local_E4;
	dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_E4);
	if (local_E4.isZero())
		return;
	local_E4.normalize();

	JGeometry::TVec3<f32> f29_f30_f31;
	f29_f30_f31.cross(params->unk0, local_E4);
	if (f29_f30_f31.isZero())
		return;
	f29_f30_f31.normalize();

	params->unk0.cross(local_E4, f29_f30_f31);
	params->unk0.normalize();

	Mtx local_a8;
	local_a8[0][0] = params->unk0.x;
	local_a8[0][1] = local_E4.x;
	local_a8[0][2] = f29_f30_f31.x;
	local_a8[0][3] = 0.0f;

	local_a8[1][0] = params->unk0.y;
	local_a8[1][1] = local_E4.y;
	local_a8[1][2] = f29_f30_f31.y;
	local_a8[1][3] = 0.0f;

	local_a8[2][0] = params->unk0.z;
	local_a8[2][1] = local_E4.z;
	local_a8[2][2] = f29_f30_f31.z;
	local_a8[2][3] = 0.0f;

	Mtx local_78;
	MTXConcat(local_a8, local_120, local_78);
	MTXMultVecArray(local_78, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecDirectionalCross::exec(const JPADrawContext* dc,
                                       JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[8];
	offs[0].set(x0, y0, 0.0f);
	offs[1].set(x1, y0, 0.0f);
	offs[2].set(x1, y1, 0.0f);
	offs[3].set(x0, y1, 0.0f);
	offs[4].set((offs[1].x + offs[0].x) * 0.5f, y0,
	            (offs[1].x - offs[0].x) * 0.5f);
	offs[5].set((offs[1].x + offs[0].x) * 0.5f, y0,
	            (offs[0].x - offs[1].x) * 0.5f);
	offs[6].set((offs[1].x + offs[0].x) * 0.5f, y1,
	            (offs[0].x - offs[1].x) * 0.5f);
	offs[7].set((offs[1].x + offs[0].x) * 0.5f, y1,
	            (offs[1].x - offs[0].x) * 0.5f);

	JGeometry::TVec3<f32> local_BC;
	dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_BC);
	if (local_BC.isZero())
		return;
	local_BC.normalize();

	JGeometry::TVec3<f32> f29_f30_f31;
	f29_f30_f31.cross(params->unk0, local_BC);
	if (f29_f30_f31.isZero())
		return;
	f29_f30_f31.normalize();

	params->unk0.cross(local_BC, f29_f30_f31);
	params->unk0.normalize();

	Mtx local_80;
	local_80[0][0] = params->unk0.x;
	local_80[0][1] = local_BC.x;
	local_80[0][2] = f29_f30_f31.x;
	local_80[0][3] = 0.0f;

	local_80[1][0] = params->unk0.y;
	local_80[1][1] = local_BC.y;
	local_80[1][2] = f29_f30_f31.y;
	local_80[1][3] = 0.0f;

	local_80[2][0] = params->unk0.z;
	local_80[2][1] = local_BC.z;
	local_80[2][2] = f29_f30_f31.z;
	local_80[2][3] = 0.0f;

	MTXMultVecArray(local_80, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXPosition3f32(offs[4].x + pt.x, offs[4].y + pt.y, offs[4].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[5].x + pt.x, offs[5].y + pt.y, offs[5].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[6].x + pt.x, offs[6].y + pt.y, offs[6].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[7].x + pt.x, offs[7].y + pt.y, offs[7].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecRotDirectionalCross::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = +params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[8];
	offs[0].set(x0, y0, 0.0f);
	offs[1].set(x1, y0, 0.0f);
	offs[2].set(x1, y1, 0.0f);
	offs[3].set(x0, y1, 0.0f);
	offs[4].set((offs[1].x + offs[0].x) * 0.5f, y0,
	            (offs[1].x - offs[0].x) * 0.5f);
	offs[5].set((offs[1].x + offs[0].x) * 0.5f, y0,
	            (offs[0].x - offs[1].x) * 0.5f);
	offs[6].set((offs[1].x + offs[0].x) * 0.5f, y1,
	            (offs[0].x - offs[1].x) * 0.5f);
	offs[7].set((offs[1].x + offs[0].x) * 0.5f, y1,
	            (offs[1].x - offs[0].x) * 0.5f);

	Mtx local_180;
	dc->pcb->mRotTypeFunc(sin, cos, local_180);

	JGeometry::TVec3<f32> local_BC;
	dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_BC);
	if (local_BC.isZero())
		return;
	local_BC.normalize();

	JGeometry::TVec3<f32> f29_f30_f31;
	f29_f30_f31.cross(params->unk0, local_BC);
	if (f29_f30_f31.isZero())
		return;
	f29_f30_f31.normalize();

	params->unk0.cross(local_BC, f29_f30_f31);
	params->unk0.normalize();

	Mtx local_d8;
	local_d8[0][0] = params->unk0.x;
	local_d8[0][1] = local_BC.x;
	local_d8[0][2] = f29_f30_f31.x;
	local_d8[0][3] = 0.0f;

	local_d8[1][0] = params->unk0.y;
	local_d8[1][1] = local_BC.y;
	local_d8[1][2] = f29_f30_f31.y;
	local_d8[1][3] = 0.0f;

	local_d8[2][0] = params->unk0.z;
	local_d8[2][1] = local_BC.z;
	local_d8[2][2] = f29_f30_f31.z;
	local_d8[2][3] = 0.0f;

	Mtx local_a8;
	MTXConcat(local_d8, local_180, local_a8);
	MTXMultVecArray(local_a8, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXPosition3f32(offs[4].x + pt.x, offs[4].y + pt.y, offs[4].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[5].x + pt.x, offs[5].y + pt.y, offs[5].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[6].x + pt.x, offs[6].y + pt.y, offs[6].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[7].x + pt.x, offs[7].y + pt.y, offs[7].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecDirBillBoard::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	JGeometry::TVec3<f32> local_9C;
	dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_9C);
	JGeometry::TVec3<f32> dir(dc->pcb->unk34[0][1], dc->pcb->unk34[1][1],
	                          dc->pcb->unk34[2][1]);

	local_9C.cross(local_9C, dir);
	if (local_9C.isZero())
		return;
	local_9C.normalize();

	MTXMultVecSR(dc->pcb->unk34, &local_9C, &local_9C);

	f32 x0 = -(params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC));
	f32 y0 = +(params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10));
	f32 x1 = +(params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC));
	f32 y1 = -(params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10));

	JGeometry::TVec2<f32> offs[4];
	offs[0].set(x0, y0);
	offs[1].set(x1, y0);
	offs[2].set(x1, y1);
	offs[3].set(x0, y1);

	f32 x = local_9C.x;
	f32 y = local_9C.y;

	offs[0].set(x * offs[0].x - y * offs[0].y, x * offs[0].y + y * offs[0].x);
	offs[1].set(x * offs[1].x - y * offs[1].y, x * offs[1].y + y * offs[1].x);
	offs[2].set(x * offs[2].x - y * offs[2].y, x * offs[2].y + y * offs[2].x);
	offs[3].set(x * offs[3].x - y * offs[3].y, x * offs[3].y + y * offs[3].x);

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(dc->pcb->unk34, &pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecRotation::exec(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 x1 = params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y0 = params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	JGeometry::TVec3<f32> offs[4];
	offs[0].set(x0, y0, 0.0f);
	offs[1].set(x1, y0, 0.0f);
	offs[2].set(x1, y1, 0.0f);
	offs[3].set(x0, y1, 0.0f);

	Mtx mtx;
	dc->pcb->mRotTypeFunc(sin, cos, mtx);
	MTXMultVecArray(mtx, offs, offs, ARRAY_COUNT(offs));

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(offs[0].x + pt.x, offs[0].y + pt.y, offs[0].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(offs[1].x + pt.x, offs[1].y + pt.y, offs[1].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(offs[2].x + pt.x, offs[2].y + pt.y, offs[2].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(offs[3].x + pt.x, offs[3].y + pt.y, offs[3].z + pt.z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecRotationCross::exec(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
	f32 y0 = +params->unk14 * (dc->pcb->unk8 + dc->pcb->unk10);
	f32 x1 = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);
	f32 y1 = -params->unk14 * (dc->pcb->unk8 - dc->pcb->unk10);

	Mtx rotMtx;
	JGeometry::TVec3<f32> pt[8];

	pt[0].set(x0, y0, 0.0f);
	pt[1].set(x1, y0, 0.0f);
	pt[2].set(x1, y1, 0.0f);
	pt[3].set(x0, y1, 0.0f);

	pt[4].set((pt[1].x + pt[0].x) * 0.5f, y0, (pt[1].x - pt[0].x) * 0.5f);
	pt[5].set((pt[1].x + pt[0].x) * 0.5f, y0, (pt[0].x - pt[1].x) * 0.5f);
	pt[6].set((pt[1].x + pt[0].x) * 0.5f, y1, (pt[0].x - pt[1].x) * 0.5f);
	pt[7].set((pt[1].x + pt[0].x) * 0.5f, y1, (pt[1].x - pt[0].x) * 0.5f);

	dc->pcb->mRotTypeFunc(sin, cos, rotMtx);

	MTXMultVecArray(rotMtx, pt, pt, ARRAY_COUNT(pt));
	f32 x = particle->mGlobalPosition.x;
	f32 y = particle->mGlobalPosition.y;
	f32 z = particle->mGlobalPosition.z;

	GXBegin(GX_QUADS, GX_VTXFMT0, 8);
	GXPosition3f32(pt[0].x + x, pt[0].y + y, pt[0].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(pt[1].x + x, pt[1].y + y, pt[1].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(pt[2].x + x, pt[2].y + y, pt[2].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(pt[3].x + x, pt[3].y + y, pt[3].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXPosition3f32(pt[4].x + x, pt[4].y + y, pt[4].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[0].x, dc->pcb->mTexCoords[0].y);
	GXPosition3f32(pt[5].x + x, pt[5].y + y, pt[5].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[1].x, dc->pcb->mTexCoords[1].y);
	GXPosition3f32(pt[6].x + x, pt[6].y + y, pt[6].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[2].x, dc->pcb->mTexCoords[2].y);
	GXPosition3f32(pt[7].x + x, pt[7].y + y, pt[7].z + z);
	GXTexCoord2f32(dc->pcb->mTexCoords[3].x, dc->pcb->mTexCoords[3].y);
	GXEnd();
}

void JPADrawExecPoint::exec(const JPADrawContext* dc, JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JGeometry::TVec3<f32> pos;
	particle->getGlobalPosition(pos);
	GXBegin(GX_POINTS, GX_VTXFMT0, 1);
	GXPosition3f32(pos.x, pos.y, pos.z);
	GXTexCoord2f32(0.0f, 0.0f);
	GXEnd();
}

void JPADrawExecLine::exec(const JPADrawContext* dc, JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JGeometry::TVec3<f32> local_40;
	JGeometry::TVec3<f32> f31_f30_f39;
	particle->getGlobalPosition(f31_f30_f39);
	local_40 = particle->mVelocity;
	if (local_40.isZero())
		return;

	local_40.normalize();

	local_40 *= dc->pcb->unk8 * particle->getDrawParamPPtr()->unk14;

	local_40.sub(f31_f30_f39, local_40);

	GXBegin(GX_LINES, GX_VTXFMT0, 2);
	GXPosition3f32(f31_f30_f39.x, f31_f30_f39.y, f31_f30_f39.z);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32(local_40.x, local_40.y, local_40.z);
	GXTexCoord2f32(0.0f, 1.0f);
	GXEnd();
}

JSULink<JPABaseParticle>* stripeGetNext(JSULink<JPABaseParticle>* link)
{
	return link->getNext();
}
JSULink<JPABaseParticle>* stripeGetPrev(JSULink<JPABaseParticle>* link)
{
	return link->getPrev();
}
void JPADrawExecStripe::exec(const JPADrawContext* dc)
{
	u32 elems = dc->unk18->getNumLinks();
	if (elems < 2)
		return;

	typedef JSULink<JPABaseParticle>* (*NxtFunc)(JSULink<JPABaseParticle>*);

	NxtFunc getNext;
	JSULink<JPABaseParticle>* start;

	f32 fVar2 = 0.0f;
	f32 fVar9 = 1.0f / (elems - 1);
	if (!(dc->mBaseShape->mFlags & 0x1)) {
		start   = dc->unk18->getFirst();
		getNext = stripeGetNext;
	} else {
		fVar2   = 1.0f;
		fVar9   = -fVar9;
		start   = dc->unk18->getLast();
		getNext = stripeGetPrev;
	}

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, elems * 2);
	for (JSULink<JPABaseParticle>* link = start; link != nullptr;
	     link                           = getNext(link), fVar2 += fVar9) {
		JPABaseParticle* particle = link->getObject();

		JPADrawParams* params = particle->getDrawParamPPtr();

		f32 sin = JMASSin(params->unk34);
		f32 cos = JMASCos(params->unk34);

		f32 x = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
		f32 y = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);

		JGeometry::TVec3<f32> v1(x * sin, x * cos, 0.0f);
		JGeometry::TVec3<f32> v2(y * sin, y * cos, 0.0f);

		JGeometry::TVec3<f32> pt0;
		particle->getGlobalPosition(pt0);

		JGeometry::TVec3<f32> local_BC;
		dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_BC);
		if (local_BC.isZero())
			local_BC.set(0.0f, 1.0f, 0.0f);
		else
			local_BC.normalize();

		JGeometry::TVec3<f32> f29_f30_f31;
		f29_f30_f31.cross(params->unk0, local_BC);
		if (f29_f30_f31.isZero())
			f29_f30_f31.set(0.0f, 1.0f, 0.0f);
		else
			f29_f30_f31.normalize();

		params->unk0.cross(local_BC, f29_f30_f31);
		params->unk0.normalize();

		JGeometry::TVec3<f32> u1(params->unk0.x, f29_f30_f31.x, local_BC.x);
		JGeometry::TVec3<f32> u2(params->unk0.y, f29_f30_f31.y, local_BC.y);
		JGeometry::TVec3<f32> u3(params->unk0.z, f29_f30_f31.z, local_BC.z);
		(void)&u1;
		(void)&u2;
		(void)&u3;

		GXPosition3f32(pt0.x + v1.dot(u1), pt0.y + v1.dot(u2),
		               pt0.z + v1.dot(u3));
		GXTexCoord2f32(0.0f, fVar2);
		GXPosition3f32(pt0.x + v2.dot(u1), pt0.y + v2.dot(u2),
		               pt0.z + v2.dot(u3));
		GXTexCoord2f32(1.0f, fVar2);
	}
	GXEnd();
}

void JPADrawExecStripeCross::exec(const JPADrawContext* dc)
{

	u32 elems = dc->unk18->getNumLinks();
	if (elems < 2)
		return;

	typedef JSULink<JPABaseParticle>* (*NxtFunc)(JSULink<JPABaseParticle>*);

	NxtFunc getNext;
	JSULink<JPABaseParticle>* start;

	f32 fVar2;
	f32 fVar9 = 1.0f / (elems - 1);
	if (!(dc->mBaseShape->mFlags & 0x1)) {
		start   = dc->unk18->getFirst();
		fVar2   = 0.0f;
		getNext = &stripeGetNext;
	} else {
		fVar9   = -fVar9;
		start   = dc->unk18->getLast();
		fVar2   = 1.0f;
		getNext = &stripeGetPrev;
	}

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, elems * 2);
	for (JSULink<JPABaseParticle>* link = start; link != nullptr;
	     link                           = getNext(link), fVar2 += fVar9) {
		JPABaseParticle* particle = link->getObject();

		JPADrawParams* params = particle->getDrawParamPPtr();

		f32 sin = JMASSin(params->unk34);
		f32 cos = JMASCos(params->unk34);

		f32 x = -params->unk10 * (dc->pcb->unk4 + dc->pcb->unkC);
		f32 y = +params->unk10 * (dc->pcb->unk4 - dc->pcb->unkC);

		JGeometry::TVec3<f32> v1(x * sin, x * cos, 0.0f);
		JGeometry::TVec3<f32> v2(y * sin, y * cos, 0.0f);

		JGeometry::TVec3<f32> pt0;
		particle->getGlobalPosition(pt0);

		JGeometry::TVec3<f32> local_BC;
		dc->pcb->mDirTypeFunc(particle, dc->mBaseEmitter, local_BC);
		if (local_BC.isZero())
			local_BC.set(0.0f, 1.0f, 0.0f);
		else
			local_BC.normalize();

		JGeometry::TVec3<f32> f29_f30_f31;
		f29_f30_f31.cross(params->unk0, local_BC);
		if (f29_f30_f31.isZero())
			f29_f30_f31.set(0.0f, 1.0f, 0.0f);
		else
			f29_f30_f31.normalize();

		params->unk0.cross(local_BC, f29_f30_f31);
		params->unk0.normalize();

		JGeometry::TVec3<f32> u1(params->unk0.x, f29_f30_f31.x, local_BC.x);
		JGeometry::TVec3<f32> u2(params->unk0.y, f29_f30_f31.y, local_BC.y);
		JGeometry::TVec3<f32> u3(params->unk0.z, f29_f30_f31.z, local_BC.z);
		(void)&u1;
		(void)&u2;
		(void)&u3;

		GXPosition3f32(pt0.x + v1.dot(u1), pt0.y + v1.dot(u2),
		               pt0.z + v1.dot(u3));
		GXTexCoord2f32(0.0f, fVar2);
		GXPosition3f32(pt0.x + v2.dot(u1), pt0.y + v2.dot(u2),
		               pt0.z + v2.dot(u3));
		GXTexCoord2f32(1.0f, fVar2);
	}
	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, elems * 2);
	for (JSULink<JPABaseParticle>* link = start; link != nullptr;
	     link                           = getNext(link), fVar2 += fVar9) {
		JPABaseParticle* particle = link->getObject();

		JPADrawParams* params = particle->getDrawParamPPtr();

		JGeometry::TVec3<f32> pt0;
		particle->getGlobalPosition(pt0);

		f32 cos = JMASCos(params->unk34);
		f32 sin = -JMASSin(params->unk34);

		f32 x = -params->unk14 * (dc->pcb->unk4 + dc->pcb->unkC);
		f32 y = params->unk14 * (dc->pcb->unk4 - dc->pcb->unkC);

		JGeometry::TVec3<f32> local_BC;
		local_BC = particle->mVelocity;

		JGeometry::TVec3<f32> v1(x * sin, x * cos, 0.0f);
		JGeometry::TVec3<f32> v2(y * sin, y * cos, 0.0f);

		if (local_BC.isZero())
			local_BC.set(0.0f, 1.0f, 0.0f);
		else
			local_BC.normalize();

		JGeometry::TVec3<f32> f29_f30_f31;
		f29_f30_f31.cross(params->unk0, local_BC);
		if (f29_f30_f31.isZero())
			f29_f30_f31.set(0.0f, 1.0f, 0.0f);
		else
			f29_f30_f31.normalize();

		params->unk0.cross(local_BC, f29_f30_f31);
		params->unk0.normalize();

		JGeometry::TVec3<f32> u1(params->unk0.x, f29_f30_f31.x, local_BC.x);
		JGeometry::TVec3<f32> u2(params->unk0.y, f29_f30_f31.y, local_BC.y);
		JGeometry::TVec3<f32> u3(params->unk0.z, f29_f30_f31.z, local_BC.z);
		(void)&u1;
		(void)&u2;
		(void)&u3;

		GXPosition3f32(pt0.x + v1.dot(u1), pt0.y + v1.dot(u2),
		               pt0.z + v1.dot(u3));
		GXTexCoord2f32(0.0f, fVar2);
		GXPosition3f32(pt0.x + v2.dot(u1), pt0.y + v2.dot(u2),
		               pt0.z + v2.dot(u3));
		GXTexCoord2f32(1.0f, fVar2);
	}
	GXEnd();
}

void JPADrawExecRegisterColorEmitterPE::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->unk14->mPrmColor;
	GXColor env = dc->unk14->mEnvColor;

	prm.r = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a = JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	env.r = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterColorEmitterP::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->unk14->mPrmColor;

	prm.r = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a = JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterColorEmitterE::exec(const JPADrawContext* dc)
{
	GXColor env = dc->unk14->mEnvColor;

	env.r = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterColorChildPE::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->mSweepShape->getPrm();
	GXColor env = dc->mSweepShape->getEnv();

	prm.r = JPA_U8_THRE(prm.r, dc->pcb->mPrmColor.r);
	prm.g = JPA_U8_THRE(prm.g, dc->pcb->mPrmColor.g);
	prm.b = JPA_U8_THRE(prm.b, dc->pcb->mPrmColor.b);
	prm.a = JPA_U8_THRE(prm.a, dc->pcb->mPrmColor.a);
	env.r = JPA_U8_THRE(env.r, dc->pcb->mEnvColor.r);
	env.g = JPA_U8_THRE(env.g, dc->pcb->mEnvColor.g);
	env.b = JPA_U8_THRE(env.b, dc->pcb->mEnvColor.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawCalcColorPrm::calc(const JPADrawContext* dc)
{
	dc->unk14->mPrmColor = dc->mBaseShape->getPrmColor(dc->pcb->mColorAnmFrame);
}

void JPADrawCalcColorEnv::calc(const JPADrawContext* dc)
{
	dc->unk14->mEnvColor = dc->mBaseShape->getEnvColor(dc->pcb->mColorAnmFrame);
}

void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext* dc)
{
	s32 tick  = dc->mBaseEmitter->unk10.getFrame();
	s16 max   = dc->mBaseShape->getColorRegAnmMaxFrm();
	s32 frame = tick < max ? tick : max;

	dc->pcb->mColorAnmFrame = frame;
}

void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext* dc)
{
	f32 tick = dc->mBaseEmitter->unk10.getFrame();
	dc->pcb->mColorAnmFrame
	    = ((u32)tick) % (dc->mBaseShape->getColorRegAnmMaxFrm() + 1);
}

void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext* dc)
{
	s32 tick     = dc->mBaseEmitter->unk10.getFrame();
	s32 maxFrame = dc->mBaseShape->getColorRegAnmMaxFrm();
	s32 odd   = (tick / maxFrame) & 1; // whether we're on an even or odd loop
	s32 frame = tick % maxFrame;
	dc->pcb->mColorAnmFrame = frame + (odd * maxFrame) - 2 * (odd * frame);
}

void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext* dc)
{
	dc->pcb->mColorAnmFrame = 0;
}

void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext* dc)
{
	dc->pcb->mColorAnmFrame = 0;
}

void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext* dc)
{
	s32 tick         = dc->mBaseEmitter->unk10.getFrame();
	u8 anmKeyNum     = dc->mBaseShape->getTextureAnmKeyNum();
	s32 idx          = ((anmKeyNum - 1) < tick) ? anmKeyNum - 1 : tick;
	dc->unk14->unkC0 = dc->mTexIndices[dc->mBaseShape->getTextureIndex(idx)];
}

void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext* dc)
{
	s32 tick         = dc->mBaseEmitter->unk10.getFrame();
	dc->unk14->unkC0 = dc->mTexIndices[dc->mBaseShape->getTextureIndex(
	    tick % dc->mBaseShape->getTextureAnmKeyNum())];
}

void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext* dc)
{
	s32 tick     = dc->mBaseEmitter->unk10.getFrame();
	s32 maxFrame = dc->mBaseShape->getTextureAnmKeyNum() - 1;
	s32 odd   = (tick / maxFrame) & 1; // whether we're on an even or odd loop
	s32 frame = tick % maxFrame;
	dc->unk14->unkC0 = dc->mTexIndices[dc->mBaseShape->getTextureIndex(
	    frame + (odd * maxFrame) - 2 * (odd * frame))];
}

void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext* dc)
{
	dc->unk14->unkC0 = dc->mBaseShape->getTextureIndex();
}

void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext* dc)
{
	dc->unk14->unkC0 = dc->mBaseShape->getTextureIndex();
}

void JPADrawExecCallBack::exec(const JPADrawContext* dc)
{
	dc->mBaseEmitter->drawEmitterCallBack();
}

void JPADrawExecCallBack::exec(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->drawParticleCallBack(dc->mBaseEmitter);
}

void JPADrawCalcScaleX::calc(const JPADrawContext* dc,
                             JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	if (dc->pcb->mScaleAnmTimer < dc->mExtraShape->getScaleInTiming()) {
		params->unk10
		    = params->unkC
		      * ((dc->mExtraShape->getIncreaseRateX() * dc->pcb->mScaleAnmTimer)
		         + dc->mExtraShape->getScaleInValueX());
	} else if (dc->pcb->mScaleAnmTimer > dc->mExtraShape->getScaleOutTiming()) {
		params->unk10 = params->unkC
		                * ((dc->mExtraShape->getDecreaseRateX()
		                    * (dc->pcb->mScaleAnmTimer
		                       - dc->mExtraShape->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk10 = params->unkC;
	}
}

void JPADrawCalcScaleY::calc(const JPADrawContext* dc,
                             JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	if (dc->pcb->mScaleAnmTimer < dc->mExtraShape->getScaleInTiming()) {
		params->unk14
		    = params->unkC
		      * ((dc->mExtraShape->getIncreaseRateY() * dc->pcb->mScaleAnmTimer)
		         + dc->mExtraShape->getScaleInValueY());
	} else if (dc->pcb->mScaleAnmTimer > dc->mExtraShape->getScaleOutTiming()) {
		params->unk14 = params->unkC
		                * ((dc->mExtraShape->getDecreaseRateY()
		                    * (dc->pcb->mScaleAnmTimer
		                       - dc->mExtraShape->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk14 = params->unkC;
	}
}

void JPADrawCalcScaleXBySpeed::calc(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	JGeometry::TVec3<f32> vel = particle->mVelocity;

	if (dc->pcb->mScaleAnmTimer < dc->mExtraShape->getScaleInTiming()) {
		params->unk10
		    = params->unkC
		      * ((dc->mExtraShape->getIncreaseRateX() * dc->pcb->mScaleAnmTimer)
		         + dc->mExtraShape->getScaleInValueX());
	} else if (dc->pcb->mScaleAnmTimer > dc->mExtraShape->getScaleOutTiming()) {
		params->unk10 = params->unkC
		                * ((dc->mExtraShape->getDecreaseRateX()
		                    * (dc->pcb->mScaleAnmTimer
		                       - dc->mExtraShape->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk10 = params->unkC;
	}
	params->unk10 *= vel.length() * 0.01f;
}

void JPADrawCalcScaleYBySpeed::calc(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	JGeometry::TVec3<f32> vel = particle->mVelocity;

	if (dc->pcb->mScaleAnmTimer < dc->mExtraShape->getScaleInTiming()) {
		params->unk14
		    = params->unkC
		      * ((dc->mExtraShape->getIncreaseRateY() * dc->pcb->mScaleAnmTimer)
		         + dc->mExtraShape->getScaleInValueY());
	} else if (dc->pcb->mScaleAnmTimer > dc->mExtraShape->getScaleOutTiming()) {
		params->unk14 = params->unkC
		                * ((dc->mExtraShape->getDecreaseRateY()
		                    * (dc->pcb->mScaleAnmTimer
		                       - dc->mExtraShape->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk14 = params->unkC;
	}
	params->unk14 *= vel.length() * 0.01f;
}

void JPADrawCalcScaleCopyX2Y::calc(const JPADrawContext*,
                                   JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	params->unk14 = params->unk10;
}

void JPADrawCalcScaleAnmTimingNormal::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particle)
{
	dc->pcb->mScaleAnmTimer = particle->mLifeProgress;
}

void JPADrawCalcScaleAnmTimingRepeatX::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particle)
{
	dc->pcb->mScaleAnmTimer
	    = (particle->getAge() % dc->mExtraShape->getAnmCycleX())
	      / (f32)dc->mExtraShape->getAnmCycleX();
}

void JPADrawCalcScaleAnmTimingRepeatY::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particle)
{
	dc->pcb->mScaleAnmTimer
	    = (particle->getAge() % dc->mExtraShape->getAnmCycleY())
	      / (f32)dc->mExtraShape->getAnmCycleY();
}

void JPADrawCalcScaleAnmTimingReverseX::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particle)
{
	// whether we're on an even or odd loop
	f32 odd   = (particle->getAge() / dc->mExtraShape->getAnmCycleX()) & 1;
	f32 frame = (f32)(particle->getAge() % dc->mExtraShape->getAnmCycleX())
	            / dc->mExtraShape->getAnmCycleX();
	dc->pcb->mScaleAnmTimer = odd + (frame - odd * 2.0f * frame);
}

void JPADrawCalcScaleAnmTimingReverseY::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particle)
{
	// whether we're on an even or odd loop
	f32 odd   = (particle->getAge() / dc->mExtraShape->getAnmCycleY()) & 1;
	f32 frame = (f32)(particle->getAge() % dc->mExtraShape->getAnmCycleY())
	            / dc->mExtraShape->getAnmCycleY();
	dc->pcb->mScaleAnmTimer = odd + (frame - odd * 2.0f * frame);
}

void JPADrawCalcColorPrm::calc(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk2C
	    = dc->mBaseShape->getPrmColor(dc->pcb->mColorAnmFrame);
}

void JPADrawCalcColorEnv::calc(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk30
	    = dc->mBaseShape->getEnvColor(dc->pcb->mColorAnmFrame);
}

void JPADrawCalcColorCopyFromEmitter::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk2C = dc->unk14->mPrmColor;
	particle->getDrawParamPPtr()->unk30 = dc->unk14->mEnvColor;
}

void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame = (particle->getAge() < dc->mBaseShape->getColorRegAnmMaxFrm())
	                ? particle->getAge()
	                : dc->mBaseShape->getColorRegAnmMaxFrm();
	dc->pcb->mColorAnmFrame = frame;
}

void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame = ((particle->getDrawParamPPtr()->unk28
	              & dc->mBaseShape->getColLoopOffset())
	             + particle->getAge())
	            % (dc->mBaseShape->getColorRegAnmMaxFrm() + 1);
	dc->pcb->mColorAnmFrame = frame;
}

void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particle)
{
	s32 thing      = particle->getDrawParamPPtr()->unk28;
	s32 loopOffset = dc->mBaseShape->getColLoopOffset() & thing;
	s32 t          = loopOffset + particle->getAge();
	s32 maxFrame   = dc->mBaseShape->getColorRegAnmMaxFrm();
	s32 odd        = (t / maxFrame) & 1;
	s32 frame      = t % maxFrame;

	dc->pcb->mColorAnmFrame = frame + (odd * maxFrame) - 2 * (odd * frame);
}

void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext* dc,
                                         JPABaseParticle* particle)
{
	s32 start = particle->getDrawParamPPtr()->unk28
	            & dc->mBaseShape->getColLoopOffset();
	s32 maxFrame = dc->mBaseShape->getColorRegAnmMaxFrm() + 1;
	s32 frame    = (s32)(start + maxFrame * particle->mLifeProgress) % maxFrame;
	dc->pcb->mColorAnmFrame = frame;
}

void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame = (particle->getDrawParamPPtr()->unk28
	             & dc->mBaseShape->getColLoopOffset())
	            % (dc->mBaseShape->getColorRegAnmMaxFrm() + 1);
	dc->pcb->mColorAnmFrame = frame;
}

void JPADrawCalcAlpha::calc(const JPADrawContext* dc, JPABaseParticle* particle)
{
	f32 time = particle->mLifeProgress;
	f32 alpha;
	if (time < dc->mExtraShape->getAlphaInTiming()) {
		alpha = time * dc->mExtraShape->getAlphaIncreaseRate()
		        + dc->mExtraShape->getAlphaInValue();
	} else if (time > dc->mExtraShape->getAlphaOutTiming()) {
		alpha = dc->mExtraShape->getAlphaBaseValue()
		        + dc->mExtraShape->getAlphaDecreaseRate()
		              * (time - dc->mExtraShape->getAlphaOutTiming());
	} else {
		alpha = dc->mExtraShape->getAlphaBaseValue();
	}
	particle->getDrawParamPPtr()->unk20 = alpha;
}

void JPADrawCalcAlphaFlickNrmSin::calc(const JPADrawContext* dc,
                                       JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamPPtr();

	f32 sin = JMASSin((particles->getAge() * 16384) * params->unk24
	                  * (1.0f - dc->mExtraShape->getAlphaWaveParam1()));
	params->unk20
	    *= params->unk24
	           * (((sin - 1.0f) * 0.5f) * dc->mExtraShape->getAlphaWaveParam3())
	       + 1.0f;
	if (params->unk20 < 0.0f)
		params->unk20 = 0.0f;
}

void JPADrawCalcAlphaFlickAddSin::calc(const JPADrawContext* dc,
                                       JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamPPtr();

	f32 theta = (particles->getAge() * 16384) * params->unk24;

	// TODO: try to introduce temps without making this non-matching
	params->unk20
	    *= (params->unk24
	            * ((((JMASSin(theta
	                          * (1.0f - dc->mExtraShape->getAlphaWaveParam1()))
	                  + JMASSin(
	                      theta
	                      * (1.0f - dc->mExtraShape->getAlphaWaveParam2())))
	                 - 2.0f)
	                * 0.5f)
	               * dc->mExtraShape->getAlphaWaveParam3())
	        + 2.0f)
	       * 0.5f;
	if (params->unk20 < 0.0f)
		params->unk20 = 0.0f;
}

void JPADrawCalcAlphaFlickMultSin::calc(const JPADrawContext* dc,
                                        JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamPPtr();

	f32 theta = (particles->getAge() * 16384) * params->unk24;
	f32 mul3  = (dc->mExtraShape->getAlphaWaveParam3() * 0.5f) * params->unk24;

	// TODO: try to introduce temps without making this non-matching
	params->unk20
	    *= ((mul3
	         * (JMASSin(theta * (1.0f - dc->mExtraShape->getAlphaWaveParam1()))
	            - 1.0f))
	        + 1.0f)
	       * ((mul3
	           * (JMASSin(theta
	                      * (1.0f - dc->mExtraShape->getAlphaWaveParam2()))
	              - 1.0f))
	          + 1.0f);
	if (params->unk20 < 0.0f)
		params->unk20 = 0.0f;
}

void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	s32 idx = ((dc->mBaseShape->getTextureAnmKeyNum() - 1)
	           < (s32)particles->getAge())
	              ? (dc->mBaseShape->getTextureAnmKeyNum() - 1)
	              : particles->getAge();
	particles->getDrawParamPPtr()->unk3A
	    = dc->mTexIndices[dc->mBaseShape->getTextureIndex(idx)];
}

void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	particles->getDrawParamPPtr()->unk3A
	    = dc->mTexIndices[dc->mBaseShape->getTextureIndex(
	        ((particles->getDrawParamPPtr()->unk28
	          & dc->mBaseShape->getTexLoopOffset())
	         + particles->getAge())
	        % dc->mBaseShape->getTextureAnmKeyNum())];
}

void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particles)
{
	s32 loopOffset = particles->getDrawParamPPtr()->unk28
	                 & dc->mBaseShape->getTexLoopOffset();
	s32 t        = loopOffset + particles->getAge();
	s32 maxFrame = dc->mBaseShape->getTextureAnmKeyNum() - 1;
	s32 odd      = (t / maxFrame) & 1;
	s32 frame    = t % maxFrame;

	s32 id = frame + (odd * maxFrame) - 2 * (odd * frame);

	s32 tid = dc->mBaseShape->getTextureIndex(id);

	particles->getDrawParamPPtr()->unk3A = dc->mTexIndices[tid];
}

void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particles)
{
	s32 maxFrame = dc->mBaseShape->getTextureAnmKeyNum();
	s32 start    = particles->getDrawParamPPtr()->unk28
	            & dc->mBaseShape->getTexLoopOffset();
	s32 frame = (s32)(start + maxFrame * particles->mLifeProgress) % maxFrame;
	particles->getDrawParamPPtr()->unk3A
	    = dc->mTexIndices[dc->mBaseShape->getTextureIndex(frame)];
}

void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	s32 start = particles->getDrawParamPPtr()->unk28
	            & dc->mBaseShape->getTexLoopOffset();
	particles->getDrawParamPPtr()->unk3A
	    = dc->mTexIndices[dc->mBaseShape->getTextureIndex(
	        start % dc->mBaseShape->getTextureAnmKeyNum())];
}

void JPADrawCalcChildAlphaOut::calc(const JPADrawContext*,
                                    JPABaseParticle* particles)
{
	particles->getDrawParamCPtr()->unk20 = 1.0f - particles->mLifeProgress;
}

void JPADrawCalcChildScaleOut::calc(const JPADrawContext*,
                                    JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamCPtr();

	params->unk10 = params->unkC * (1.0f - particles->mLifeProgress);
	params->unk14 = params->unk24 * (1.0f - particles->mLifeProgress);
}
