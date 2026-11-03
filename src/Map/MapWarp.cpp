#include <Map/MapWarp.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/mtx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/M3UModelMario.hpp>

void TMapWarp::changeModel(int i)
{
	if (mCurrentModelIndex == i)
		return;

	// TODO: inlines
	gpMap->getModelManager()
	    ->getJointModel(0)
	    ->mChildren[mCurrentModelIndex]
	    ->sleep();
	gpMap->getModelManager()->getJointModel(0)->mChildren[i]->awake();
	mCurrentModelIndex = i;
}

void TMapWarp::warp(int index)
{
	int dstModelIdx = mWarpInfos[index].mDestinationModelIndex;
	if (mCurrentModelIndex != dstModelIdx) {
		gpMap->getModelManager()
		    ->getJointModel(0)
		    ->getChild(mCurrentModelIndex)
		    ->sleep();
		gpMap->getModelManager()
		    ->getJointModel(0)
		    ->getChild(dstModelIdx)
		    ->awake();
		mCurrentModelIndex = mWarpInfos[index].mDestinationModelIndex;

		JGeometry::TVec3<f32> pos
		    = SMS_GetMarioPos() + mWarpInfos[index].mWarpOffset;
		SMS_MarioWarpRequest(pos, (SMS_GetMarioAngleY() * 180.0f) / 32768.0f);
	}
}

void TMapWarp::watchToWarp()
{
	const TBGCheckData* checkData;
	f32 fVar8 = gpMap->checkGroundExactY(SMS_GetMarioPos().x,
	                                     SMS_GetMarioPos().y + 30.0f,
	                                     SMS_GetMarioPos().z, &checkData);

	if (checkData->isWarp())
		warp(checkData->getData());

	if (checkData->isMapChange())
		changeModel(checkData->getData());

	int no = gpCubeStream->getInCubeNo(SMS_GetMarioPos());
	if (no == -1)
		return;

	TCubeStreamInfo& info = (TCubeStreamInfo&)(*gpCubeStream->unk14)[no];
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, info.unk18.x, info.unk18.y,
	               info.unk18.z);

	JGeometry::TVec3<f32> vec2(0.0f, 0.0f, 0.0f);
	vec2.z = 0.01f * info.unk40;
	MTXMultVec(mtx, &vec2, &vec2);
	if ((info.unk38 == 0 ? true : false) || (info.unk38 == 1 ? true : false))
		SMS_FlowMoveMario(vec2);
	else
		SMS_WindMoveMario(vec2);
}

void TMapWarp::initModel()
{
	int num = gpMap->getModelManager()->getJointModel(0)->mChildrenNum;
	for (int i = 0; i < num; ++i)
		if (i != mCurrentModelIndex)
			gpMap->getModelManager()
			    ->getJointModel(0)
			    ->getChild((u16)i)
			    ->sleep();
}

int getWarpPointNo(const char* name)
{
	// Fabricated
	struct NameTableEntry {
		const char* unk0;
		u32 unk4;
	};
	static const NameTableEntry point_name_table[] = {
		{ "warpA1", 0 },  { "warpA0", 1 },  { "warpB1", 2 },  { "warpB0", 3 },
		{ "warpC1", 4 },  { "warpC0", 5 },  { "warpD1", 6 },  { "warpD0", 7 },
		{ "warpE1", 8 },  { "warpE0", 9 },  { "warpF1", 10 }, { "warpF0", 11 },
		{ "warpG1", 12 }, { "warpG0", 13 }, { "warpH1", 14 }, { "warpH0", 15 },
		{ "warpI1", 16 }, { "warpI0", 17 }, { nullptr, 0 },
	};

	int i = 0;
	while (strcmp(point_name_table[i].unk0, name) != 0)
		++i;
	return point_name_table[i].unk4;
}

void loadWarpPointPos(JSUMemoryInputStream& stream, int count, Vec* points)
{
	for (int i = 0; i < count; ++i) {
		const char* name = stream.readString();
		Vec& point       = points[getWarpPointNo(name)];
		stream >> point.x >> point.y >> point.z;

		u32 dummy;
		stream >> dummy >> dummy >> dummy;
		stream >> dummy >> dummy >> dummy;
	}
}

void TMapWarp::init(JSUMemoryInputStream& stream)
{
	mWarpPairCount = stream.readU32();
	if (!mWarpPairCount)
		return;

	mCurrentModelIndex = stream.readU32();
	mWarpInfos         = new TMapWarpInfo[mWarpPairCount * 2];

	u32 local_1d0[20];
	u32 local_180[20];
	JGeometry::TVec3<f32> local_130[20];

	for (int i = 0; i < mWarpPairCount; ++i) {
		local_1d0[i] = stream.readU32();
		local_180[i] = stream.readU32();
	}

	loadWarpPointPos(stream, mWarpPairCount * 2, local_130);

	for (int i = 0; i < mWarpPairCount; ++i) {
		mWarpInfos[2 * i].mWarpOffset.x
		    = local_130[2 * i].x - local_130[2 * i + 1].x;
		mWarpInfos[2 * i].mWarpOffset.y
		    = local_130[2 * i].y - local_130[2 * i + 1].y;
		mWarpInfos[2 * i].mWarpOffset.z
		    = local_130[2 * i].z - local_130[2 * i + 1].z;

		mWarpInfos[2 * i].mDestinationModelIndex = local_180[i];
		mWarpInfos[2 * i].mSourceModelIndex      = local_1d0[i];

		mWarpInfos[2 * i + 1].mWarpOffset.x = -mWarpInfos[2 * i].mWarpOffset.x;
		mWarpInfos[2 * i + 1].mWarpOffset.y = -mWarpInfos[2 * i].mWarpOffset.y;
		mWarpInfos[2 * i + 1].mWarpOffset.z = -mWarpInfos[2 * i].mWarpOffset.z;

		mWarpInfos[2 * i + 1].mDestinationModelIndex = local_1d0[i];
		mWarpInfos[2 * i + 1].mSourceModelIndex      = local_180[i];
	}

	if (SMSGetMarDirector()->getCurrentStage() == 4) {
		unkC = 8.0f;
	}
}

TMapWarp::TMapWarp()
    : mWarpPairCount(0)
    , mWarpInfos(0)
    , mCurrentModelIndex(0)
    , unkC(3.0f)
{
}
