#include <MoveBG/MapObjCloud.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Enemy/Graph.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TRideCloud::mMoveSpeedDefault = 2.0f;
f32 TRideCloud::mCushionLen       = 80.0f;
f32 TRideCloud::mCushionSpeed     = 0.015f;
f32 TRideCloud::mScaleSpeed       = 0.005f;

TRideCloud::TRideCloud(const char* name)
    : TRailMapObj(name)
    , unk150(0.0f)
    , unk154(0.0f)
    , unk158(0)
    , unk15C(0.0f)
    , unk16C(0)
{
	unk15C = mMoveSpeedDefault;
	unk160 = 1.0f;
	unk164 = 1.0f;
	unk168 = 1.0f;

	unk16E.r = 0;
	unk16E.g = 0;
	unk16E.b = 0;
	unk16E.a = 0;
}

u32 TRideCloud::touchWater(THitActor*)
{
	unk158 += 10;
	if (unk158 > 600)
		unk158 = 600;
	return 1;
}

// The exact sibling `TRailMapObj::setGroundCollision` puts the scratch matrix
// at 0x18 in the same 0x50 frame and differs only by a guard; the four bytes
// of low region that guard's inlined reads make are supplied here by reading
// `mMapCollisionManager` through one level that binds its result. This stands
// in for a `TMapObjBase::getMapCollisionManager()` accessor of that shape, but
// `MoveBG/MapObjBase.hpp` is shared with source-linked TUs, so it is parked
// here and reported.
// Measured, all at frame 0x50 unless noted: a binding level on the collision
// pointer itself or on `getAnmMtx(0)` is +8 (matrix at 0x20, frame 0x58), a
// non-binding `RideCloudSetMtx(mtx, src)` wrapper is -8, and `getUnk8()` on
// the inner pointer is +8. Worth zero: `TPosition3f` instead of `TMtx34f`, a
// named `MtxPtr` or `J3DModel*`, function-scope instead of block-scope for the
// matrix, an early return, and `mMapCollisionManager->unk8->moveMtx(mtx)` in
// place of the binding `if`. `mtx.set(*(const TMtx34f*)...)` also lands it (it
// routes through the `set(const SMatrix34C&)` overload, whose extra conversion
// binding is the same +4) but needs a reinterpret cast.
static inline TMapCollisionManager* RideCloudCollisionManager(TRideCloud* cloud)
{
	TMapCollisionManager* manager = cloud->mMapCollisionManager;
	return manager;
}

void TRideCloud::setGroundCollision()
{
	if (RideCloudCollisionManager(this)) {
		// TODO: this is used in MapObjRailBlock too, inline global?
		TMtx34f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (TMapCollisionBase* col = mMapCollisionManager->unk8)
			col->moveMtx(mtx);
	}
}

void TRideCloud::calcRootMatrix()
{
	f32 fVar2 = JMAHermiteInterpolation(unk150, 0.0f, 0.0f, 0.0f, 1.0f,
	                                    -mCushionLen, 0.0f);

	MsMtxSetTRS(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y + fVar2,
	            mPosition.z, mRotation.x, mRotation.y, mRotation.z,
	            mScaling.x * unk160, mScaling.y * unk164, mScaling.z * unk168);
}

void TRideCloud::initMapObj()
{
	TRailMapObj::initMapObj();
	unk154 = mScaledBodyRadius;
}

void TRideCloud::load(JSUMemoryInputStream& stream)
{
	TRailMapObj::load(stream);
	u32 r, g, b, a;
	stream >> r >> g >> b >> a;
	unk16E.r = r & 0xff;
	unk16E.g = g & 0xff;
	unk16E.b = b & 0xff;
	unk16E.a = 0xff;
	stream >> r >> g >> b >> a;
	unk176.r = r & 0xff;
	unk176.g = g & 0xff;
	unk176.b = b & 0xff;
	unk176.a = 0xff;
	SMS_InitPacket_TwoTevColor(getModel(), 0, GX_TEVREG0, &unk16E, GX_TEVREG1,
	                           &unk176);
	offLiveFlag(LIVE_FLAG_UNK400);
	onLiveFlag(LIVE_FLAG_UNK8);
}

u32 TRideCloud::getShadowType() { return SHADOW_TYPE_CIRCLE; }

// TODO: 189 of 193 instructions and the frame are exact; what is left is a
// three-register FPR permutation in `mDamageRadius`' two multiplies, where
// retail parks the 300.0f literal in f2 (the register `fVar8` has just freed)
// and unk160 in f1, and we get f1/f2. `getScaling()` fixed the `mScaling.x`
// half of it (that read really does go through the accessor: it puts the load
// in retail's f0); the literal's register resists all eight groupings and
// operand orders of the product, a named intermediate, `*=`, and a binding
// level on unk160.
//
// The 56 bytes of dead low region this body was missing are five inline
// expansions, each a member read through one level that binds its result.
// Measured from the 0x90 base: `mMapCollisionManager` +8, `checkRailFlag`
// (both sites) +8, `unk138->getGraph()` +8, `unk138->getCurrent()` +0x10,
// `getScaling().x` +8 = retail's 0xc8 exactly; also +8 each and
// interchangeable with the last one: `getCurGraphIndex()`, any single
// `node.getRailNode()` site (all three are +0x20). Worth zero:
// `SMS_GetMarioSpeedY()` alone (+8 paired with the collision-manager level).
// Worse: a binding level on `MsClamp<f32>` (+2 instructions),
// `node.checkFlag()` over `getRailNode()->mFlags` (-8).
// As in setGroundCollision these stand in for accessors that bind their
// result on TMapObjBase/TRailMapObj/TGraphTracer, whose headers are shared
// with source-linked TUs, so they are parked here and reported.
static inline bool RideCloudRailFlag(TRideCloud* cloud, u32 flag)
{
	bool set = cloud->checkRailFlag(flag);
	return set;
}

static inline TGraphWeb* RideCloudGraph(TRideCloud* cloud)
{
	TGraphWeb* graph = cloud->unk138->getGraph();
	return graph;
}

static inline TGraphNode& RideCloudCurrentNode(TRideCloud* cloud)
{
	TGraphNode& node = cloud->unk138->getCurrent();
	return node;
}

void TRideCloud::control()
{
	TMapObjBase::control();
	TMapCollisionBase* col = RideCloudCollisionManager(this)->unk8;
	if (*gpMarioSpeedY > 0.0f)
		col->setAllBGType(0x400);
	else
		col->setAllBGType(0);

	checkMarioRiding();
	if (!RideCloudRailFlag(this, 0x1)) {
		unk150 = MsClamp<f32>(unk150 - mCushionSpeed, 0.0f, 1.0f);
	} else {
		unk150 = MsClamp<f32>(unk150 + mCushionSpeed, 0.0f, 1.0f);
	}

	f32 fVar8;
	if (unk158 > 0) {
		fVar8 = mScaleSpeed;
		--unk158;
	} else {
		fVar8 = -mScaleSpeed;
	}

	unk160        = MsClamp<f32>(unk160 + fVar8, 1.0f, 3.0f);
	unk168        = MsClamp<f32>(unk168 + fVar8, 1.0f, 3.0f);
	mDamageRadius = getScaling().x * 300.0f * unk160;
	mDamageHeight = 50.0f;
	calcEntryRadius();
	if (!calcRecycle() && !RideCloudRailFlag(this, 0x2)) {
		if (unk16C != 0) {
			--unk16C;
		} else {
			// The graph has to be named: retail keeps it in r28-r31
			// across moveToNextNode/moveTo and reads the node array
			// back out of it for node2, where re-reading unk138->unk0
			// costs an extra load and one callee-saved register.
			TGraphWeb* graph = RideCloudGraph(this);
			if (!graph || graph->isDummy())
				return;

			if (moveToNextNode(unk15C)) {
				TGraphNode& node = RideCloudCurrentNode(this);

				if ((node.getRailNode()->mFlags & 0x1000)) {
					unk14A = 180;
					unk148 = 2;
				}

				unk138->moveToShortestNext();

				if (node.getRailNode()->mFlags & 0x800)
					unk16C = node.getRailNode()->mPitch;

				TRailNode* node2
				    = graph->getGraphNode(unk138->getCurGraphIndex())
				          .getRailNode();
				if (node2->mYaw != 0xffff)
					unk15C = node2->mYaw * 0.01f;

				resetStep(unk15C);
			}
		}
	}
}

void TRideCloud::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_VIEW)
		mScaledBodyRadius = unk154 * mScaling.x;
	TRailMapObj::perform(cue, graphics);
}
