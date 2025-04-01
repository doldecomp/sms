#include <JSystem/JParticle/JPADrawVisitor.hpp>
#include <JSystem/JParticle/JPAExTexShape.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

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
	switch (dc->unk10->getIndTexMode()) {
	case 1: {
		GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
		u16 texIdx = dc->unk20[dc->unk10->getIndTextureID()];
		dc->unk1C->unk2C[texIdx]->load(GX_TEXMAP1);
		coord = GX_TEXCOORD2;
		mapId = GX_TEXMAP2;
		break;
	}
	case 2: {
		GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(GX_TEXCOORD1, GX_TRUE, GX_TRUE);
		GXEnableTexOffsets(GX_TEXCOORD2, GX_TRUE, GX_TRUE);
		u16 texIdx = dc->unk20[dc->unk10->getIndTextureID()];
		dc->unk1C->unk2C[texIdx]->load(GX_TEXMAP1);
		u16 texIdx2 = dc->unk20[dc->unk10->getSubTextureID()];
		dc->unk1C->unk2C[texIdx2]->load(GX_TEXMAP2);
		coord = GX_TEXCOORD3;
		mapId = GX_TEXMAP3;
		break;
	}
	}

	if (dc->unk10->isEnableSecondTex()) {
		GXSetTexCoordGen(coord, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GXEnableTexOffsets(coord, GX_TRUE, GX_TRUE);
		u16 texIdx = dc->unk20[dc->unk10->getSecondTexIndex()];
		dc->unk1C->unk2C[texIdx]->load(mapId);
	}
}

void JPADrawExecGenPrjMtx::exec(const JPADrawContext* dc)
{
	Mtx mtx;
	C_MTXLightPerspective(mtx, dc->unk0->getFovy(), dc->unk0->getAspect(), 0.5f,
	                      -0.5f, 0.5f, 0.5f);
	MTXConcat(mtx, JPADrawContext::pcb->unk68, mtx);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX3x4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_TRUE, GX_TRUE);
}

void JPADrawExecGenPrjTexMtx::exec(const JPADrawContext* dc)
{
	Mtx projMtx;
	C_MTXLightPerspective(projMtx, dc->unk0->getFovy(), dc->unk0->getAspect(),
	                      0.5f, -0.5f, 0.5f, 0.5f);

	f32 tick   = dc->unk0->unk10.getFrame();
	f32 transX = tick * dc->unk4->getTexScrollTransX()
	             + dc->unk4->getTexStaticTransX();
	f32 transY = tick * dc->unk4->getTexScrollTransY()
	             + dc->unk4->getTexStaticTransY();
	f32 scaleX = tick * dc->unk4->getTexScrollScaleX()
	             + dc->unk4->getTexStaticScaleX();
	f32 scaleY = tick * dc->unk4->getTexScrollScaleY()
	             + dc->unk4->getTexStaticScaleY();
	s32 angle = DEG_TO_RAD(tick * dc->unk4->getTexScrollRotate());
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
	MTXConcat(projMtx, JPADrawContext::pcb->unk68, mtx);
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

void JPADrawExecSetTexMtx::exec(const JPADrawContext* pDC)
{
	s32 tick    = pDC->unk0->unk10.getFrame();
	f32 tilingX = 0.5f * pDC->unk4->getTilingX();
	f32 tilingY = 0.5f * pDC->unk4->getTilingY();
	f32 transX  = tick * pDC->unk4->getTexScrollTransX()
	             + pDC->unk4->getTexStaticTransX();
	f32 transY = tick * pDC->unk4->getTexScrollTransY()
	             + pDC->unk4->getTexStaticTransY();
	f32 scaleX = tick * pDC->unk4->getTexScrollScaleX()
	             + pDC->unk4->getTexStaticScaleX();
	f32 scaleY = tick * pDC->unk4->getTexScrollScaleY()
	             + pDC->unk4->getTexStaticScaleY();
	s32 angle = DEG_TO_RAD(tick * pDC->unk4->getTexScrollRotate());
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
	dc->unk1C->loadDefaultTexture(GX_TEXMAP0);
}

void JPADrawExecLoadTexture::exec(const JPADrawContext* dc)
{
	dc->unk1C->load(dc->unk14->unkC0, GX_TEXMAP0);
}

void JPADrawExecSetPointSize::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	GXSetPointSize(JPADrawContext::pcb->unk4
	                   * particle->getDrawParamPPtr()->unk10,
	               GX_TO_ONE);
}

void JPADrawExecSetLineWidth::exec(const JPADrawContext* dc,
                                   JPABaseParticle* particle)
{
	GXSetLineWidth(JPADrawContext::pcb->unk4
	                   * particle->getDrawParamPPtr()->unk10,
	               GX_TO_ONE);
}

void JPADrawExecRegisterPrmColorAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = params->unk2C;
	prm.r       = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g       = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b       = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterPrmAlphaAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = dc->unk14->unkB8;
	prm.r       = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g       = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b       = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterEnvColorAnm::exec(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor env = params->unk30;
	env.r       = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g       = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b       = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterPrmCEnv::exec(const JPADrawContext* dc,
                                      JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = params->unk2C;
	GXColor env = params->unk30;

	prm.r = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	env.r = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterPrmAEnv::exec(const JPADrawContext* dc,
                                      JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	GXColor prm = dc->unk14->unkB8;
	GXColor env = params->unk30;

	prm.r = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = params->unk20 * JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	env.r = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecSetTexMtx::exec(const JPADrawContext* dc,
                                JPABaseParticle* particle)
{
	s32 tick    = particle->getAge();
	f32 tilingX = 0.5f * dc->unk4->getTilingX();
	f32 tilingY = 0.5f * dc->unk4->getTilingY();
	f32 transX  = tick * dc->unk4->getTexScrollTransX()
	             + dc->unk4->getTexStaticTransX();
	f32 transY = tick * dc->unk4->getTexScrollTransY()
	             + dc->unk4->getTexStaticTransY();
	f32 scaleX = tick * dc->unk4->getTexScrollScaleX()
	             + dc->unk4->getTexStaticScaleX();
	f32 scaleY = tick * dc->unk4->getTexScrollScaleY()
	             + dc->unk4->getTexStaticScaleY();
	s32 angle = DEG_TO_RAD(tick * dc->unk4->getTexScrollRotate());
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
	dc->unk1C->load(particle->getDrawParamPPtr()->unk3A, GX_TEXMAP0);
}

void JPADrawExecBillBoard::exec(const JPADrawContext* dc,
                                JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	f32 scaleX = particle->getDrawParamPPtr()->unk10;
	f32 scaleY = particle->getDrawParamPPtr()->unk14;

	f32 x0 = scaleX * (JPADrawContext::pcb->unk4 - JPADrawContext::pcb->unkC);
	f32 y0 = scaleY * (JPADrawContext::pcb->unk8 - JPADrawContext::pcb->unk10);
	scaleX *= (JPADrawContext::pcb->unk4 + JPADrawContext::pcb->unkC);
	scaleY *= (JPADrawContext::pcb->unk8 + JPADrawContext::pcb->unk10);

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(*JPADrawContext::pcb->unk34, pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(-scaleX + pt.x, +scaleY + pt.y, pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[0].x,
	               JPADrawContext::pcb->unk14[0].y);
	GXPosition3f32(+x0 + pt.x, +scaleY + pt.y, pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[1].x,
	               JPADrawContext::pcb->unk14[1].y);
	GXPosition3f32(+x0 + pt.x, -y0 + pt.y, pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[2].x,
	               JPADrawContext::pcb->unk14[2].y);
	GXPosition3f32(-scaleX + pt.x, -y0 + pt.y, pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[3].x,
	               JPADrawContext::pcb->unk14[3].y);
	GXEnd();
}

void JPADrawExecRotBillBoard::exec(const JPADrawContext*,
                                   JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 scaleX = params->unk10;
	f32 scaleY = params->unk14;

	f32 x0 = -params->unk10
	         * (JPADrawContext::pcb->unk4 + JPADrawContext::pcb->unkC);
	f32 y0 = +params->unk14
	         * (JPADrawContext::pcb->unk8 + JPADrawContext::pcb->unk10);
	f32 x1 = +params->unk10
	         * (JPADrawContext::pcb->unk4 - JPADrawContext::pcb->unkC);
	f32 y1 = -params->unk14
	         * (JPADrawContext::pcb->unk8 - JPADrawContext::pcb->unk10);

	JGeometry::TVec3<f32> pt;
	particle->getGlobalPosition(pt);
	MTXMultVec(*JPADrawContext::pcb->unk34, pt, &pt);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32((x0 * cos - y0 * sin) + pt.x, (x0 * sin + y0 * cos) + pt.y,
	               pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[0].x,
	               JPADrawContext::pcb->unk14[0].y);
	GXPosition3f32((x1 * cos - y0 * sin) + pt.x, (x1 * sin + y0 * cos) + pt.y,
	               pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[1].x,
	               JPADrawContext::pcb->unk14[1].y);
	GXPosition3f32((x1 * cos - y1 * sin) + pt.x, (x1 * sin + y1 * cos) + pt.y,
	               pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[2].x,
	               JPADrawContext::pcb->unk14[2].y);
	GXPosition3f32((x0 * cos - y1 * sin) + pt.x, (x0 * sin + y1 * cos) + pt.y,
	               pt.z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[3].x,
	               JPADrawContext::pcb->unk14[3].y);
	GXEnd();
}

void JPADrawExecYBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotYBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }

void dirTypeVel(JPABaseParticle* particle, JPABaseEmitter*,
                JGeometry::TVec3<f32>& result)
{
	result.set(particle->unk38);
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
	result.set(emitter->unk210);
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

	result.x -= pos.x;
	result.y -= pos.y;
	result.z -= pos.z;
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
	f32 d = (1.0f - cos) * (1.0f / 3.0f);
	f32 c = d + (0.57735f * sin);
	f32 b = d - (0.57735f * sin);
	f32 a = d + cos;

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

void JPADrawExecDirectional::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotDirectional::exec(const JPADrawContext*, JPABaseParticle*) {
}
void JPADrawExecDirectionalCross::exec(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawExecRotDirectionalCross::exec(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawExecDirBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotation::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotationCross::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecPoint::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecLine::exec(const JPADrawContext*, JPABaseParticle*) { }
JSULink<JPABaseParticle>* stripeGetNext(JSULink<JPABaseParticle>* link)
{
	return link->getNext();
}
JSULink<JPABaseParticle>* stripeGetPrev(JSULink<JPABaseParticle>* link)
{
	return link->getPrev();
}
void JPADrawExecStripe::exec(const JPADrawContext*) { }
void JPADrawExecStripeCross::exec(const JPADrawContext*) { }

void JPADrawExecRegisterColorEmitterPE::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->unk14->unkB8;
	GXColor env = dc->unk14->unkBC;

	prm.r = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	env.r = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterColorEmitterP::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->unk14->unkB8;

	prm.r = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	GXSetTevColor(GX_TEVREG0, prm);
}

void JPADrawExecRegisterColorEmitterE::exec(const JPADrawContext* dc)
{
	GXColor env = dc->unk14->unkBC;

	env.r = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawExecRegisterColorChildPE::exec(const JPADrawContext* dc)
{
	GXColor prm = dc->unkC->getPrm();
	GXColor env = dc->unkC->getEnv();

	prm.r = JPA_U8_THRE(prm.r, JPADrawContext::pcb->unk98.r);
	prm.g = JPA_U8_THRE(prm.g, JPADrawContext::pcb->unk98.g);
	prm.b = JPA_U8_THRE(prm.b, JPADrawContext::pcb->unk98.b);
	prm.a = JPA_U8_THRE(prm.a, JPADrawContext::pcb->unk98.a);
	env.r = JPA_U8_THRE(env.r, JPADrawContext::pcb->unk9C.r);
	env.g = JPA_U8_THRE(env.g, JPADrawContext::pcb->unk9C.g);
	env.b = JPA_U8_THRE(env.b, JPADrawContext::pcb->unk9C.b);
	GXSetTevColor(GX_TEVREG0, prm);
	GXSetTevColor(GX_TEVREG1, env);
}

void JPADrawCalcColorPrm::calc(const JPADrawContext* dc)
{
	dc->unk14->unkB8 = dc->unk4->getPrmColor(JPADrawContext::pcb->unkAC);
}

void JPADrawCalcColorEnv::calc(const JPADrawContext* dc)
{
	dc->unk14->unkBC = dc->unk4->getEnvColor(JPADrawContext::pcb->unkAC);
}

void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext*) { }
void JPADrawExecCallBack::exec(const JPADrawContext*) { }
void JPADrawExecCallBack::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleX::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleY::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleXBySpeed::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleYBySpeed::calc(const JPADrawContext*, JPABaseParticle*) { }

void JPADrawCalcScaleCopyX2Y::calc(const JPADrawContext*,
                                   JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	params->unk14 = params->unk10;
}

void JPADrawCalcScaleAnmTimingNormal::calc(const JPADrawContext*,
                                           JPABaseParticle* particle)
{
	JPADrawContext::pcb->unkA8 = particle->unk48;
}

void JPADrawCalcScaleAnmTimingRepeatX::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingRepeatY::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingReverseX::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingReverseY::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcColorPrm::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcColorEnv::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcColorCopyFromEmitter::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext*,
                                         JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcAlpha::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcAlphaFlickNrmSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcAlphaFlickAddSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcAlphaFlickMultSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcChildAlphaOut::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcChildScaleOut::calc(const JPADrawContext*, JPABaseParticle*) { }
