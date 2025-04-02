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

void JPADrawExecSetTexMtx::exec(const JPADrawContext* dc)
{
	s32 tick    = dc->unk0->unk10.getFrame();
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

void JPADrawExecRotation::exec(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
}

void JPADrawExecRotationCross::exec(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JPADrawParams* params = particle->getDrawParamPPtr();

	f32 sin = JMASSin(params->unk34);
	f32 cos = JMASCos(params->unk34);

	f32 x0 = -params->unk10
	         * (JPADrawContext::pcb->unk4 + JPADrawContext::pcb->unkC);
	f32 y0 = +params->unk14
	         * (JPADrawContext::pcb->unk8 + JPADrawContext::pcb->unk10);
	f32 x1 = +params->unk10
	         * (JPADrawContext::pcb->unk4 - JPADrawContext::pcb->unkC);
	f32 y1 = -params->unk14
	         * (JPADrawContext::pcb->unk8 - JPADrawContext::pcb->unk10);

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

	// TODO: inlines? more temps?
	char trash[0x10];

	JPADrawContext::pcb->unkA4(sin, cos, rotMtx);

	MTXMultVecArray(rotMtx, pt, pt, ARRAY_COUNT(pt));
	f32 x = particle->unk2C.x;
	f32 y = particle->unk2C.y;
	f32 z = particle->unk2C.z;

	GXBegin(GX_QUADS, GX_VTXFMT0, 8);
	GXPosition3f32(pt[0].x + x, pt[0].y + y, pt[0].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[0].x,
	               JPADrawContext::pcb->unk14[0].y);
	GXPosition3f32(pt[1].x + x, pt[1].y + y, pt[1].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[1].x,
	               JPADrawContext::pcb->unk14[1].y);
	GXPosition3f32(pt[2].x + x, pt[2].y + y, pt[2].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[2].x,
	               JPADrawContext::pcb->unk14[2].y);
	GXPosition3f32(pt[3].x + x, pt[3].y + y, pt[3].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[3].x,
	               JPADrawContext::pcb->unk14[3].y);
	GXPosition3f32(pt[4].x + x, pt[4].y + y, pt[4].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[0].x,
	               JPADrawContext::pcb->unk14[0].y);
	GXPosition3f32(pt[5].x + x, pt[5].y + y, pt[5].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[1].x,
	               JPADrawContext::pcb->unk14[1].y);
	GXPosition3f32(pt[6].x + x, pt[6].y + y, pt[6].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[2].x,
	               JPADrawContext::pcb->unk14[2].y);
	GXPosition3f32(pt[7].x + x, pt[7].y + y, pt[7].z + z);
	GXTexCoord2f32(JPADrawContext::pcb->unk14[3].x,
	               JPADrawContext::pcb->unk14[3].y);
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

void JPADrawExecLine::exec(const JPADrawContext*, JPABaseParticle* particle)
{
	if (particle->isInvisibleParticle())
		return;

	JGeometry::TVec3<f32> pt0;
	JGeometry::TVec3<f32> vel;

	particle->getGlobalPosition(pt0);
	vel.set(particle->unk38);
	if (!vel.isZero()) {
		vel.normalize();

		f32 size
		    = JPADrawContext::pcb->unk8 * particle->getDrawParamPPtr()->unk14;
		vel.scale(size);

		JGeometry::TVec3<f32> pt1;
		pt1.sub(pt0, vel);

		GXBegin(GX_LINES, GX_VTXFMT0, 2);
		GXPosition3f32(pt0.x, pt0.y, pt0.z);
		GXTexCoord2f32(0.0f, 0.0f);
		GXPosition3f32(pt1.x, pt1.y, pt1.z);
		GXTexCoord2f32(0.0f, 1.0f);
		GXEnd();
	}
}

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

void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext* dc)
{
	s32 tick  = dc->unk0->unk10.getFrame();
	s16 max   = dc->unk4->getColorRegAnmMaxFrm();
	s32 frame = tick < max ? tick : max;

	JPADrawContext::pcb->unkAC = frame;
}

void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext* dc)
{
	f32 tick = dc->unk0->unk10.getFrame();
	JPADrawContext::pcb->unkAC
	    = ((u32)tick) % (dc->unk4->getColorRegAnmMaxFrm() + 1);
}

void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext* dc)
{
	s32 tick     = dc->unk0->unk10.getFrame();
	s32 maxFrame = dc->unk4->getColorRegAnmMaxFrm();
	s32 odd   = (tick / maxFrame) & 1; // whether we're on an even or odd loop
	s32 frame = tick % maxFrame;
	JPADrawContext::pcb->unkAC = frame + (odd * maxFrame) - 2 * (odd * frame);
}

void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext*)
{
	JPADrawContext::pcb->unkAC = 0;
}

void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext*)
{
	JPADrawContext::pcb->unkAC = 0;
}

void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext* dc)
{
	s32 tick         = dc->unk0->unk10.getFrame();
	u8 anmKeyNum     = dc->unk4->getTextureAnmKeyNum();
	s32 idx          = ((anmKeyNum - 1) < tick) ? anmKeyNum - 1 : tick;
	dc->unk14->unkC0 = dc->unk20[dc->unk4->getTextureIndex(idx)];
}

void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext* dc)
{
	s32 tick         = dc->unk0->unk10.getFrame();
	dc->unk14->unkC0 = dc->unk20[dc->unk4->getTextureIndex(
	    tick % dc->unk4->getTextureAnmKeyNum())];
}

void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext* dc)
{
	s32 tick     = dc->unk0->unk10.getFrame();
	s32 maxFrame = dc->unk4->getTextureAnmKeyNum() - 1;
	s32 odd   = (tick / maxFrame) & 1; // whether we're on an even or odd loop
	s32 frame = tick % maxFrame;
	dc->unk14->unkC0 = dc->unk20[dc->unk4->getTextureIndex(
	    frame + (odd * maxFrame) - 2 * (odd * frame))];
}

void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext* dc)
{
	dc->unk14->unkC0 = dc->unk4->getTextureIndex();
}

void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext* dc)
{
	dc->unk14->unkC0 = dc->unk4->getTextureIndex();
}

void JPADrawExecCallBack::exec(const JPADrawContext* dc)
{
	dc->unk0->drawEmitterCallBack();
}

void JPADrawExecCallBack::exec(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->drawParticleCallBack(dc->unk0);
}

void JPADrawCalcScaleX::calc(const JPADrawContext* dc,
                             JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	if (JPADrawContext::pcb->unkA8 < dc->unk8->getScaleInTiming()) {
		params->unk10
		    = params->unkC
		      * ((dc->unk8->getIncreaseRateX() * JPADrawContext::pcb->unkA8)
		         + dc->unk8->getScaleInValueX());
	} else if (JPADrawContext::pcb->unkA8 > dc->unk8->getScaleOutTiming()) {
		params->unk10 = params->unkC
		                * ((dc->unk8->getDecreaseRateX()
		                    * (JPADrawContext::pcb->unkA8
		                       - dc->unk8->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk10 = params->unkC;
	}
}

void JPADrawCalcScaleY::calc(const JPADrawContext* dc,
                             JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	if (JPADrawContext::pcb->unkA8 < dc->unk8->getScaleInTiming()) {
		params->unk14
		    = params->unkC
		      * ((dc->unk8->getIncreaseRateY() * JPADrawContext::pcb->unkA8)
		         + dc->unk8->getScaleInValueY());
	} else if (JPADrawContext::pcb->unkA8 > dc->unk8->getScaleOutTiming()) {
		params->unk14 = params->unkC
		                * ((dc->unk8->getDecreaseRateY()
		                    * (JPADrawContext::pcb->unkA8
		                       - dc->unk8->getScaleOutTiming()))
		                   + 1.0f);
	} else {
		params->unk14 = params->unkC;
	}
}

void JPADrawCalcScaleXBySpeed::calc(const JPADrawContext* dc,
                                    JPABaseParticle* particle)
{
	JPADrawParams* params = particle->getDrawParamPPtr();

	// TODO: fakematch
	JGeometry::TVec3<f32> vel;
	(Vec&)vel = (Vec&)particle->unk38;

	if (JPADrawContext::pcb->unkA8 < dc->unk8->getScaleInTiming()) {
		params->unk10
		    = params->unkC
		      * ((dc->unk8->getIncreaseRateX() * JPADrawContext::pcb->unkA8)
		         + dc->unk8->getScaleInValueX());
	} else if (JPADrawContext::pcb->unkA8 > dc->unk8->getScaleOutTiming()) {
		params->unk10 = params->unkC
		                * ((dc->unk8->getDecreaseRateX()
		                    * (JPADrawContext::pcb->unkA8
		                       - dc->unk8->getScaleOutTiming()))
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

	// TODO: fakematch
	JGeometry::TVec3<f32> vel;
	(Vec&)vel = (Vec&)particle->unk38;

	if (JPADrawContext::pcb->unkA8 < dc->unk8->getScaleInTiming()) {
		params->unk14
		    = params->unkC
		      * ((dc->unk8->getIncreaseRateY() * JPADrawContext::pcb->unkA8)
		         + dc->unk8->getScaleInValueY());
	} else if (JPADrawContext::pcb->unkA8 > dc->unk8->getScaleOutTiming()) {
		params->unk14 = params->unkC
		                * ((dc->unk8->getDecreaseRateY()
		                    * (JPADrawContext::pcb->unkA8
		                       - dc->unk8->getScaleOutTiming()))
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

void JPADrawCalcScaleAnmTimingNormal::calc(const JPADrawContext*,
                                           JPABaseParticle* particle)
{
	JPADrawContext::pcb->unkA8 = particle->unk48;
}

void JPADrawCalcScaleAnmTimingRepeatX::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particle)
{
	JPADrawContext::pcb->unkA8 = (particle->getAge() % dc->unk8->getAnmCycleX())
	                             / (f32)dc->unk8->getAnmCycleX();
}

void JPADrawCalcScaleAnmTimingRepeatY::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particle)
{
	JPADrawContext::pcb->unkA8 = (particle->getAge() % dc->unk8->getAnmCycleY())
	                             / (f32)dc->unk8->getAnmCycleY();
}

void JPADrawCalcScaleAnmTimingReverseX::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particle)
{
	// whether we're on an even or odd loop
	f32 odd   = (particle->getAge() / dc->unk8->getAnmCycleX()) & 1;
	f32 frame = (f32)(particle->getAge() % dc->unk8->getAnmCycleX())
	            / dc->unk8->getAnmCycleX();
	JPADrawContext::pcb->unkA8 = odd + (frame - odd * 2.0f * frame);
}

void JPADrawCalcScaleAnmTimingReverseY::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particle)
{
	// whether we're on an even or odd loop
	f32 odd   = (particle->getAge() / dc->unk8->getAnmCycleY()) & 1;
	f32 frame = (f32)(particle->getAge() % dc->unk8->getAnmCycleY())
	            / dc->unk8->getAnmCycleY();
	JPADrawContext::pcb->unkA8 = odd + (frame - odd * 2.0f * frame);
}

void JPADrawCalcColorPrm::calc(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk2C
	    = dc->unk4->getPrmColor(JPADrawContext::pcb->unkAC);
}

void JPADrawCalcColorEnv::calc(const JPADrawContext* dc,
                               JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk30
	    = dc->unk4->getEnvColor(JPADrawContext::pcb->unkAC);
}

void JPADrawCalcColorCopyFromEmitter::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particle)
{
	particle->getDrawParamPPtr()->unk2C = dc->unk14->unkB8;
	particle->getDrawParamPPtr()->unk30 = dc->unk14->unkBC;
}

void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame = (particle->getAge() < dc->unk4->getColorRegAnmMaxFrm())
	                ? particle->getAge()
	                : dc->unk4->getColorRegAnmMaxFrm();
	JPADrawContext::pcb->unkAC = frame;
}

void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame
	    = ((particle->getDrawParamPPtr()->unk28 & dc->unk4->getColLoopOffset())
	       + particle->getAge())
	      % (dc->unk4->getColorRegAnmMaxFrm() + 1);
	JPADrawContext::pcb->unkAC = frame;
}

void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particle)
{
	s32 loopOffset
	    = particle->getDrawParamPPtr()->unk28 & dc->unk4->getColLoopOffset();
	s32 maxFrame = dc->unk4->getColorRegAnmMaxFrm();
	s32 t        = loopOffset + particle->getAge();
	s32 odd      = (t / maxFrame) & 1;
	s32 frame    = t % maxFrame;

	JPADrawContext::pcb->unkAC = frame + (odd * maxFrame) - 2 * (odd * frame);
}

void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext* dc,
                                         JPABaseParticle* particle)
{
	s32 start
	    = particle->getDrawParamPPtr()->unk28 & dc->unk4->getColLoopOffset();
	s32 maxFrame = dc->unk4->getColorRegAnmMaxFrm() + 1;
	s32 frame    = (s32)(start + maxFrame * particle->unk48) % maxFrame;
	JPADrawContext::pcb->unkAC = frame;
}

void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext* dc,
                                          JPABaseParticle* particle)
{
	s32 frame
	    = (particle->getDrawParamPPtr()->unk28 & dc->unk4->getColLoopOffset())
	      % (dc->unk4->getColorRegAnmMaxFrm() + 1);
	JPADrawContext::pcb->unkAC = frame;
}

void JPADrawCalcAlpha::calc(const JPADrawContext* dc, JPABaseParticle* particle)
{
	f32 time = particle->unk48;
	f32 alpha;
	if (time < dc->unk8->getAlphaInTiming()) {
		alpha = time * dc->unk8->getAlphaIncreaseRate()
		        + dc->unk8->getAlphaInValue();
	} else if (time > dc->unk8->getAlphaOutTiming()) {
		alpha = dc->unk8->getAlphaBaseValue()
		        + dc->unk8->getAlphaDecreaseRate()
		              * (time - dc->unk8->getAlphaOutTiming());
	} else {
		alpha = dc->unk8->getAlphaBaseValue();
	}
	particle->getDrawParamPPtr()->unk20 = alpha;
}

void JPADrawCalcAlphaFlickNrmSin::calc(const JPADrawContext* dc,
                                       JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamPPtr();

	f32 sin = JMASSin((particles->getAge() * 16384) * params->unk24
	                  * (1.0f - dc->unk8->getAlphaWaveParam1()));
	params->unk20
	    *= params->unk24
	           * (((sin - 1.0f) * 0.5f) * dc->unk8->getAlphaWaveParam3())
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
	            * ((((JMASSin(theta * (1.0f - dc->unk8->getAlphaWaveParam1()))
	                  + JMASSin(theta
	                            * (1.0f - dc->unk8->getAlphaWaveParam2())))
	                 - 2.0f)
	                * 0.5f)
	               * dc->unk8->getAlphaWaveParam3())
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
	f32 mul3  = (dc->unk8->getAlphaWaveParam3() * 0.5f) * params->unk24;

	// TODO: try to introduce temps without making this non-matching
	params->unk20
	    *= ((mul3
	         * (JMASSin(theta * (1.0f - dc->unk8->getAlphaWaveParam1()))
	            - 1.0f))
	        + 1.0f)
	       * ((mul3
	           * (JMASSin(theta * (1.0f - dc->unk8->getAlphaWaveParam2()))
	              - 1.0f))
	          + 1.0f);
	if (params->unk20 < 0.0f)
		params->unk20 = 0.0f;
}

void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	s32 idx = ((dc->unk4->getTextureAnmKeyNum() - 1) < (s32)particles->getAge())
	              ? (dc->unk4->getTextureAnmKeyNum() - 1)
	              : particles->getAge();
	particles->getDrawParamPPtr()->unk3A
	    = dc->unk20[dc->unk4->getTextureIndex(idx)];
}

void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	particles->getDrawParamPPtr()->unk3A = dc->unk20[dc->unk4->getTextureIndex(
	    ((particles->getDrawParamPPtr()->unk28 & dc->unk4->getTexLoopOffset())
	     + particles->getAge())
	    % dc->unk4->getTextureAnmKeyNum())];
}

void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext* dc,
                                             JPABaseParticle* particles)
{
	s32 loopOffset
	    = particles->getDrawParamPPtr()->unk28 & dc->unk4->getTexLoopOffset();
	s32 maxFrame = dc->unk4->getTextureAnmKeyNum() - 1;
	s32 t        = loopOffset + particles->getAge();
	s32 odd      = (t / maxFrame) & 1;
	s32 frame    = t % maxFrame;

	particles->getDrawParamPPtr()->unk3A = dc->unk20[dc->unk4->getTextureIndex(
	    frame + (odd * maxFrame) - 2 * (odd * frame))];
}

void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext* dc,
                                           JPABaseParticle* particles)
{
	s32 maxFrame = dc->unk4->getTextureAnmKeyNum();
	s32 start
	    = particles->getDrawParamPPtr()->unk28 & dc->unk4->getTexLoopOffset();
	s32 frame = (s32)(start + maxFrame * particles->unk48) % maxFrame;
	particles->getDrawParamPPtr()->unk3A
	    = dc->unk20[dc->unk4->getTextureIndex(frame)];
}

void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext* dc,
                                            JPABaseParticle* particles)
{
	s32 start
	    = particles->getDrawParamPPtr()->unk28 & dc->unk4->getTexLoopOffset();
	particles->getDrawParamPPtr()->unk3A = dc->unk20[dc->unk4->getTextureIndex(
	    start % dc->unk4->getTextureAnmKeyNum())];
}

void JPADrawCalcChildAlphaOut::calc(const JPADrawContext*,
                                    JPABaseParticle* particles)
{
	particles->getDrawParamCPtr()->unk20 = 1.0f - particles->unk48;
}

void JPADrawCalcChildScaleOut::calc(const JPADrawContext*,
                                    JPABaseParticle* particles)
{
	JPADrawParams* params = particles->getDrawParamCPtr();

	params->unk10 = params->unkC * (1.0f - particles->unk48);
	params->unk14 = params->unk24 * (1.0f - particles->unk48);
}
