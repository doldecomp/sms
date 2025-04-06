#include <Map/MapWarp.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/mtx.h>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

TMapWarp::TMapWarp()
    : unk0(0)
    , unk4(0)
    , unk8(0)
    , unkC(3.0f)
{
}

void TMapWarp::init(JSUMemoryInputStream& stream)
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

	unk0 = stream.readU32();
	if (!unk0)
		return;

	unk8 = stream.readU32();
	unk4 = new TMapWarpInfo[unk0 * 2];

	u32 local_1d0[20];
	u32 local_180[20];
	JGeometry::TVec3<f32> local_130[20];

	for (int i = 0; i < unk0; ++i) {
		local_1d0[i] = stream.readU32();
		local_180[i] = stream.readU32();
	}

	int cnt = unk0 * 2;
	for (int i = 0; i < cnt; ++i) {
		const char* str = stream.readString();
		u32 needle      = 0;
		while (strcmp(point_name_table[needle].unk0, str) != 0)
			++needle;

		u32 idx = point_name_table[needle].unk4;
		stream.read(&local_130[idx].x, 4);
		stream.read(&local_130[idx].y, 4);
		stream.read(&local_130[idx].z, 4);

		u32 dummy;
		stream.read(&dummy, 4);
		stream.read(&dummy, 4);
		stream.read(&dummy, 4);
		stream.read(&dummy, 4);
		stream.read(&dummy, 4);
		stream.read(&dummy, 4);
	}

	for (int i = 0; i < unk0; ++i) {
		unk4[2 * i].unk8.x = local_130[2 * i].x - local_130[2 * i + 1].x;
		unk4[2 * i].unk8.y = local_130[2 * i].y - local_130[2 * i + 1].y;
		unk4[2 * i].unk8.z = local_130[2 * i].z - local_130[2 * i + 1].z;

		unk4[2 * i].unk0 = local_180[i];
		unk4[2 * i].unk4 = local_1d0[i];

		unk4[2 * i + 1].unk8.x = -unk4[2 * i].getUnk8().x;
		unk4[2 * i + 1].unk8.y = -unk4[2 * i].getUnk8().y;
		unk4[2 * i + 1].unk8.z = -unk4[2 * i].getUnk8().z;

		unk4[2 * i + 1].unk0 = local_180[i];
		unk4[2 * i + 1].unk4 = local_1d0[i];
	}

	if (gpMarDirector->map == 4) {
		unkC = 8.0f;
	}
}

void loadWarpPointPos(JSUMemoryInputStream&, int, Vec*) { }
void getWarpPointNo(const char*) { }

void TMapWarp::initModel()
{
	// TODO: inlines
	int num = gpMap->mModelManager->mJointModels[0]->mChildrenNum;
	for (int i = 0; i < num; ++i)
		if (i != unk8)
			gpMap->mModelManager->mJointModels[0]->mChildren[(u16)i]->sleep();
}

void TMapWarp::watchToWarp()
{
	const TBGCheckData* checkData;
	f32 fVar8 = gpMap->checkGroundExactY(gpMarioPos->x, gpMarioPos->y + 30.0f,
	                                     gpMarioPos->z, &checkData);

	if (checkData->unk0 == 0x200 || checkData->unk0 == 0x201 ? true : false) {
		int warp = unk4[checkData->unk2].unk0;
		if (warp != unk8) {
			// TODO: inlines
			gpMap->mModelManager->mJointModels[0]->mChildren[unk8]->sleep();
			gpMap->mModelManager->mJointModels[0]->mChildren[warp]->awake();
			unk8 = warp;

			// TODO: tons of inlines presumably?
			JGeometry::TVec3<f32> marioPos(*gpMarioPos);
			marioPos.add(unk4[checkData->unk2].unk8);
			SMS_MarioWarpRequest(marioPos,
			                     (*gpMarioAngleY * 180.0f) / 32768.0f);
		}
	}

	if (checkData->unk0 == 0x202 || checkData->unk0 == 0x203 ? true : false) {
		if (checkData->unk2 != unk8) {
			// TODO: inlines
			gpMap->mModelManager->mJointModels[0]->mChildren[unk8]->sleep();
			gpMap->mModelManager->mJointModels[0]
			    ->mChildren[checkData->unk2]
			    ->awake();
			unk8 = checkData->unk2;
		}
	}

	int no = gpCubeStream->getInCubeNo(*gpMarioPos);
	if (no != -1) {
		TCubeStreamInfo& info = (TCubeStreamInfo&)(*gpCubeStream->unk14)[no];
		JGeometry::TVec3<s16> vec;
		vec.x = 182.0444f * info.unk18.x;
		vec.y = 182.0444f * info.unk18.y;
		vec.z = 182.0444f * info.unk18.z;
		Mtx mtx;
		MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, vec.x, vec.y, vec.z);
		JGeometry::TVec3<f32> vec2(0.0f, 0.0f, info.unk40 * 0.01f);
		MTXMultVec(mtx, &vec2, &vec2);
		if ((info.unk38 == 0 ? true : false)
		    || (info.unk38 == 1 ? true : false))
			SMS_FlowMoveMario(vec2);
		else
			SMS_WindMoveMario(vec2);
	}
}

void TMapWarp::warp(int) { }

void TMapWarp::changeModel(int i)
{
	if (unk8 == i)
		return;

	// TODO: inlines
	gpMap->mModelManager->mJointModels[0]->mChildren[unk8]->sleep();
	gpMap->mModelManager->mJointModels[0]->mChildren[i]->awake();
	unk8 = i;
}
