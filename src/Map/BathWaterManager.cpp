#include <Map/BathWaterManager.hpp>
#include <JSystem/ResTIMG.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTexMtx.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/gx/GXCommandList.h>

#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// NOTE: the main tragedy of this file is that it looks like a bunch of classes
// were defined right inside of the cpp file and all their methods were defined
// in-line, and furthermore, some methods were defined in-line inside of a
// NESTED class, in which case MWCC **always** inlines the contents independent
// of any heuristics. Hence, a LOT here is dumb inlining guesswork.
// Most inlines are fabricated.

TBathWaterParams::TBathWaterParams(const char* path)
    : TParams(path)
    , PARAM_INIT(suppliesDrops, 1)
    , PARAM_INIT(bathtubGravity, 1)
    , PARAM_INIT(intersects, 1)
    , PARAM_INIT(isVisible, 1)
    , PARAM_INIT(checksMario, 1)
    , PARAM_INIT(numDrops, 120)
    , PARAM_INIT(dropRadius, 300.0f)
    , PARAM_INIT(texScale, 3.0f)
    , PARAM_INIT(hitScale, 5.0f)
    , PARAM_INIT(modelScale, 1.5f)
    , PARAM_INIT(modelScale2, 1.0f)
    , PARAM_INIT(modelScaleY, 1.0f)
    , PARAM_INIT(gravity, 18.0f)
    , PARAM_INIT(bounceY, 0.05f)
    , PARAM_INIT(bounceXZ, 0.5f)
    , PARAM_INIT(damp, 0.985f)
    , PARAM_INIT(jump, 65.0f)
    , PARAM_INIT(overGravity, 0.0f)
    , PARAM_INIT(emitVel, 20.0f)
    , PARAM_INIT(lifeTime, 0)
{
	TParams::load(mPrmPath);
}

TBathWaterGlobalParams::TBathWaterGlobalParams()
    : TParams("/MapObj/bathwaterglobal.prm")
    , PARAM_INIT(regR, 0)
    , PARAM_INIT(regG, 0)
    , PARAM_INIT(regB, 0)
    , PARAM_INIT(regA, 255)
    , PARAM_INIT(kRegR, 144)
    , PARAM_INIT(kRegG, 24)
    , PARAM_INIT(kRegB, 0)
    , PARAM_INIT(kRegA, 255)
    , PARAM_INIT(polygonR, 255)
    , PARAM_INIT(polygonG, 255)
    , PARAM_INIT(polygonB, 151)
    , PARAM_INIT(indTexScale, 1.5f)
    , PARAM_INIT(showsCap, 1)
    , PARAM_INIT(bendsNormal, 0)
    , PARAM_INIT(showsMist, 0)
    , PARAM_INIT(clearsAlpha, 1)
    , PARAM_INIT(alpha, 200)
    , PARAM_INIT(scrolls, 1)
    , PARAM_INIT(displaysMesh, 0)
    , PARAM_INIT(mode, 0)
    , PARAM_INIT(mask, 1)
    , PARAM_INIT(indirectScale, -3)
    , PARAM_INIT(scrollSpan, 60)
    , PARAM_INIT(meshTexWidth, 80)
    , PARAM_INIT(envMapScale, 0.6f)
    , PARAM_INIT(capHeight, 150.0f)
    , PARAM_INIT(meshWidth, 7000.0f)
{
	TParams::load(mPrmPath);
}

static void minmax_set(JGeometry::TBox3<f32>&, const JGeometry::TVec3<f32>&) { }

class TBathWater : public THitActor {
public:
	class TDrop;

	TBathWater()
	    : THitActor("HitActor")
	    , unk68(0)
	{
		unk70 = 500;
		unk88 = new TDrop[unk70];
		unk8C = 0;
	}

	void initialize(TBathWaterParams* params, const TBathtubData& data)
	{
		unk8C = params;
		unk74 = params->numDrops.get();

		int i = 0;
		for (TBathWater::TDrop *drop = unk88, *end = unk88 + unk70; drop < end;
		     ++drop) {
			drop->reset(data.getPos(i++, unk70, unk8C->dropRadius.get()),
			            unk68.get_float01());
		}

		initHitActor(0x4000025B, 1, 0x80000000, unk8C->dropRadius.get(),
		             unk8C->dropRadius.get() * 2.0f, 0.0f, 0.0f);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
		unk78.set(0.0f, 0.0f, 0.0f);
		unk84 = 0.0f;
	}

	void addDrop(const JGeometry::TVec3<f32>& param_1, f32 param_2)
	{
		if (unk74 < unk8C->numDrops.get()) {

			unk88[unk74].reset(param_1, unk68.get_float01());
			unk88[unk74].unkC.y = param_2;

			// Main hint at inlining + classes in cpp being employed:
			// otherwise you couldn't hope to have this on line 655.
			OSReport("BathWaterManager.cpp(%d): ...\n", 655, unk74++);
		}
	}

	bool eraseDrop(TDrop* drop)
	{
		int idx = drop - unk88;
		if (idx < unk74) {
			unk74--;
			if (idx < unk74)
				*drop = unk88[unk74];
			return true;
		} else {
			return false;
		}
	}

	bool tryHitMario(THitActor* mario)
	{
		TBathWaterParams* params = unk8C;
		f32 rr    = params->dropRadius.get() * params->hitScale.get();
		f32 upper = mario->mDamageHeight + rr;
		f32 radH  = rr + mario->mDamageRadius;
		f32 radH2 = radH * radH;
		f32 mx    = mario->mPosition.x;
		f32 my    = mario->mPosition.y;
		f32 mz    = mario->mPosition.z;

		for (TBathWater::TDrop* drop = unk88; drop < unk88 + unk74; ++drop) {
			f32 dy = drop->unk0.y - my;
			if (!(dy > upper) && !(dy < -rr)) {
				f32 dx = mx - drop->unk0.x;
				f32 dz = mz - drop->unk0.z;
				if (dx * dx + dz * dz < radH2) {
					setAttackRadius(unk8C->dropRadius.get());
					setAttackRadius(unk8C->dropRadius.get() * 2.0f);
					mPosition.set(drop->unk0);
					mPosition.y -= unk8C->dropRadius.get();
					mario->receiveMessage(this, HIT_MESSAGE_UNKA);
					return true;
				}
			}
		}
		return false;
	}

	bool tryHitMario2(THitActor* mario, const TBathtubData& data)
	{
		f32 mx  = mario->mPosition.x;
		f32 my  = mario->mPosition.y;
		f32 mz  = mario->mPosition.z;
		f32 mdh = mario->mDamageHeight;

		JGeometry::TVec3<f32> capCenter = data.getThing();

		f32 rad = JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
		                                      - data.unk44 * data.unk44);
		bool hit;
		if (capCenter.y < my)
			return false;

		f32 h = data.unk3C - data.unk44;
		if (my + mdh < capCenter.y - h)
			return false;

		f32 dx = mx - capCenter.x;
		f32 dz = mz - capCenter.z;
		if (dx * dx + dz * dz >= rad * rad)
			return false;

		setAttackRadius(rad);
		mPosition.set(data.getThing());
		mPosition.y -= h;
		setAttackHeight(h);
		mario->receiveMessage(this, HIT_MESSAGE_UNKA);
		return true;
	}

	class TDrop {
	public:
		TDrop() { }

		void reset(const JGeometry::TVec3<f32>& param_1, f32 param_2)
		{
			unk48 = param_2;
			unk0.set(param_1);
			unkC.zero();
			unk18.i.zero();
			unk18.f.zero();
			unk30.i.zero();
			unk30.f.zero();
			unk4C = 0;
		}

		void doThing(f32 damp)
		{
			unk0.add(unk18.i);
			unk0.add(unk18.f);
			unkC.scale(damp);
			unkC.add(unk30.i);
			unkC.add(unk30.f);
		}

		void calcBathtub(const TBathtubData& data, f32 radius,
		                 const JGeometry::TVec3<f32>& grav1,
		                 const JGeometry::TVec3<f32>& grav2, int& count,
		                 JGeometry::TVec3<f32>& accum)
		{
			JGeometry::TVec3<f32> m(data.unk18.at(1, 0), data.unk18.at(1, 1),
			                        data.unk18.at(1, 2));
			JGeometry::TVec3<f32> delta;
			delta.sub(unk0, data.mPos);
			f32 outerR = data.unk40 + radius;
			f32 innerR = data.unk3C - radius;
			f32 distSq = delta.squared();
			f32 proj   = m.dot(delta);

			if (distSq <= outerR * outerR) {
				if (proj < 0.0f) {
					if (distSq >= innerR * innerR) {
						f32 dist = JGeometry::TUtil<f32>::sqrt(distSq);
						f32 pen  = dist - innerR;
						f32 inv  = -1.0f / dist;
						JGeometry::TVec3<f32> n;
						n.scale(inv, delta);
						JGeometry::TVec3<f32> point;
						point.scale(pen, n);
						unk18.extend(point);

						f32 c = -n.dot(unkC);
						if (c < 0.0f)
							c = 0.0f;

						JGeometry::TVec3<f32> point2;
						point2.scale(c, n);
						point2 += data.unk58;
						unk30.extend(point2);
					} else {
						f32 f = radius + (data.mPos.y - data.unk44);
						if (unk0.y < f) {
							f32 pen = f - unk0.y;
							JGeometry::TVec3<f32> point(0.0f, pen, 0.0f);
							unk18.extend(point);
							JGeometry::TVec3<f32> point2(0.0f, -1.0f * unkC.y,
							                             0.0f);
							point2 += data.unk58;
							unk30.extend(point2);
						} else {
							count++;
							accum.add(unk0);
						}
					}
				} else {
					count++;
					accum.add(unk0);
				}
				unk30.extend(grav1);
			} else {
				if (proj > 0.0f && proj < radius + data.unk48
				    && distSq > innerR * innerR && distSq < outerR * outerR) {
					JGeometry::TVec3<f32> point;
					point.scale((radius + data.unk48) - proj, m);
					unk18.extend(point);

					JGeometry::TVec3<f32> r;
					r.scale(1.5f * -m.dot(unkC), m);
					JGeometry::TVec3<f32> thing;
					thing.set(delta);
					thing.setLength(0.01f * radius);
					r.add(thing);
					unk30.extend(r);
					unk30.extend(grav2);
				} else {
					unk30.extend(grav2);
					count++;
					accum.add(unk0);
				}
			}
		}

		// TODO: break up into pieces and move into TBathWater.
		// Placed here ONLY to force-inline it abusing an MWCC bug.
		static void calcWaterModel(TBathWater* bw, const TBathtubData& data)
		{
			f32 gravity = bw->unk8C->gravity.get();
			JGeometry::TVec3<f32> gravVec1;
			gravVec1.scale(-gravity,
			               data.getGravityDir(bw->unk8C->overGravity.get()));
			JGeometry::TVec3<f32> gravVec2(0.0f, -gravity, 0.0f);

			TDrop* end2    = bw->unk88 + bw->unk74;
			f32 dropRadius = bw->unk8C->dropRadius.get();
			int count      = 0;

			JGeometry::TVec3<f32> accum(0.0f, 0.0f, 0.0f);

			if (data.unk18.at(1, 1) > 0.0f) {
				for (TDrop* drop = bw->unk88; drop < end2; ++drop) {
					drop->unk0.add(drop->unkC);
					drop->unk18.i.zero();
					drop->unk18.f.zero();
					drop->unk30.i.zero();
					drop->unk30.f.zero();
					drop->calcBathtub(data, dropRadius, gravVec1, gravVec2,
					                  count, accum);
				}
			} else {
				for (TDrop* drop = bw->unk88; drop < end2; ++drop) {
					drop->unk0.add(drop->unkC);
					drop->unk18.i.zero();
					drop->unk18.f.zero();
					drop->unk30.i.zero();
					drop->unk30.f.zero();
					drop->unk30.extend(gravVec2);
					accum.add(drop->unk0);
					count++;
				}
			}

			f32 heightVal;
			if (count * 30 > bw->unk74) {
				f32 inv = 1.0f / (f32)count;
				accum *= inv;
				f32 t     = 3.0f * (f32)count;
				t         = t / (f32)bw->unk74;
				heightVal = JGeometry::TUtil<f32>::sqrt(t);
				if (heightVal > 1.0f)
					heightVal = 1.0f;
			} else {
				heightVal = 0.0f;
			}
			bw->unk78.set(accum);
			bw->unk84 = heightVal;

			if (bw->unk8C->intersects.get()) {
				f32 dropRadius = bw->unk8C->dropRadius.get();
				f32 twoR       = 2.0f * dropRadius;
				f32 sep2       = 4.0f * (dropRadius * dropRadius);
				for (TDrop* a = bw->unk88; a < end2; a++) {
					for (TDrop* b = a + bw->unk8C->intersects.get(); b < end2;
					     b += bw->unk8C->intersects.get()) {
						JGeometry::TVec3<f32> local_2D8;
						local_2D8.sub(b->unk0, a->unk0);
						(void)&local_2D8;
						if (!(local_2D8.squared() > sep2)) {
							f32 dist = local_2D8.length();
							JGeometry::TVec3<f32> local_2E4;
							local_2E4.scale(1.0f / dist, local_2D8);

							(void)&local_2E4;

							f32 half = (twoR - dist) / 2.0f;

							local_2D8.x = local_2E4.x * half;
							local_2D8.z = local_2E4.z * half;

							f32 hny = half * local_2E4.y;

							local_2D8.set(local_2E4.x * half,
							              (local_2E4.y + 1.0f) * hny,
							              local_2E4.z * half);
							b->unk18.extend(local_2D8);
							local_2D8.x = -local_2D8.x;
							local_2D8.z = -local_2D8.z;
							local_2D8.y = (local_2E4.y - 1.0f) * hny;
							a->unk18.extend(local_2D8);

							f32 mag = hny;
							if (mag < twoR - dist)
								mag = twoR - dist;

							local_2E4.x *= mag * bw->unk8C->bounceXZ.get();
							local_2E4.y *= mag * bw->unk8C->bounceY.get();
							local_2E4.z *= mag * bw->unk8C->bounceXZ.get();
							b->unk30.extend(local_2E4);
							local_2E4.negate();
							a->unk30.extend(local_2E4);
						}
					}
				}
			}

			f32 threshold = data.mPos.y - data.unk3C;
			if (data.unk65)
				threshold = data.mPos.y - 8.0f * data.unk3C;

			int respawnIdx = 0;
			for (TDrop* drop = bw->unk88; drop < end2; drop++) {
				if (drop->unk0.y < threshold) {
					if (bw->unk8C->suppliesDrops.get() && !data.unk65) {
						drop->reset(
						    data.getPos(respawnIdx++, bw->unk70, dropRadius),
						    bw->unk68.get_float01());
					} else if (bw->eraseDrop(drop)) {
						end2--;
						drop->doThing(bw->unk8C->damp.get());
					}
				} else {
					drop->doThing(bw->unk8C->damp.get());
				}
			}

			int lifeTime = bw->unk8C->lifeTime.get();
			if (lifeTime > 0) {
				for (TDrop* drop = bw->unk88; drop < end2; --end2, ++drop) {
					drop->unk4C++;
					if (drop->unk4C > lifeTime)
						bw->eraseDrop(drop);
				}
			}

			if (bw->unk74 < bw->unk8C->numDrops.get()) {
				if (bw->unk8C->suppliesDrops.get() && !data.unk65) {
					bw->unk88[bw->unk74++].reset(
					    data.getPos(respawnIdx++, bw->unk70, dropRadius),
					    bw->unk68.get_float01());
				}
			} else if (bw->unk74 > bw->unk8C->numDrops.get()) {
				bw->unk74 = bw->unk8C->numDrops.get();
			}
		}

	public:
		/* 0x0 */ JGeometry::TVec3<f32> unk0;
		/* 0xC */ JGeometry::TVec3<f32> unkC;
		/* 0x18 */ JGeometry::TBox3<f32> unk18;
		/* 0x30 */ JGeometry::TBox3<f32> unk30;
		/* 0x48 */ f32 unk48;
		/* 0x4C */ int unk4C;
	};

public:
	/* 0x68 */ JMath::TRandomFast unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ s32 unk70;
	/* 0x74 */ s32 unk74;
	/* 0x78 */ JGeometry::TVec3<f32> unk78;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ TDrop* unk88;
	/* 0x8C */ TBathWaterParams* unk8C;
};

static void initScreen2D(s16, s16) { }

static void drawCap(const JGeometry::TVec3<f32>& pos, f32 radius)
{
	static f32 delta = 2.0f * 3.1415927f / 30.0f;
	f32 angle;
	f32 r;

	r     = radius / cosf(0.5f * delta);
	angle = 0.0f;
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 30);
	for (int i = 0; i < 30; i++) {
		GXPosition3f32(r * cosf(angle) + pos.x, pos.y, r * sinf(angle) + pos.z);
		GXTexCoord2u8(0x40, 0x40);
		angle += delta;
	}
	GXEnd();
}

namespace {
void clearEFB_alpha(s16 x, s16 y, s16 wd, s16 ht, u8 alpha)
{
	Mtx44 m;
	Mtx pmtx;

	if (wd <= 0)
		wd = SMSGetGameRenderWidth();
	if (ht <= 0)
		ht = SMSGetGameRenderHeight();

	f32 fx      = x;
	f32 fwd     = wd;
	f32 fy      = y;
	f32 fht     = ht;
	f32 fright  = fx + fwd;
	f32 fbottom = fy + fht;

	C_MTXOrtho(m, fy, fbottom, fx, fright, 0.0f, 1.0f);
	PSMTXIdentity(pmtx);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetProjection(m, GX_ORTHOGRAPHIC);
	GXSetViewport(fx, fy, fwd, fht, 0.0f, 1.0f);
	GXSetScissor(x, y, wd, ht);
	GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, alpha);
	GXLoadPosMtxImm(pmtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2s16(x, y);
	GXPosition2s16(x + wd, y);
	GXPosition2s16(x + wd, y + ht);
	GXPosition2s16(x, y + ht);
	GXEnd();

	GXSetColorUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, alpha);
}
} // namespace

void set_light_pos_nrm(GXLightObj*, MtxPtr, JGeometry::TVec3<f32>*,
                       JGeometry::TVec3<f32>*)
{
}

static void init_draw_light(MtxPtr, JGeometry::TVec3<f32>*, GXColor*) { }

static void draw_mist(u16 x, u16 y, u16 wd, u16 ht, void* buffer)
{
	Mtx e_m;
	Mtx44 m;
	GXTexObj tex_obj;

	GXColor tev_color = { 0x03, 0x03, 0x03, 0x00 };
	u8 vFilter[7]     = { 0x15, 0x00, 0x00, 0x16, 0x00, 0x00, 0x15 };

	f32 f_left   = x;
	f32 f_wd     = wd;
	f32 f_top    = y;
	f32 f_ht     = ht;
	f32 f_right  = f_left + f_wd;
	f32 f_bottom = f_top + f_ht;
	f32 offset_x = (4.0f / f_wd);
	f32 offset_y = (2.0f / f_ht);

	C_MTXOrtho(m, f_top, f_bottom, f_left, f_right, 0.0f, 1.0f);
	PSMTXIdentity(e_m);
	GXSetTexCopySrc(x, y, wd, ht);
	GXSetCopyFilter(GX_FALSE, 0, GX_TRUE, vFilter);
	GXSetTexCopyDst(wd >> 1, ht >> 1, GX_TF_RGB565, GX_TRUE);
	GXCopyTex(buffer, GX_FALSE);
	GXPixModeSync();
	GXInitTexObj(&tex_obj, buffer, wd >> 1, ht >> 1, GX_TF_RGB565, GX_CLAMP,
	             GX_CLAMP, 0);
	GXInitTexObjLOD(&tex_obj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX3, GX_DIRECT);

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0);

	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetNumTexGens(4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, 0x3c, GX_FALSE,
	                  0x7d);

	GXLoadTexObj(&tex_obj, GX_TEXMAP0);

	GXSetNumTevStages(4);

	// Stage 0
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_HALF, GX_CC_C0);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_FALSE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevDirect(GX_TEVSTAGE0);

	// Stage 1
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_HALF,
	                GX_CC_CPREV);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_FALSE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevDirect(GX_TEVSTAGE1);

	// Stage 2
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_HALF,
	                GX_CC_CPREV);
	GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_FALSE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevDirect(GX_TEVSTAGE2);

	// Stage 3
	GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_TEXC, GX_CC_HALF,
	                GX_CC_CPREV);
	GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevDirect(GX_TEVSTAGE3);

	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

	GXSetTevColor(GX_TEVREG0, tev_color);
	GXSetProjection(m, GX_ORTHOGRAPHIC);
	GXSetViewport(f_left, f_top, f_wd, f_ht, 0.0, 1.0);
	GXSetScissor(x, y, wd, ht);

	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_TRUE);
	GXLoadPosMtxImm(e_m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);
	GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(x, y);
	GXTexCoord2f32(-offset_x, 0.0f);
	GXTexCoord2f32(offset_x, 0.0f);
	GXTexCoord2f32(0.0f, -offset_y);
	GXTexCoord2f32(0.0f, offset_y);
	GXPosition2u16(x + wd, y);
	GXTexCoord2f32(1.0f - offset_x, 0.0f);
	GXTexCoord2f32(1.0f + offset_x, 0.0f);
	GXTexCoord2f32(1.0f, -offset_y);
	GXTexCoord2f32(1.0f, offset_y);
	GXPosition2u16(x + wd, y + ht);
	GXTexCoord2f32(1.0f - offset_x, 1.0f);
	GXTexCoord2f32(1.0f + offset_x, 1.0f);
	GXTexCoord2f32(1.0f, 1.0f - offset_y);
	GXTexCoord2f32(1.0f, 1.0f + offset_y);
	GXPosition2u16(x, y + ht);
	GXTexCoord2f32(-offset_x, 1.0f);
	GXTexCoord2f32(+offset_x, 1.0f);
	GXTexCoord2f32(0.0f, 1.0f - offset_y);
	GXTexCoord2f32(0.0f, 1.0f + offset_y);
	GXEnd();

	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(x, y);
	GXTexCoord2f32(-offset_x, 0.0f);
	GXTexCoord2f32(offset_x, 0.0f);
	GXTexCoord2f32(0.0f, -offset_y);
	GXTexCoord2f32(0.0f, offset_y);
	GXPosition2u16(x + wd, y);
	GXTexCoord2f32(1.0f - offset_x, 0.0f);
	GXTexCoord2f32(1.0f + offset_x, 0.0f);
	GXTexCoord2f32(1.0f, -offset_y);
	GXTexCoord2f32(1.0f, offset_y);
	GXPosition2u16(x + wd, y + ht);
	GXTexCoord2f32(1.0f - offset_x, 1.0f);
	GXTexCoord2f32(1.0f + offset_x, 1.0f);
	GXTexCoord2f32(1.0f, 1.0f - offset_y);
	GXTexCoord2f32(1.0f, 1.0f + offset_y);
	GXPosition2u16(x, y + ht);
	GXTexCoord2f32(-offset_x, 1.0f);
	GXTexCoord2f32(+offset_x, 1.0f);
	GXTexCoord2f32(0.0f, 1.0f - offset_y);
	GXTexCoord2f32(0.0f, 1.0f + offset_y);
	GXEnd();
}

static void f2i(f32) { }

static void make_ORTHOmatrix(MtxPtr, f32, f32, f32, f32, f32, f32, f32, f32) { }

static void init_tobj_resource(GXTexObj* tex_obj, void* resource)
{
	ResTIMG* timg;
	GXTexFmt format;
	u16 width;
	u16 height;
	GXTexWrapMode wrapS;
	GXTexWrapMode wrapT;
	GXTexFilter minFilter;
	GXTexFilter magFilter;
	void* image;

	timg = (ResTIMG*)resource;

	format    = (GXTexFmt)timg->format;
	width     = timg->width;
	height    = timg->height;
	wrapS     = (GXTexWrapMode)timg->wrapS;
	wrapT     = (GXTexWrapMode)timg->wrapT;
	image     = (u8*)timg + timg->imageDataOffset;
	minFilter = (GXTexFilter)timg->minFilter;
	magFilter = (GXTexFilter)timg->magFilter;

	DCStoreRange(image, GXGetTexBufferSize(width, height, format, GX_FALSE, 0));
	GXInitTexObj(tex_obj, image, width, height, format, wrapS, wrapT, GX_FALSE);
	GXInitTexObjLOD(tex_obj, minFilter, magFilter, 0.0f, 0.0f, 0.0f, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
}

class TBathWaterRenderer {
public:
	virtual void prerender(JDrama::TGraphics*, const TBathtubData&,
	                       TBathWater**, TBathWaterParams**, int)
	    = 0;
	virtual void render(JDrama::TGraphics*, const TBathtubData&, TBathWater**,
	                    TBathWaterParams**, int)
	    = 0;
	virtual f32 getHeight(f32, f32) const = 0;
};

class TBathWaterFlatRenderer : public TBathWaterRenderer {
public:
	TBathWaterFlatRenderer(TBathWaterGlobalParams* params)
	{
		unk2C = params;
		unk28 = new (0x20) char[0x14A000];
		unk24 = JKRGetResource("/scene/map/map/ball.bti");
		init_tobj_resource(&unk4, unk24);
	}

	virtual void prerender(JDrama::TGraphics*, const TBathtubData&,
	                       TBathWater**, TBathWaterParams**, int)
	{
	}
	virtual void render(JDrama::TGraphics* graphics, const TBathtubData& data,
	                    TBathWater** waters, TBathWaterParams** params, int num)
	{
		s16 w = SMSGetGameRenderWidth();
		s16 h = SMSGetGameRenderHeight();
		if (unk2C->clearsAlpha.get())
			clearEFB_alpha(0, 0, 0, 0, 0);

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetNumChans(0);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);
		GXSetNumChans(0);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetNumTexGens(1);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c,
		                  GX_FALSE, 0x7d);
		GXLoadTexObj(&unk4, GX_TEXMAP0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetCullMode(GX_CULL_NONE);
		GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
		GXLoadPosMtxImm(graphics->mViewMtx.mMtx, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);

		f32 r0 = graphics->mViewMtx.at(0, 0);
		f32 r1 = graphics->mViewMtx.at(0, 1);
		f32 r2 = graphics->mViewMtx.at(0, 2);
		f32 u0 = graphics->mViewMtx.at(1, 0);
		f32 u1 = graphics->mViewMtx.at(1, 1);
		f32 u2 = graphics->mViewMtx.at(1, 2);

		for (int i = 0; i < num; ++i) {
			TBathWaterParams* p = params[i];
			if (p->isVisible.get()) {
				f32 size = p->texScale.get() * p->dropRadius.get();
				f32 rx = r0 * size, ry = r1 * size, rz = r2 * size;
				f32 ux = u0 * size, uy = u1 * size, uz = u2 * size;
				GXBegin(GX_QUADS, GX_VTXFMT0, (waters[i]->unk74 * 4) & 0xfffc);
				for (TBathWater::TDrop* d = waters[i]->unk88;
				     d < waters[i]->unk88 + waters[i]->unk74; ++d) {
					GXPosition3f32(d->unk0.x + rx, d->unk0.y + ry,
					               d->unk0.z + rz);
					GXTexCoord2u8(0, 0);
					GXPosition3f32(d->unk0.x + ux, d->unk0.y + uy,
					               d->unk0.z + uz);
					GXTexCoord2u8(0, 0x80);
					GXPosition3f32(d->unk0.x - rx, d->unk0.y - ry,
					               d->unk0.z - rz);
					GXTexCoord2u8(0x80, 0x80);
					GXPosition3f32(d->unk0.x - ux, d->unk0.y - uy,
					               d->unk0.z - uz);
					GXTexCoord2u8(0x80, 0);
				}
				GXEnd();
			}
		}

		if (unk2C->showsCap.get() && !data.unk65) {
			drawCap(data.getThing(),
			        JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
			                                    - data.unk44 * data.unk44));
		}

		GXColor color = (GXColor) { 0x78, 0xFA, 0x14, unk2C->alpha.get() };

		GXTexObj texObj;
		GXInitTexObj(&texObj, unk28, w, h, GX_TF_I8, GX_CLAMP, GX_CLAMP,
		             GX_FALSE);
		GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
		                GX_FALSE, GX_ANISO_1);
		GXSetTexCopySrc(0, 0, w, h);
		GXSetTexCopyDst(w, h, GX_CTF_A8, GX_FALSE);
		GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);
		GXCopyTex(unk28, GX_FALSE);
		GXPixModeSync();

		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
		GXSetNumChans(0);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetNumTexGens(1);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c,
		                  GX_FALSE, 0x7d);
		GXLoadTexObj(&texObj, GX_TEXMAP0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_C0);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A0, GX_CA_KONST,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO,
		                GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_5_8);
		GXSetTevColor(GX_TEVREG0, color);

		s16 w2 = SMSGetGameRenderWidth();
		s16 h2 = SMSGetGameRenderHeight();
		TPosition3f mtx;
		mtx.identity();
		Mtx44 ortho;
		C_MTXOrtho(ortho, 0.0f, (f32)h2, 0.0f, (f32)w2, -1.0f, 1.0f);
		GXSetProjection(ortho, GX_ORTHOGRAPHIC);
		GXSetViewport(0.0f, 0.0f, (f32)w2, (f32)h2, 0.0f, 1.0f);
		GXSetScissor(0, 0, w2, h2);
		GXLoadPosMtxImm(mtx, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		GXSetCullMode(GX_CULL_BACK);
		GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);
		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_FALSE);

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2s16(0, 0);
		GXTexCoord2f32(0.0f, 0.0f);
		GXPosition2s16(w, 0);
		GXTexCoord2f32(1.0f, 0.0f);
		GXPosition2s16(w, h);
		GXTexCoord2f32(1.0f, 1.0f);
		GXPosition2s16(0, h);
		GXTexCoord2f32(0.0f, 1.0f);
		GXEnd();

		if (unk2C->showsMist.get())
			draw_mist(0, 0, w, h, unk28);

		GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
	}
	virtual f32 getHeight(f32, f32) const { return 0.0f; }

public:
	/* 0x4 */ GXTexObj unk4;
	/* 0x24 */ void* unk24;
	/* 0x28 */ void* unk28;
	/* 0x2C */ TBathWaterGlobalParams* unk2C;
};

class TBathWaterMeshRenderer : public TBathWaterRenderer {
public:
	TBathWaterMeshRenderer(TBathWaterGlobalParams* params,
	                       JUTTexture* screen_tex)
	{
		unk80134 = params;
		unk80138 = screen_tex;
		unk800AE = 0;
		unk80150 = new (0x20) char[0x55668];
		unk80154 = new (0x20) char[0x30023];
		unk800A4 = new (0x20) char[0x10000];
		unk800A8 = new (0x20) char[0x55668];
		unk80144 = J3DModelLoaderDataBase::load(
		    JKRGetResource("/scene/map/map/ball.bmd"),
		    J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift));
		unk80148 = J3DModelLoaderDataBase::load(
		    JKRGetResource("/scene/map/map/water.bmd"),
		    J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift));
		unk8013C = new JUTTexture(
		    (const ResTIMG*)JKRGetResource("/scene/map/map/water_ball.bti"));
		JUTTexture* warpTex = new JUTTexture(
		    (const ResTIMG*)JKRGetResource("/scene/map/map/water_warp.bti"));
		unk80140 = warpTex;
		init_tobj_resource(&unk800B4,
		                   JKRGetResource("/scene/map/map/ball.bti"));
		init_tobj_resource(&unk800F4,
		                   JKRGetResource("/scene/map/map/mesh.bti"));

		TScreenTexture* tex = JDrama::TNameRefGen::search<TScreenTexture>(
		    "スクリーンテクスチャ");

		unk80148->getTexture()->setResTIMG(1, *tex->getTexture()->getTexInfo());
		unk80148->getMaterialNodePointer(0)->makeDisplayList();
		unk8014C = new J3DModel(unk80148, 0, 1);

		static u8 clear_z_TX[64] = {
			0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
			0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
			0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		};
		GXInitTexObj(&unk80114, clear_z_TX, 4, 4, GX_TF_Z24X8, GX_REPEAT,
		             GX_REPEAT, 0);
		GXInitTexObjLOD(&unk80114, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
		                GX_FALSE, GX_ANISO_1);
		clearHeightMap();
	}

	void clearEFB(GXColor, u16, u16, bool) { }

	void makeHeightMap(f32 h);
	void makeNormalMap();
	void calcCoord();
	void clearHeightMap();

	virtual void prerender(JDrama::TGraphics* graphics,
	                       const TBathtubData& data, TBathWater** waters,
	                       TBathWaterParams** params, int num)
	{
		f32 negR = -data.unk3C;
		f32 R3   = 3.0f * data.unk3C;

		unk800AC = unk80134->meshTexWidth.get() & ~3;
		unk800B0 = 1.0f / (f32)unk800AC;

		// Half-stripped crap
		JGeometry::TVec3<f32> v1;
		v1.set(data.getThing());
		if (data.unk3C * data.unk3C - data.unk44 * data.unk44 <= 0)
			(void)(data.unk3C * data.unk3C - data.unk44 * data.unk44);

		JGeometry::TVec3<f32> v2;
		v2.set(data.getThing());

		f32 ey = v2.y + R3;
		JGeometry::TVec3<f32> v3;
		v3.set(data.getThing());

		JGeometry::TVec3<f32> dir(v3.x - v2.x, (v3.y + negR) - ey, v3.z - v2.z);
		JGeometry::TVec3<f32> up(0.0f, 0.0f, -1.0f);
		tmpFake(dir, up);

		unk80020.ref(0, 3) = -unk80020.at(0, 0) * v2.x - unk80020.at(0, 1) * ey
		                     - unk80020.at(0, 2) * v2.z;
		unk80020.ref(1, 3) = -unk80020.at(1, 0) * v2.x - unk80020.at(1, 1) * ey
		                     - unk80020.at(1, 2) * v2.z;
		unk80020.ref(2, 3) = -unk80020.at(2, 0) * v2.x - unk80020.at(2, 1) * ey
		                     - unk80020.at(2, 2) * v2.z;

		MtxPtr projMtx = gpCamera->unk16C;
		j3dSys.drawInit();
		GXSetViewport(0.0f, 0.0f, (f32)SMSGetGameRenderWidth(),
		              (f32)SMSGetGameRenderHeight(), 0.0f, 1.0f);

		TProjection3f proj;
		f32 halfW = 0.5f * unk80134->meshWidth.get();
		proj.orthographic(
		    halfW,
		    halfW
		        - unk800B0
		              * (unk80134->meshWidth.get()
		                 * (f32)SMSGetGameRenderHeight()),
		    -halfW,
		    unk800B0
		            * (unk80134->meshWidth.get() * (f32)SMSGetGameRenderWidth())
		        - halfW,
		    0.0f, R3 - negR);

		GXSetProjection(proj.mMtx, GX_ORTHOGRAPHIC);
		GXSetScissor(0, 0, unk800AC, unk800AC);

		GXSetNumChans(0);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetNumTexGens(1);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c,
		                  GX_FALSE, 0x7d);
		GXLoadTexObj(&unk80114, GX_TEXMAP0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_TEXC);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_FALSE);
		GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		GXSetCullMode(GX_CULL_BACK);

		J3DModelData* modelData = unk80144;
		J3DLoadArrayBasePtr(GX_VA_POS, modelData->getVtxPosArray());
		J3DLoadArrayBasePtr(GX_VA_NRM,
		                    modelData->getVertexData().getVtxNormArray());
		J3DLoadArrayBasePtr(GX_VA_CLR0,
		                    modelData->getVertexData().getVtxColorArray(0));
		GXCallDisplayList(modelData->getShapeNodePointer(0)->mGDCommands, 0xC0);
		GXLoadNrmMtxImm(unk80020, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);

		for (int i = 0; i < num; i++) {
			if (!params[i]->isVisible.get())
				continue;

			for (TBathWater::TDrop* d = waters[i]->unk88;
			     d < waters[i]->unk88 + waters[i]->unk74; d++) {
				TBathWaterParams* p = params[i];
				f32 t               = d->unk48;
				f32 rad             = p->dropRadius.get()
				          * (t * p->modelScale.get()
				             + (1.0f - t) * p->modelScale2.get());
				f32 px = d->unk0.x;
				f32 py = d->unk0.y
				         + (p->modelScaleY.get() * p->dropRadius.get() - rad);
				f32 pz = d->unk0.z;

				TPosition3f local;
				local.set(rad, 0.0f, 0.0f, px, 0.0f, rad, 0.0f, py, 0.0f, 0.0f,
				          rad, pz);
				TPosition3f mtx;
				mtx.concat(unk80020, local);
				GXLoadPosMtxImm(mtx, GX_PNMTX0);

				for (u16 s = 0; s < unk80144->getShapeNum(); s++) {
					J3DShape* shape = unk80144->getShapeNodePointer(s);
					for (u16 mg = 0; mg < shape->getMtxGroupNum(); mg++) {
						J3DShapeDraw* sd = shape->getShapeDraw(mg);
						if (sd)
							sd->draw();
					}
				}
			}
		}

		if (unk80134->showsCap.get() && !data.unk65) {
			GXLoadPosMtxImm(unk80020, GX_PNMTX0);
			GXClearVtxDesc();
			GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
			GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
			GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
			GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);
			JGeometry::TVec3<f32> capCenter = data.getThing();
			f32 rad = JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
			                                      - data.unk44 * data.unk44);
			drawCap(capCenter, rad);
			capCenter.y += 0.5f * negR;
			drawCap(capCenter, unk800B0 * unk80134->meshWidth.get() + rad);
		}

		GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);
		GXSetTexCopySrc(0, 0, unk800AC, unk800AC);
		GXSetTexCopyDst(unk800AC, unk800AC, GX_TF_Z24X8, GX_FALSE);
		GXCopyTex(unk800A4, GX_FALSE);
		GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
		GXSetProjection(projMtx, GX_PERSPECTIVE);
		GXSetScissor(0, 0, SMSGetGameRenderWidth(), SMSGetGameRenderHeight());
	}
	virtual void render(JDrama::TGraphics* graphics, const TBathtubData& data,
	                    TBathWater** waters, TBathWaterParams** params, int num)
	{
		CPolarSubCamera* cam = gpCamera;
		MtxPtr r30           = cam->getUnk1EC();
		MtxPtr r22           = cam->unk16C;
		s16 r28              = SMSGetGameRenderWidth();
		s16 r29              = SMSGetGameRenderHeight();
		clearEFB_alpha(0, 0, 0, 0, 0);
		MtxPtr mtx = gpCamera->unk1EC.mMtx;
		s16 r23    = SMSGetGameRenderWidth();
		s16 r24    = SMSGetGameRenderHeight();

		GXSetProjection(cam->getUnk16C(), GX_PERSPECTIVE);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);
		GXSetNumChans(0);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetNumTexGens(1);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c,
		                  GX_FALSE, 0x7d);
		GXLoadTexObj(&unk800B4, GX_TEXMAP0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_TEXA);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
		GXSetZCompLoc(GX_TRUE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetCullMode(GX_CULL_NONE);
		GXLoadPosMtxImm(mtx, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);

		f32 r0 = mtx[0][0];
		f32 r1 = mtx[0][1];
		f32 r2 = mtx[0][2];
		f32 u0 = mtx[1][0];
		f32 u1 = mtx[1][1];
		f32 u2 = mtx[1][2];

		for (int i = 0; i < num; ++i) {
			TBathWaterParams* p = params[i];
			f32 size            = p->texScale.get() * p->dropRadius.get();
			f32 rx = r0 * size, ry = r1 * size, rz = r2 * size;
			f32 ux = u0 * size, uy = u1 * size, uz = u2 * size;
			GXBegin(GX_QUADS, GX_VTXFMT0, (waters[i]->unk74 * 4) & 0xfffc);
			for (TBathWater::TDrop* d = waters[i]->unk88;
			     d < waters[i]->unk88 + waters[i]->unk74; ++d) {
				GXPosition3f32(d->unk0.x + rx, d->unk0.y + ry, d->unk0.z + rz);
				GXTexCoord2u8(0, 0);
				GXPosition3f32(d->unk0.x + ux, d->unk0.y + uy, d->unk0.z + uz);
				GXTexCoord2u8(0, 0x80);
				GXPosition3f32(d->unk0.x - rx, d->unk0.y - ry, d->unk0.z - rz);
				GXTexCoord2u8(0x80, 0x80);
				GXPosition3f32(d->unk0.x - ux, d->unk0.y - uy, d->unk0.z - uz);
				GXTexCoord2u8(0x80, 0);
			}
			GXEnd();
		}

		if (unk80134->showsCap.get() && !data.unk65) {
			drawCap(data.getThing(),
			        JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
			                                    - data.unk44 * data.unk44));
		}

		GXInitTexObj(&unk800D4, unk800A8, r23, r24, (GXTexFmt)1, GX_CLAMP,
		             GX_CLAMP, GX_FALSE);
		GXInitTexObjLOD(&unk800D4, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
		                GX_FALSE, GX_ANISO_1);
		GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);
		GXSetTexCopySrc(0, 0, r23, r24);
		GXSetTexCopyDst(r23, r24, (GXTexFmt)0x27, GX_FALSE);
		GXCopyTex(unk800A8, GX_FALSE);
		GXPixModeSync();
		GXSetProjection(r22, GX_PERSPECTIVE);

		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, 1,
		              GX_DF_CLAMP, GX_AF_NONE);
		GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);

		GXColor matColor
		    = (GXColor) { unk80134->polygonR.get(), unk80134->polygonG.get(),
			              unk80134->polygonB.get(), 0xFF };
		GXColor ambColor = (GXColor) { 0, 0, 0, 0xFF };
		GXSetChanMatColor(GX_COLOR0A0, matColor);
		GXSetChanAmbColor(GX_COLOR0A0, ambColor);

		GXSetNumTexGens(4);
		unk80138->load(GX_TEXMAP0);
		unk8013C->load(GX_TEXMAP1);
		unk80140->load(GX_TEXMAP2);
		GXLoadTexObj(&unk800D4, GX_TEXMAP3);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0,
		                  GX_FALSE, 0x7d);
		GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX1,
		                  GX_FALSE, 0x7d);
		GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX3x4, GX_TG_TEX0, GX_TEXMTX2,
		                  GX_FALSE, 0x7d);
		GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0,
		                  GX_FALSE, 0x7d);

		f32 cell = unk800B0 * unk80134->meshWidth.get();
		f32 tmp  = 3.0f * data.unk3C;
		unk80080.set(cell, -data.unk3C - tmp, cell);
		unk80050.identity33();

		JGeometry::TVec3<f32> pos = data.getThing();

		unk80050.mMtx[0][3] = -0.5f * unk80134->meshWidth.get() + pos.x;
		unk80050.mMtx[1][3] = pos.y + tmp;
		unk80050.mMtx[2][3] = -0.5f * unk80134->meshWidth.get() + pos.z;
		j3dSys.setViewMtx(r30);

		J3DTexGenBlock* tgb
		    = unk80148->getMaterialNodePointer(0)->getTexGenBlock();
		J3DTexMtx* tm0          = tgb->getTexMtx(0);
		tm0->mSRT.mScaleX       = 1.0f;
		tm0->mSRT.mScaleY       = 1.0f;
		tm0->mSRT.mTranslationX = 0.0f;
		tm0->mCenter.x          = 0.5f;
		tm0->mCenter.y          = 0.5f;
		J3DTexMtx* tm1          = tgb->getTexMtx(1);
		tm1->mSRT.mScaleX       = unk80134->envMapScale.get();
		tm1->mSRT.mScaleY       = unk80134->envMapScale.get();
		tm1->mSRT.mTranslationX = 0.0f;
		tm1->mSRT.mTranslationY = 0.0f;
		tm1->mCenter.x          = 0.5f;
		tm1->mCenter.y          = 0.5f;
		J3DTexMtx* tm2          = tgb->getTexMtx(2);
		tm2->mSRT.mScaleX       = unk80134->indTexScale.get();
		tm2->mSRT.mScaleY       = unk80134->indTexScale.get();
		s32 span                = unk80134->scrollSpan.get();
		tm2->mSRT.mTranslationY = (f32)(waters[0]->unk6C % span) / (f32)span;
		tm2->mCenter.x          = 0.5f;
		tm2->mCenter.y          = 0.5f;
		tgb->getTexMtx(1);

		TProjection3f effectMtx;
		SMS_GetLightPerspectiveForEffectMtx(effectMtx);
		tgb->getTexMtx(0)->setEffectMtx(effectMtx);
		tgb->calc(unk80050);
		GXLoadTexMtxImm(tgb->getTexMtx(0)->getTotalMtx(), GX_TEXMTX0,
		                GX_MTX3x4);
		GXLoadTexMtxImm(tgb->getTexMtx(1)->getTotalMtx(), GX_TEXMTX1,
		                GX_MTX3x4);
		GXLoadTexMtxImm(tgb->getTexMtx(2)->getTotalMtx(), GX_TEXMTX2,
		                GX_MTX3x4);

		GXSetNumIndStages(1);
		GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
		GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
		f32 indMtx[2][3];
		indMtx[0][0] = 0.6f;
		indMtx[0][1] = 0.0f;
		indMtx[0][2] = 0.0f;
		indMtx[1][0] = 0.0f;
		indMtx[1][1] = 0.6f;
		indMtx[1][2] = 0.0f;
		GXSetIndTexMtx(GX_ITM_0, indMtx, unk80134->indirectScale.get());

		if (unk80134->mode.get() == 2)
			GXSetNumTevStages(1);
		else
			GXSetNumTevStages(3);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
		switch (unk80134->mode.get()) {
		case 0:
			GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0,
			                GX_CC_KONST);
			break;
		case 1:
			GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
			                GX_CC_ZERO);
			break;
		case 2:
			GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO,
			                GX_CC_ZERO);
			break;
		}
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE,
		                GX_ITM_0);
		GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);

		GXSetTevKColor(GX_KCOLOR0, (GXColor) {
		                               unk80134->kRegR.get(),
		                               unk80134->kRegG.get(),
		                               unk80134->kRegB.get(),
		                               unk80134->kRegA.get(),
		                           });
		GXSetTevColor(GX_TEVREG1, (GXColor) {
		                              unk80134->regR.get(),
		                              unk80134->regG.get(),
		                              unk80134->regB.get(),
		                              unk80134->regA.get(),
		                          });
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC,
		                GX_CC_CPREV);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevIndWarp(GX_TEVSTAGE1, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE,
		                GX_ITM_0);
		GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
		GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevDirect(GX_TEVSTAGE2);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetZCompLoc(GX_FALSE);
		GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_FALSE);
		GXSetCullMode(GX_CULL_NONE);

		Mtx concat;
		PSMTXConcat(r30, unk80050, concat);
		DCInvalidateRange(unk800A4, unk800AC * (unk800AC * 4));
		GXLoadPosMtxImm(concat, GX_PNMTX0);
		GXLoadNrmMtxImm(r30, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
		GXSetCullMode(GX_CULL_BACK);
		GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
		if (unk80134->mask.get())
			GXSetAlphaCompare(GX_GEQUAL, unk80134->alpha.get(), GX_AOP_AND,
			                  GX_ALWAYS, 0);
		else
			GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

		makeHeightMap(unk80080.y);
		makeNormalMap();
		calcCoord();
		DCStoreRange(&unk20, 0x30000);
		DCStoreRange(&unk30020, 0x30000);
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
		GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
		GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
		GXSetArray(GX_VA_POS, &unk20, 0xc);
		GXSetArray(GX_VA_NRM, &unk30020, 0xc);
		GXSetArray(GX_VA_TEX0, &unk60020, 0x8);
		GXCallDisplayList(unk80154, unk80158);
		if (unk80134->showsMist.get())
			draw_mist(0, 0, r28, r29, unk80150);
		GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
	}
	virtual f32 getHeight(f32 x, f32 z) const
	{
		s16 n = unk800AC;
		if (n < 1)
			return 0.0f;

		int i = (int)((x - unk80050.at(0, 3)) / unk80080.x);
		if (i < 0)
			i = 0;
		else if (i >= n)
			i = n - 1;

		int j = (int)((z - unk80050.at(2, 3)) / unk80080.z);
		if (j < 0)
			j = 0;
		else if (j >= n)
			j = n - 1;

		return unk20[i][j].y + unk80050.at(1, 3);
	}

	void tmpFake(const JGeometry::TVec3<f32>& dir,
	             const JGeometry::TVec3<f32>& up)
	{
		unk80020.setLookDir(dir, up);
	}

public:
	/* 0x4 */ char unk4[0x20 - 0x4];
	/* 0x20 */ JGeometry::TVec3<f32> unk20[128][128];
	/* 0x30020 */ JGeometry::TVec3<f32> unk30020[128][128];
	/* 0x60020 */ JGeometry::TVec2<f32> unk60020[128][128];
	/* 0x80020 */ TPosition3f unk80020;
	/* 0x80050 */ TPosition3f unk80050;
	/* 0x80080 */ JGeometry::TVec3<f32> unk80080;
	/* 0x8008C */ char unk8008C[0x800A4 - 0x8008C];
	/* 0x800A4 */ void* unk800A4;
	/* 0x800A8 */ void* unk800A8;
	/* 0x800AC */ s16 unk800AC;
	/* 0x800AE */ s16 unk800AE;
	/* 0x800B0 */ f32 unk800B0;
	/* 0x800B4 */ GXTexObj unk800B4;
	/* 0x800D4 */ GXTexObj unk800D4;
	/* 0x800F4 */ GXTexObj unk800F4;
	/* 0x80114 */ GXTexObj unk80114;
	/* 0x80134 */ TBathWaterGlobalParams* unk80134;
	/* 0x80138 */ JUTTexture* unk80138;
	/* 0x8013C */ JUTTexture* unk8013C;
	/* 0x80140 */ JUTTexture* unk80140;
	/* 0x80144 */ J3DModelData* unk80144;
	/* 0x80148 */ J3DModelData* unk80148;
	/* 0x8014C */ J3DModel* unk8014C;
	/* 0x80150 */ void* unk80150;
	/* 0x80154 */ void* unk80154;
	/* 0x80158 */ int unk80158;
	/* 0x8015C */ char unk8015C[4];
};

TBathWaterPreprocessor::TBathWaterPreprocessor(TBathWaterManager* manager)
    : JDrama::TViewObj()
    , unk10(manager)
{
}

void TBathWaterPreprocessor::perform(u32 param, JDrama::TGraphics* graphics)
{
	if (param & 8)
		unk10->preprocess(graphics);
}

TBathWaterManager::TBathWaterManager()
    : JDrama::TViewObj("バスタブの水")
    , unk10(0)
    , unk34(this)
{
	unk14 = new TBathWaterParams*[2];
	unk18 = 0;
	unk1C = 0;
	unk20 = new TBathWater*[2];
	for (int i = 0; i < 2; i++) {
		unk20[i] = new TBathWater;
	}
	unk24 = 0;
	unk30 = 0;
}

const char* TBathWaterManager::fileNames[] = {
	"MapObj/bathwater_wave.prm",
	"MapObj/bathwater_overflow.prm",
};

void TBathWaterManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk18 = new TBathWaterGlobalParams();
	for (int i = 0; i < 2; ++i)
		unk14[i] = new TBathWaterParams(fileNames[i]);
}

void TBathWaterMeshRenderer::clearHeightMap()
{
	for (int i = 0; i < 128; ++i)
		for (int j = 0; j < 128; ++j)
			unk20[i][j].y = 0;

	unk80080.set(1.0f, 1.0f, 1.0f);
	unk80050.identity();
}

void TBathWaterMeshRenderer::makeHeightMap(f32 h)
{
	// GX_TF_Z24X8 depth texture (unk800AC^2 texels, 0x10000 buf): prerender
	// copies the drop-ball Z-buffer here; makeHeightMap decodes it (raw 4x4
	// GX tile layout: 64B/tile, two u16 planes) into the height field.

	s16 n = unk800AC >> 2;
	for (s16 br = 0; br < n; ++br) {
		for (s16 bc = 0; bc < n; ++bc) {
			u16* block = (u16*)unk800A4 + ((br + bc * n) << 5);
			for (s16 sr = 0; sr < 4; ++sr) {
				for (s16 sc = 0; sc < 4; ++sc) {
					s32 v = (((block + sr + sc * 4)[0] << 16) & 0xFF0000)
					        | (block + sr + sc * 4)[0x10];
					unk20[sr + br * 4][sc + bc * 4].y
					    = h * ((f32)v * (1.0f / 16777216.0f));
				}
			}
		}
	}
}

void TBathWaterMeshRenderer::makeNormalMap()
{
	f32 scale = -unk80134->meshWidth.get() / (f32)unk800AC;
	if (!unk80134->bendsNormal.get())
		scale *= 2.0f;

	for (int r = 0; r < unk800AC; ++r) {
		for (int c = 0; c < unk800AC; ++c) {
			f32 a  = unk20[r > 0 ? r - 1 : 0][c].y;
			f32 b  = unk20[r < unk800AC - 1 ? r + 1 : r][c].y;
			f32 a2 = unk20[r][c > 0 ? c - 1 : 0].y;
			f32 b2 = unk20[r][c < unk800AC - 1 ? c + 1 : c].y;

			unk30020[r][c].x = scale * (b - a);
			unk30020[r][c].y = scale * scale;
			unk30020[r][c].z = scale * (b2 - a2);
			unk30020[r][c].normalize();
		}
	}
}

void TBathWaterMeshRenderer::calcCoord()
{
	if (unk800AE != unk800AC) {
		unk800AE = unk800AC;

		for (int r = 0; r < unk800AE; ++r)
			for (int c = 0; c < unk800AE; ++c)
				unk60020[r][c].set((f32)r * unk800B0, (f32)c * unk800B0);

		DCStoreRange(&unk60020, 0x20000);

		for (int r = 0; r < unk800AE; ++r) {
			for (int c = 0; c < unk800AE; ++c) {
				unk20[r][c].x = unk80080.x * (f32)r;
				unk20[r][c].z = unk80080.z * (f32)c;
			}
		}

		GDLObj list;
		GDInitGDLObj(&list, unk80154, 0x30040);
		GDSetCurrent(&list);

		u16 vtxCount = unk800AC * (unk800AC - 1) * 2;
		GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vtxCount);

		for (int r = unk800AC - 1; r > 0; --r) {
			int rm = r - 1;
			for (int c = 0; c < unk800AC; ++c) {
				int a = r + c * 128;
				int b = rm + c * 128;
				GDPosition1x16(a);
				GDNormal1x16(a);
				GDTexCoord1x16(a);
				GDPosition1x16(b);
				GDNormal1x16(b);
				GDTexCoord1x16(b);
			}
		}

		GDEnd();

		GDPadCurr32();
		GDFlushCurrToMem();
		unk80158 = GDGetCurrOffset();
		GDSetCurrent(nullptr);
	}
}

void TBathWaterManager::loadAfter()
{
	TScreenTexture* tex
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	unk28[0] = new TBathWaterFlatRenderer(unk18);
	unk28[1] = new TBathWaterMeshRenderer(unk18, tex->getTexture());
	unk30    = unk28[1];
}

void TBathWaterManager::wave(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&,
                             f32, f32) const
{
}

void TBathWaterManager::initializeIfYet_()
{
	if (unk24 == nullptr) {
		TBathtub* bathtub = JDrama::TNameRefGen::search<TBathtub>("バスタブ");
		if (bathtub && bathtub->unk298) {
			const TBathtubData& data = bathtub->getBathtubData();
			for (int actor = 0; actor < 2; ++actor) {
				unk20[actor]->initialize(unk14[actor], data);

				for (int iter = 0; iter < 200; iter++)
					TBathWater::TDrop::calcWaterModel(unk20[actor], data);
			}
			unk24 = bathtub;
		}
	}
}

void TBathWaterManager::preprocess(JDrama::TGraphics* graphics)
{
	if (unk24 != 0 && unk30 != nullptr)
		unk30->prerender(graphics, unk24->getBathtubData(), unk20, unk14, 2);
}

f32 TBathWaterManager::getWaterHeight(f32 x, f32 z) const
{
	if (unk24 != 0 && unk30 != nullptr)
		return unk30->getHeight(x, z);
	return 0.0f;
}

namespace {
void CalcJumpVelocityY(const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&, f32, f32, f32,
                       JGeometry::TVec3<f32>*)
{
}
} // namespace

void TBathWaterManager::throwMario(f32 param_1)
{
	const TBathtubData& data = unk24->getBathtubData();

	JGeometry::TVec3<f32> diff;
	diff.sub(SMS_GetMarioPos(), data.mPos);

	JGeometry::TVec3<f32> local;
	data.unk18.mult33(diff, local);

	JGeometry::TVec3<f32> horiz;
	horiz   = local;
	horiz.y = 0.0f;

	JGeometry::TVec3<f32> vel;
	if (horiz.length() < 4500.0) {
		horiz.setLength(4150.0f);

		JGeometry::TVec3<f32> w;
		data.unk18.mult33(horiz, w);
		w += data.mPos;
		w.y += 120.0f;

		f32 gravity = SMS_GetMarioGravity();
		int count   = 1;
		f32 vy      = 100.0f;
		f32 y       = gpMarioPos->y;
		while (true) {
			y += vy;
			if (vy < 0.0f && y <= w.y)
				break;
			vy -= gravity;
			if (vy < -75.0f)
				vy = -75.0f;
			count++;
		}

		vel.x = (w.x - gpMarioPos->x) / (f32)count;
		vel.y = 100.0f;
		vel.z = (w.z - gpMarioPos->z) / (f32)count;
	} else {
		vel.x = 0.0f;
		vel.y = param_1;
		vel.z = 0.0f;
	}

	SMS_ThrowMario(vel, vel.length());
}

// TODO: needs more matching
static inline bool fakeCalcPos(const TBathtubData& data, f32 radius, f32 rnd1,
                               JGeometry::TVec3<f32>* out)
{

	JGeometry::TVec3<f32> axis;
	axis.set(data.unk18.at(1, 0), 0.0f, data.unk18.at(1, 2));

	if (axis.isZero())
		return false;

	JGeometry::TVec3<f32> nAxis;
	nAxis.normalize(axis);

	JGeometry::TVec3<f32> cross;
	cross.cross(nAxis, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	cross.normalize();

	f32 perturb = 0.18f * rnd1;
	nAxis.x += cross.x * perturb;
	nAxis.y += cross.y * perturb;
	nAxis.z += cross.z * perturb;

	f32 h = 0.9f * (data.unk3C - radius);
	nAxis.setLength(h);

	JGeometry::TVec3<f32> up2(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> center;
	center.set(data.getThing());
	out->set(up2.x * radius + nAxis.x + center.x,
	         up2.y * radius + nAxis.y + center.y,
	         up2.z * radius + nAxis.z + center.z);
	return true;
}

void TBathWaterManager::perform(u32 param, JDrama::TGraphics* graphics)
{
	if (unk24 == nullptr) {
		initializeIfYet_();
		return;
	}

	if (param & 1) {
		unk30 = unk28[unk18->displaysMesh.get()];
		unk1C += 1;
		if (!(unk1C & 3)) {
			for (int actor = 0; actor < 2; ++actor) {
				TBathWater* bw           = unk20[actor];
				const TBathtubData& data = unk24->getBathtubData();

				TBathWater::TDrop::calcWaterModel(bw, data);
			}

			if (!unk24->unk29A) {
				for (int i = 0; i < 2; ++i) {
					if (!SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER)
					    && unk14[i]->checksMario.get()) {
						if (unk20[i]->tryHitMario(SMS_GetMarioHitActor()))
							throwMario(unk14[i]->jump.get());
					}
				}

				if (!SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER)) {
					if (unk20[0]->tryHitMario2(SMS_GetMarioHitActor(),
					                           unk24->getBathtubData()))
						throwMario(unk14[0]->jump.get());
				}
			}

			// Periodic jump-drop spawn.
			if ((unk1C & 7) == 4 && unk24->getBathtubData().unk64) {
				const TBathtubData& data = unk24->getBathtubData();

				JGeometry::TVec3<f32> vel;
				if (fakeCalcPos(data, unk14[1]->dropRadius.get(),
				                unk10.get_float(-1.0f, 1.0f), &vel))
					unk20[1]->addDrop(vel, unk10.get_float01());
			}

			TBathWater* soundBw = unk20[0];
			JGeometry::TVec3<f32> avg;
			avg.set(soundBw->unk78);
			f32 tmp = soundBw->unk84;
			if (tmp > 0.0f)
				SMSGetMSound()->startSoundActorWithInfo(MSD_SE_BS_KOOPA_FLOOD,
				                                        &avg, nullptr, tmp, 0,
				                                        0, nullptr, 0, 4);
		}
	}

	if (param & 8)
		unk30->prerender(graphics, unk24->getBathtubData(), unk20, unk14, 2);
}
