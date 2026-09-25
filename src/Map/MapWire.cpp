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

void TMapWire::drawLower() const
{
	f32 xOffset = mDrawAxes.x;
	f32 zOffset = mDrawAxes.y;
	xOffset *= mDrawWidth;
	zOffset *= mDrawWidth;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	GXPosition3f32(mStartPoint.x - xOffset, mStartPoint.y,
	               mStartPoint.z - zOffset);
	GXPosition3f32(mStartPoint.x, mStartPoint.y - mDrawHeight, mStartPoint.z);

	for (int i = 0; i < mNumActiveMapWirePoints; i++) {
		GXPosition3f32(mMapWirePoints[i].mPosition.x - xOffset,
		               mMapWirePoints[i].mPosition.y,
		               mMapWirePoints[i].mPosition.z - zOffset);
		GXPosition3f32(mMapWirePoints[i].mPosition.x,
		               mMapWirePoints[i].mPosition.y - mDrawHeight,
		               mMapWirePoints[i].mPosition.z);
	}

	GXPosition3f32(mEndPoint.x - xOffset, mEndPoint.y, mEndPoint.z - zOffset);
	GXPosition3f32(mEndPoint.x, mEndPoint.y - mDrawHeight, mEndPoint.z);

	GXEnd();

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	GXPosition3f32(mStartPoint.x, mStartPoint.y - mDrawHeight, mStartPoint.z);
	GXPosition3f32(mStartPoint.x + xOffset, mStartPoint.y,
	               mStartPoint.z + zOffset);

	const JGeometry::TVec3<f32>* point;
	for (int i = 0; i < mNumActiveMapWirePoints; i++) {
		point = &mMapWirePoints[i].mPosition;
		GXPosition3f32(point->x, point->y - mDrawHeight, point->z);
		GXPosition3f32(point->x + xOffset, point->y, point->z + zOffset);
	}

	GXPosition3f32(mEndPoint.x, mEndPoint.y - mDrawHeight, mEndPoint.z);
	GXPosition3f32(mEndPoint.x + xOffset, mEndPoint.y, mEndPoint.z + zOffset);

	GXEnd();
}

void TMapWire::drawUpper() const
{
	f32 xOffset = mDrawAxes.x;
	f32 zOffset = mDrawAxes.y;
	xOffset *= mDrawWidth;
	zOffset *= mDrawWidth;

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumActiveMapWirePoints + 2) * 2);

	GXPosition3f32(mStartPoint.x + xOffset, mStartPoint.y,
	               mStartPoint.z + zOffset);
	GXPosition3f32(mStartPoint.x - xOffset, mStartPoint.y,
	               mStartPoint.z - zOffset);

	const JGeometry::TVec3<f32>* point;
	for (int index = 0; index < mNumActiveMapWirePoints; index++) {
		point = &mMapWirePoints[index].mPosition;
		GXPosition3f32(point->x + xOffset, point->y, point->z + zOffset);
		GXPosition3f32(point->x - xOffset, point->y, point->z - zOffset);
	}

	GXPosition3f32(mEndPoint.x + xOffset, mEndPoint.y, mEndPoint.z + zOffset);
	GXPosition3f32(mEndPoint.x - xOffset, mEndPoint.y, mEndPoint.z - zOffset);

	GXEnd();
}

f32 TMapWire::getPointPowerAtReleased(f32 pos) const
{
	// 1 = default height, 0 = stretched all the way down
	f32 relativeHeightAtPos;
	if (pos >= mHangPos)
		relativeHeightAtPos = (pos - mHangPos) / (1.0f - mHangPos);
	else
		relativeHeightAtPos = 1.0f - pos / mHangPos;

	f32 power = 1.0f - relativeHeightAtPos * relativeHeightAtPos;

	return power;
}

void TMapWire::getPointPosAtReleased(f32 pos, JGeometry::TVec3<f32>* out) const
{
	JGeometry::TVec3<f32> linePoint;
	getPointPosOnLine(pos, &linePoint);

	JGeometry::TVec3<f32> defaultPoint;
	getPointPosDefault(pos, &defaultPoint);

	f32 power = getPointPowerAtReleased(pos);
	out->set(linePoint.x,
	         linePoint.y
	             + (1.0f - mBounceRemainingPower)
	                   * (defaultPoint.y - linePoint.y)
	             + power * mHangOrBouncePoint.y,
	         linePoint.z);
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

	f32 cosine           = JMASCos(mMoveTimer * 32768.0f);
	mHangOrBouncePoint.y = cosine * mBounceAmplitude * mBounceRemainingPower;

	return false;
}

void TMapWire::initPointAtJustReleased(f32 pos, TMapWirePoint* point)
{
	point->mPosOnWire = pos;
	getPointPosAtReleased(pos, &point->mPosition);
	point->mPosReturnRate = (point->mDefaultPosOnWire - pos) / 1000.0f;
}

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
		JGeometry::TVec3<f32> marioVel;
		marioVel.x       = *gpMarioSpeedX;
		marioVel.y       = *gpMarioSpeedY;
		marioVel.z       = *gpMarioSpeedZ;
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

void TMapWire::move()
{
	bool bounceFinished;

	switch (mState) {
	case IDLE:
		break;

	case HANGING:
		break;

	case RELEASED:
		bounceFinished = updateMovePointAtReleased();

		if (bounceFinished) {
			TMapWirePoint* mapWirePoint;

			for (int i = 0; i < mNumActiveMapWirePoints; i++) {
				mapWirePoint = &mMapWirePoints[i];
				mapWirePoint->reset();
			}

			mState = TMapWire::IDLE;
		} else {
			for (int i = 0; i < mNumActiveMapWirePoints; i++)
				updatePointAtReleased(i);
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

	f32 totalLength   = JGeometry::TVec3<f32>(flatEnd - flatStart).length();
	f32 partialLength = JGeometry::TVec3<f32>(perpPoint - flatStart).length();
	return partialLength / totalLength;
}

void TMapWire::getPointPosOnLine(f32 pos, JGeometry::TVec3<f32>* out) const
{
	out->set(mStartPoint.x + mWireSpan.x * pos,
	         mStartPoint.y + mWireSpan.y * pos,
	         mStartPoint.z + mWireSpan.z * pos);
}

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
		JGeometry::TVec3<f32> linePoint;
		linePoint.x = mStartPoint.x + mWireSpan.x * pos;
		linePoint.y = mStartPoint.y + mWireSpan.y * pos;
		linePoint.z = mStartPoint.z + mWireSpan.z * pos;

		JGeometry::TVec3<f32> defaultPoint;
		getPointPosDefault(pos, &defaultPoint);

		f32 lineX = linePoint.x;
		f32 lineZ = linePoint.z;
		f32 power = getPointPowerAtReleased(pos);
		out->x    = lineX;
		out->y
		    = linePoint.y
		      + (1.0f - mBounceRemainingPower) * (defaultPoint.y - linePoint.y)
		      + power * mHangOrBouncePoint.y;
		out->z = lineZ;
	}
}

void TMapWire::getPointPosDefault(f32 pos, JGeometry::TVec3<f32>* out) const
{
	out->set(mStartPoint.x + mWireSpan.x * pos,
	         mStartPoint.y + mWireSpan.y * pos
	             - mWireSag * JMASSin(pos * 32768.0f),
	         mStartPoint.z + mWireSpan.z * pos);
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

	TMapWirePoint* point2;
	for (int i = 0; i < mNumMapWirePoints; i++) {
		{
			TMapWirePoint* point = &mMapWirePoints[i];
			f32 pos              = (f32)(i + 1) / (f32)(mNumMapWirePoints);
			point->mPosOnWire = point->mDefaultPosOnWire = pos;
		}

		point2 = &mMapWirePoints[i];
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
