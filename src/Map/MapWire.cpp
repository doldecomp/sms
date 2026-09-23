#include <Map/MapWire.hpp>

#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include <fake_tgmath.h>
#include <types.h>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JMath.hpp>
#include <Camera/CubeMapTool.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

TMapWirePoint::TMapWirePoint()
{
	mPosOnWire     = 0.0f;
	mPosReturnRate = 0.0f;
	mPosition.zero();
	mDefaultPosition.zero();
}

f32 TMapWire::mMoveTimerSpeed = 0.03f;
f32 TMapWire::mDownRateMax    = 0.003f;
f32 TMapWire::mEndRate        = 0.001f;
f32 TMapWire::mStretchRate    = 2.0f;
f32 TMapWire::mHeightRate     = 1.3f;
f32 TMapWire::mReleaseHeight  = 100.0f;
f32 TMapWire::mFootLength     = 26.0f;
f32 TMapWire::mDrawWidth      = 5.0f;
f32 TMapWire::mDrawHeight     = 6.0f;

// Retail adds the point before the offset inside the strip loops, which only
// happens when the vertex goes through a helper taking the point by reference.
static inline void addPoint(const JGeometry::TVec3<f32>& p, f32 dx, f32 dz)
{
	GXPosition3f32(p.x + dx, p.y, p.z + dz);
}

static inline void subPoint(const JGeometry::TVec3<f32>& p, f32 dx, f32 dz)
{
	GXPosition3f32(p.x - dx, p.y, p.z - dz);
}

static inline void downPoint(const JGeometry::TVec3<f32>& p, f32 h)
{
	GXPosition3f32(p.x, p.y - h, p.z);
}

// TODO: frame is 0x70, retail 0x78; helper placement on the start/end points
// moves it in 8-byte steps but no tried split reaches 0x78.
void TMapWire::drawLower() const
{
	f32 xOffset = mDrawAxes.x;
	xOffset *= mDrawWidth;
	f32 zOffset = mDrawAxes.y;
	zOffset *= mDrawWidth;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	subPoint(mStartPoint, xOffset, zOffset);
	downPoint(getStartPoint(), mDrawHeight);

	for (int i = 0; i < mNumActiveMapWirePoints; i++) {
		subPoint(mMapWirePoints[i].mPosition, xOffset, zOffset);
		downPoint(mMapWirePoints[i].mPosition, mDrawHeight);
	}

	subPoint(mEndPoint, xOffset, zOffset);
	downPoint(mEndPoint, mDrawHeight);

	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	downPoint(mStartPoint, mDrawHeight);
	addPoint(mStartPoint, xOffset, zOffset);

	for (int i = 0; i < mNumActiveMapWirePoints; i++) {
		downPoint(mMapWirePoints[i].mPosition, mDrawHeight);
		addPoint(mMapWirePoints[i].mPosition, xOffset, zOffset);
	}

	downPoint(mEndPoint, mDrawHeight);
	addPoint(mEndPoint, xOffset, zOffset);

	GXEnd();
}

// TODO: frame is 0x40, retail 0x58; helpers on the start/end points, a
// const getPoint(), and TVec2/TVec3 offset locals were inert or wrong.
// lever-search closes it only with a mixed spelling (a named
// `startPoint` ref for the first vertex's x/z, getStartPoint() elsewhere):
// docs/progress/lever-search/mapwire_drawupper.patch. Not applied as
// implausible; addPoint/subPoint on either spelling stays at 99.9.
void TMapWire::drawUpper() const
{
	f32 xOffset = mDrawAxes.x;
	xOffset *= mDrawWidth;
	f32 zOffset = mDrawAxes.y;
	zOffset *= mDrawWidth;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	GXPosition3f32(mStartPoint.x + xOffset, mStartPoint.y,
	               mStartPoint.z + zOffset);
	GXPosition3f32(mStartPoint.x - xOffset, mStartPoint.y,
	               mStartPoint.z - zOffset);

	for (int index = 0; index < mNumActiveMapWirePoints; index++) {
		addPoint(mMapWirePoints[index].mPosition, xOffset, zOffset);
		subPoint(mMapWirePoints[index].mPosition, xOffset, zOffset);
	}

	GXPosition3f32(mEndPoint.x + xOffset, mEndPoint.y, mEndPoint.z + zOffset);
	GXPosition3f32(mEndPoint.x - xOffset, mEndPoint.y, mEndPoint.z - zOffset);

	GXEnd();
}

f32 TMapWire::getPointPowerAtReleased(f32 pos) const
{
	// 1 = default height, 0 = stretched all the way down
	f32 relativeHeightAtPos;
	if (pos >= mHangPos) {
		relativeHeightAtPos = (pos - mHangPos) / (1.0f - mHangPos);
	} else {
		relativeHeightAtPos = 1.0f - pos / mHangPos;
	}

	f32 power = 1.0f - relativeHeightAtPos * relativeHeightAtPos;
	return power;
}

void TMapWire::getPointPosAtReleased(f32 pos, JGeometry::TVec3<f32>* out) const
{
	JGeometry::TVec3<f32> linePoint;
	getPointPosOnLine(pos, &linePoint);

	JGeometry::TVec3<f32> defaultPoint;
	getPointPosDefault(pos, &defaultPoint);

	// TODO: fix this inlining issue
	f32 power = getPointPowerAtReleased(pos);
	// TODO: Regswaps for these calculations?
	f32 yAdjusted
	    = linePoint.y
	      + (1.0f - mBounceRemainingPower) * (defaultPoint.y - linePoint.y)
	      + power * mHangOrBouncePoint.y;

	out->set(linePoint.x, yAdjusted, linePoint.z);
}

void TMapWire::updatePointAtReleased(int index)
{
	TMapWirePoint* mapWirePoint = &mMapWirePoints[index];

	f32 pos = mapWirePoint->mDefaultPosOnWire;
	if (fabsf(pos - mapWirePoint->mPosOnWire)
	    > fabsf(mapWirePoint->mPosReturnRate)) {
		pos = mapWirePoint->mPosOnWire + mapWirePoint->mPosReturnRate;
	}

	getPointPosAtReleased(pos, &mapWirePoint->mPosition);
}

bool TMapWire::updateMovePointAtReleased()
{
	mBounceRemainingPower -= mBounceDecayRate;

	if (mBounceRemainingPower < TMapWire::mEndRate)
		return true;

	mMoveTimer += TMapWire::mMoveTimerSpeed;
	if (mMoveTimer >= 2.0f) {
		mMoveTimer -= 2.0f;
	}

	mHangOrBouncePoint.y = mBounceAmplitude * JMASCos(mMoveTimer * 32768.0f)
	                       * mBounceRemainingPower;
	return false;
}

void TMapWire::initPointAtJustReleased(f32 pos, TMapWirePoint* point)
{
	point->mPosOnWire = pos;
	getPointPosAtReleased(pos, &point->mPosition);
	point->mPosReturnRate = (point->mDefaultPosOnWire - pos) / 1000.0f;
}

// TODO: 99.8%: frame 0x138 vs ours 0xc8 (each getPointPosAtReleased
// expansion's linePoint/defaultPoint pair sits 0x28 apart in retail, 0x18 in
// ours, 0x3c more pool below) and the gpMarioSpeedX/Z pointer registers swap.
// Tried: power/yAdjusted named vs inline, linePoint/defaultPoint declared
// together (inert); component stores for the result (worse, 97.6).
void TMapWire::release()
{
	if (mState == TMapWire::RELEASED)
		return;

	mState = TMapWire::RELEASED;

	mHangOrBouncePoint.zero();

	mNumActiveMapWirePoints = mNumMapWirePoints;

	int halfNumPoints = mNumActiveMapWirePoints / 2;

	if (halfNumPoints != 0) {
		f32 posAdvancePerPoint = mHangPos / halfNumPoints;

		for (int i = 0; i < halfNumPoints; i++) {
			mMapWirePoints[i].reset();

			initPointAtJustReleased(posAdvancePerPoint * (i + 1),
			                        &mMapWirePoints[i]);
		}
	}

	if (mNumActiveMapWirePoints - halfNumPoints != 0) {
		f32 posAdvancePerPoint
		    = (1.0f - mHangPos) / (mNumActiveMapWirePoints - halfNumPoints);

		for (int i = halfNumPoints; i < mNumActiveMapWirePoints; i++) {
			mMapWirePoints[i].reset();

			initPointAtJustReleased(posAdvancePerPoint * (i - halfNumPoints + 1)
			                            + mHangPos,
			                        &mMapWirePoints[i]);
		}
	}

	f32 stretchRatio = mStretchRate * abs(mHangPos - 0.5f);

	if (*gpMarioSpeedY > 0) {
		JGeometry::TVec3<f32> marioVel(*gpMarioSpeedX, *gpMarioSpeedY,
		                               *gpMarioSpeedZ);
		mBounceAmplitude = mHeightRate * marioVel.length();
	} else {
		mBounceAmplitude = mReleaseHeight;
	}

	mBounceDecayRate      = mDownRateMax * stretchRatio;
	mBounceRemainingPower = 1.0f;
	mMoveTimer            = 1.0f;
}

void TMapWire::getPointPosAtHanged(f32 pos, JGeometry::TVec3<f32>* out) const
{
	f32 posOffset = pos - mHangPos;

	out->x = mWireSpan.x * posOffset + mHangOrBouncePoint.x;
	out->z = mWireSpan.z * posOffset + mHangOrBouncePoint.z;

	if (pos <= mHangReferencePos1) {
		out->y = mHangOrBouncePoint.y
		         + ((mStartPoint.y - mHangOrBouncePoint.y)
		            * (mHangReferencePos1 - pos))
		               / mHangReferencePos1;
	} else if (pos >= mHangReferencePos2) {
		out->y = mHangOrBouncePoint.y
		         + ((mEndPoint.y - mHangOrBouncePoint.y)
		            * (pos - mHangReferencePos2))
		               / (1.0f - mHangReferencePos2);
	} else {
		out->y = mHangOrBouncePoint.y;
	}
}

void TMapWire::getPointInfoAtHanged(f32 pos, TMapWirePoint* point)
{
	point->mPosOnWire = pos;

	JGeometry::TVec3<f32> outPoint;
	getPointPosAtHanged(pos, &outPoint);
	point->mPosition.set(outPoint.x, outPoint.y, outPoint.z);
}

void TMapWire::setFootPointsAtHanged(MtxPtr mtx)
{
	mState = TMapWire::HANGING;

	mHangOrBouncePoint.set(mtx[0][3], mtx[1][3],
	                       mtx[2][3]); // translate portion of matrix
	mHangPos = getPosInWire(mHangOrBouncePoint);

	mHangReferencePos1 = mHangPos - mFootLength / mWireLength;
	mHangReferencePos2 = mHangPos + mFootLength / mWireLength;

	mNumActiveMapWirePoints = 2;

	TMapWirePoint* refPoints = &mMapWirePoints[0];
	if (mFootLength < mHangPos * mWireLength) {
		getPointInfoAtHanged(mHangReferencePos1, &refPoints[0]);
	} else {
		refPoints[0].mPosOnWire = mHangPos;
		refPoints[0].mPosition.set(mHangOrBouncePoint.x, mHangOrBouncePoint.y,
		                           mHangOrBouncePoint.z);
	}

	if (mFootLength < (1.0f - mHangPos) * mWireLength) {
		getPointInfoAtHanged(mHangReferencePos2, &refPoints[1]);
	} else {
		refPoints[1].mPosOnWire = mHangPos;
		refPoints[1].mPosition.set(mHangOrBouncePoint.x, mHangOrBouncePoint.y,
		                           mHangOrBouncePoint.z);
	}
}

void TMapWire::calcViewAndDBEntry()
{
	mStartFittingModel->viewCalc();
	mEndFittingModel->viewCalc();
}

// TODO: 99.6%: frame 0xd8 vs ours 0x78 (the same getPointPosAtReleased pool
// gap as release) and the JMASCos product lands in f1 in retail, f0 in ours;
// operand order, a named cos result and a named s16 angle are all inert.
void TMapWire::move()
{
	switch (mState) {
	case IDLE:
		break;

	case HANGING:
		break;

	case RELEASED:
		if (updateMovePointAtReleased()) {
			TMapWirePoint* mapWirePoint;

			for (int i = 0; i < mNumActiveMapWirePoints; i++) {
				mapWirePoint = &mMapWirePoints[i];
				mapWirePoint->reset();
			}

			mState = TMapWire::IDLE;
		} else {
			for (int i = 0; i < mNumActiveMapWirePoints; i++) {
				updatePointAtReleased(i);
			}
		}
	}
}

f32 TMapWire::getPosInWire(const JGeometry::TVec3<f32>& point) const
{
	// TODO: This needs stack offset adjustments

	// Position here is only considered in the horizontal plane
	JGeometry::TVec3<f32> flatStart = mStartPoint;
	JGeometry::TVec3<f32> flatEnd   = mEndPoint;
	flatStart.y                     = 0.0f;
	flatEnd.y                       = 0.0f;

	JGeometry::TVec3<f32> perpPoint
	    = MsPerpendicFootToLineR(flatStart, flatEnd, point);

	// TODO: retail names both differences (`TVec3 span = flatEnd - flatStart;`
	// then `span.length()`): the inlined expansion in setFootPointsAtHanged
	// copies the `operator-` temporary into a 12-byte named slot twice, and
	// writing it that way takes that function 92.43 -> 99.59 with every opcode
	// exact. It cannot be committed while `operator-` forwards through
	// `operator-=`: the named store is one more inline level, so `sub` drops
	// out of line here as well (85.94 -> 40.42) where retail still expands it.
	// Retail's `operator-` must reach `sub` one level sooner -- a shared-header
	// change (JGVec3.hpp).
	f32 totalLength   = (flatEnd - flatStart).length();
	f32 partialLength = (perpPoint - flatStart).length();
	return partialLength / totalLength;
}

/**
 * @brief Gets a position on the straight line connecting the wire's endpoints.
 *
 * @param pos the relative position on the wire (0 to 1)
 * @param out the output vector
 */
// TODO: retail spells the products span-first (`fmadds span, pos, start`).
// `out->set(...)` here is refuted: it lands move()'s out-of-line `bl set<f>`
// (84.3 -> 85.4) but costs getPointPosOnWire 95.8 -> 48.9, so the
// out-of-line `set` in move()/release() must come from a deeper expansion.
void TMapWire::getPointPosOnLine(f32 pos, JGeometry::TVec3<f32>* out) const
{
	out->set(mWireSpan.x * pos + mStartPoint.x, mWireSpan.y * pos + mStartPoint.y,
	         mWireSpan.z * pos + mStartPoint.z);
}

// TODO: 95.8%, frame 0x78 vs 0x68 and the line point's x/z order. Component
// stores in getPointPosAtReleased fix this frame (0x68) but shrink release,
// move and init further from retail, so the shared helper is left as is (k5).
void TMapWire::getPointPosOnWire(f32 pos, JGeometry::TVec3<f32>* out) const
{
	if (pos < 0.0f) {
		pos = 0.0f;
	}
	if (pos > 1.0f) {
		pos = 1.0f;
	}

	if (mState == TMapWire::HANGING) {
		getPointPosAtHanged(pos, out);
	} else {
		getPointPosAtReleased(pos, out);
	}
}

/**
 * @brief The "default" position of a point on this wire after accounting for
 * its sag factor.
 *
 * @param pos the relative position on the wire (0 to 1)
 * @param out the output vector
 */
void TMapWire::getPointPosDefault(f32 pos, JGeometry::TVec3<f32>* out) const
{
	out->set(mWireSpan.x * pos + mStartPoint.x,
	         mWireSpan.y * pos + mStartPoint.y
	             - mWireSag * JMASSin(pos * 32768.0f),
	         mWireSpan.z * pos + mStartPoint.z);
}

void TMapWire::initTipPoints(const TCubeGeneralInfo* cubeInfo)
{
	JGeometry::TVec3<f32> halfWire(0.0f, 0.0f, mWireLength * 0.5f);

	JGeometry::TRotation3<TMtx33f> wireTransform;
	wireTransform.identity();
	wireTransform.setEular((s16)(cubeInfo->getUnk18().x / 180.0f * 32768.0f),
	                       (s16)(cubeInfo->getUnk18().y / 180.0f * 32768.0f),
	                       (s16)(cubeInfo->getUnk18().z / 180.0f * 32768.0f));

	wireTransform.mult33(halfWire);

	mStartPoint.set(cubeInfo->getUnkC().x - halfWire.x,
	                cubeInfo->getUnkC().y - halfWire.y + cubeInfo->getUnk24().y,
	                cubeInfo->getUnkC().z - halfWire.z);

	mEndPoint.set(halfWire.x + cubeInfo->getUnkC().x,
	              halfWire.y + cubeInfo->getUnkC().y + cubeInfo->getUnk24().y,
	              halfWire.z + cubeInfo->getUnkC().z);

	mWireSpan = mEndPoint - mStartPoint;
}

// TODO: Needs work, but otherwise mathematically equivalent
void TMapWire::init(const TCubeGeneralInfo* cubeInfo)
{
	mNumMapWirePoints = (s32)((cubeInfo->getUnk24().z / 50.0f + 1.0f) - 2.0f);
	mNumActiveMapWirePoints = mNumMapWirePoints;

	mMapWirePoints = new TMapWirePoint[mNumMapWirePoints];

	mWireLength = cubeInfo->getUnk24().z;

	initTipPoints(cubeInfo);

	mWireSag = cubeInfo->getUnk24().y * 0.5f;

	for (int i = 0; i < mNumMapWirePoints; i++) {
		// Inline suspect
		{
			f32 pos              = (f32)(i + 1) / (f32)(mNumMapWirePoints);
			TMapWirePoint* point = &mMapWirePoints[i];
			point->mPosOnWire = point->mDefaultPosOnWire = pos;
		}

		TMapWirePoint* point2 = &mMapWirePoints[i];
		getPointPosDefault(point2->mPosOnWire, &point2->mDefaultPosition);

		point2->reset();
	}

	if (mEndPoint.x != mStartPoint.x) {
		f32 angle   = atanf((mEndPoint.z - mStartPoint.z)
		                    / (mEndPoint.x - mStartPoint.x));
		mWireHAngle = -angle * 180.0f / M_PI + 90.0f;
	} else {
		mWireHAngle = 0.0f;
	}

	mDrawAxes.set(mEndPoint.x - mStartPoint.x, mEndPoint.z - mStartPoint.z);
	mDrawAxes.normalize();
	mDrawAxes.rotate(M_PI / 2);

	mStartFittingModel
	    = SMS_CreatePartsModel("/common/map/WireFitting.bmd",
	                           J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	                               | (1 << J3DMLF_TevStageNumShift));
	mEndFittingModel
	    = new J3DModel(mStartFittingModel->getModelData(),
	                   J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	                       | (1 << J3DMLF_TevStageNumShift),
	                   1);

	Mtx mtx;

	MsMtxSetXYZRPH(mtx, mStartPoint.x, mStartPoint.y, mStartPoint.z,
	               cubeInfo->getUnk18().x, cubeInfo->getUnk18().y,
	               cubeInfo->getUnk18().z);
	mStartFittingModel->setBaseTRMtx(mtx);
	mStartFittingModel->calc();

	MsMtxSetXYZRPH(mtx, mEndPoint.x, mEndPoint.y, mEndPoint.z,
	               cubeInfo->getUnk18().x, cubeInfo->getUnk18().y + 180.0f,
	               cubeInfo->getUnk18().z);
	mEndFittingModel->setBaseTRMtx(mtx);
	mEndFittingModel->calc();

	gpMapObjManager->entryStaticDrawBufferSun(mStartFittingModel);
	gpMapObjManager->entryStaticDrawBufferSun(mEndFittingModel);

	TMapCollisionStatic* collision1 = new TMapCollisionStatic;
	collision1->init("/common/map/WireFitting.col", 2, nullptr);
	collision1->setUpMtx(mStartFittingModel->getAnmMtx(0));

	TMapCollisionStatic* collision2 = new TMapCollisionStatic;
	collision2->init("/common/map/WireFitting.col", 2, nullptr);
	collision2->setUpMtx(mEndFittingModel->getAnmMtx(0));
}

TMapWire::TMapWire()
{
	mWireHAngle             = 0.0f;
	mWireSag                = 0.0f;
	mNumActiveMapWirePoints = 0;
	mNumMapWirePoints       = 0;
	mMapWirePoints          = nullptr;
	mHangPos                = 0.0f;
	mMoveTimer              = 0.0f;
	mBounceRemainingPower   = 0.0f;
	mBounceAmplitude        = 0.0f;
	mHangReferencePos1      = 0.0f;
	mHangReferencePos2      = 0.0f;
	mState                  = TMapWire::IDLE;
	mStartPoint.zero();
	mEndPoint.zero();
	mWireSpan.zero();
	mDrawAxes.zero();
	mHangOrBouncePoint.zero();
	mStartFittingModel = nullptr;
	mEndFittingModel   = nullptr;
}
