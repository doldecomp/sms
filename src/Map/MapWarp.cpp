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

// The 24 dead bytes are two rungs of the same chain: `getChild()` at the
// awaken site is +16 (at both sites it is +32) and the SMSGetMap() fork
// (promoted to Map.hpp in header round 18) is +4 per read site, i.e. +8 here.
// Both sites raw is 0x40, both through getChild() 0x60.
void TMapWarp::changeModel(int i)
{
	if (unk8 == i)
		return;

	// TODO: inlines
	SMSGetMap()->getModelManager()->getJointModel(0)->mChildren[unk8]
	    ->sleep();
	SMSGetMap()->getModelManager()->getJointModel(0)->getChild(i)->awake();
	unk8 = i;
}

void TMapWarp::warp(int) { }

// TODO: 99.8% (66.4% before closure batch 83). Three fixes got it there: the
// warp-point index is a named `int no = checkData->getData();` so that MWCC
// caches `no * 0x14` in a callee-saved register while re-reading the `unk4`
// member after each virtual call; `unk8` is the *left* operand of both
// inequality tests; and the warp destination is `SMS_GetMarioPos() + unk4[no]
// .unk8` bound to a named local (the by-value left operand of operator+ is
// retail's three-word copy in front of `bl TVec3::add`, and the named result is
// the second copy at 0xe8) rather than an in-place `+=`. The stream vector is
// zero-initialised and then has `.z` overwritten (`stfs` to the same slot
// twice), not built by the three-argument constructor.
// Residue: frame 0x170 vs 0x158, 24 dead bytes, plus `addi r5, r4, 0` where we
// emit `mr r5, r4` for MTXMultVec's duplicated out-pointer. The slots are not
// uniformly shifted: retail's order up the frame is [operator+ copy 0xa0,
// warpPos 0xe8, stream vector 0x100, mtx 0x10c, checkData 0x13c, the two
// double magics 0x148/0x158], ours is [copy 0xd4, stream vector 0xe0, mtx
// 0xec, warpPos 0x11c, checkData 0x12c, ...] -- so retail has 52 fewer low
// bytes and keeps both warp vectors at the bottom, below the stream vector and
// the matrix, where ours has `warpPos` in the named block. Making warpPos
// unnamed is much worse (80.2%) and an explicit `TVec3(...)` temporary bound
// to the `const&` parameter worse still (76.5%), so the two vectors are not
// plain temporaries either; the low region is the lead.
void TMapWarp::watchToWarp()
{
	const TBGCheckData* checkData;
	f32 fVar8 = gpMap->checkGroundExactY(gpMarioPos->x, gpMarioPos->y + 30.0f,
	                                     gpMarioPos->z, &checkData);

	if (checkData->isWarp()) {
		int no   = checkData->getData();
		int warp = unk4[no].unk0;
		if (unk8 != warp) {
			gpMap->getModelManager()->getJointModel(0)->getChild(unk8)->sleep();
			gpMap->getModelManager()->getJointModel(0)->getChild(warp)->awake();
			unk8 = unk4[no].unk0;

			JGeometry::TVec3<f32> warpPos
			    = SMS_GetMarioPos() + unk4[no].unk8;
			SMS_MarioWarpRequest(warpPos,
			                     (*gpMarioAngleY * 180.0f) / 32768.0f);
		}
	}

	if (checkData->isMapChange()) {
		int no = checkData->getData();
		if (unk8 != no) {
			gpMap->getModelManager()->getJointModel(0)->getChild(unk8)->sleep();
			gpMap->getModelManager()->getJointModel(0)->getChild(no)->awake();

			unk8 = no;
		}
	}

	int no = gpCubeStream->getInCubeNo(SMS_GetMarioPos());
	if (no == -1)
		return;

	TCubeStreamInfo* info = (TCubeStreamInfo*)(*gpCubeStream->unk14)[no];
	Mtx mtx;
	MsMtxSetXYZRPH(mtx, 0.0f, 0.0f, 0.0f, info->unk18.x, info->unk18.y,
	               info->unk18.z);

	JGeometry::TVec3<f32> vec2(0.0f, 0.0f, 0.0f);
	vec2.z = 0.01f * info->unk40;
	MTXMultVec(mtx, &vec2, &vec2);
	if ((info->unk38 == 0 ? true : false) || (info->unk38 == 1 ? true : false))
		SMS_FlowMoveMario(vec2);
	else
		SMS_WindMoveMario(vec2);
}

// The 16 dead bytes are one `TJointObj::getChild()` level on the loop body's
// child fetch (`getChildrenNum()` on the bound instead is the same +16; both
// together overshoot to 0x60).
void TMapWarp::initModel()
{
	// TODO: inlines
	int num = gpMap->getModelManager()->getJointModel(0)->mChildrenNum;
	for (int i = 0; i < num; ++i)
		if (i != unk8)
			gpMap->getModelManager()
			    ->getJointModel(0)
			    ->getChild((u16)i)
			    ->sleep();
}

int getWarpPointNo(const char* name)
{
	// Fabricated struct; the entries themselves are the map's 0x98-byte
	// point_name_table$2630.
	struct NameTableEntry {
		const char* mName;
		u32 mNo;
	};
	static const NameTableEntry point_name_table[] = {
		{ "warpA1", 0 },  { "warpA0", 1 },  { "warpB1", 2 },  { "warpB0", 3 },
		{ "warpC1", 4 },  { "warpC0", 5 },  { "warpD1", 6 },  { "warpD0", 7 },
		{ "warpE1", 8 },  { "warpE0", 9 },  { "warpF1", 10 }, { "warpF0", 11 },
		{ "warpG1", 12 }, { "warpG0", 13 }, { "warpH1", 14 }, { "warpH0", 15 },
		{ "warpI1", 16 }, { "warpI0", 17 }, { nullptr, 0 },
	};

	u32 needle = 0;
	while (strcmp(point_name_table[needle].mName, name) != 0)
		++needle;
	return point_name_table[needle].mNo;
}

void loadWarpPointPos(JSUMemoryInputStream& stream, int no, Vec* positions)
{
	Vec& pos = positions[no];
	stream >> pos.x >> pos.y >> pos.z;

	u32 dummy;
	stream >> dummy >> dummy >> dummy;
	stream >> dummy >> dummy >> dummy;
}

// TODO: 98.3%, frame and every stack slot exact. Closure
// batch 136 found three constructs. (1) The three stack arrays are declared in
// the order positions/warp/dest, not the reverse: named locals descend from
// the top of the local area with the first declared highest, so this is what
// puts them at 0x68/0xb8/0x108 as retail does. (2) The four pre-loop reads
// share one slot at 0x1f8 *above* the arrays, which only happens if they go
// through one named `u32 data` and `operator>>`; `stream.readU32()` gives each
// expansion its own low-region temp (four slots, 32 bytes too few overall).
// (3) `loadWarpPointPos` chains its reads: `stream >> x >> y >> z;` and two
// `stream >> dummy >> dummy >> dummy;`. Each chained `>>` continuation is 8
// bytes of low region and one hoisted `addi rN, rStream, 0` outside the name
// loop, so retail's five copies and frame 0x238 both pin the count at six
// continuations. "4,2" and "5,1" groupings are indistinguishable from "3,3",
// so only the count is evidence.
// Re-pass II (batch 178) paid the 4 bytes batch 136 could not and every r1
// displacement now matches (dummy 0x64, arrays 0x68/0xb8/0x108, `data` 0x1f8,
// `stmw r19, 0x204`, frame 0x238). Retail's low region is 0x5c = 92 bytes:
// 48 for the six `>>` continuations, 16 that loadWarpPointPos's inlined
// parameters cost, 4 for `Vec& pos = positions[no];`, 4 for the SMSGetMarDirector()
// global fork (+4 per read, the Map.hpp SMSGetMap() rung again) and 16 for two
// `getUnk8()` accessor sites (+8 each, measured by deleting them: three sites
// is frame 0x238+4, none is 0x220). Three measurements pin that split:
//   * the `pos` binding is +4 and is *needed* -- without it MWCC keeps the
//     element base in one callee-saved register and folds `+4`/`+8` into the
//     `addi r4` at each call, losing retail's hoisted `addi r20, r5, 8` /
//     `addi r23, r5, 4`. `Vec* pos = &positions[no];` is identical; a TU-local
//     direct-return fork used at the three read sites is also +4 *per site*;
//     `positions += no` costs a register (205 instructions, 94.3%).
//   * `const char* str = stream.readString();` must NOT be named: the extra
//     named local rotates seven callee-saved registers (97.2 -> 98.1 for the
//     naming alone, frame unchanged).
//   * six continuations is confirmed, not just inferred: the "3,3,1,2"
//     grouping (five continuations) drops both the pool 8 bytes *and* one
//     callee-saved register (`stmw r20`), 96.8%.
// Which two of the three negation components read through `getUnk8()` is not
// determined by the binary -- the accessor is instruction-neutral there, only
// its site count is priced. The alternative accounting (three accessors, no
// `pos` binding, no fork) also gives 92 bytes exactly but loses the two
// hoisted `addi`s (96.4%), so the binary prefers this one.
// Residue: a zero-frame callee-saved permutation with every instruction and
// every displacement identical (retail `this` r31 / stream r25 / array base
// and cnt sharing r24, ours r24 / r27 / r25+r26). Inert on it: `cnt` in the
// for-init scope, the `Vec*` spelling. Worse: `cnt` declared before the arrays
// (205 instructions). loadWarpPointPos's own UNUSED copy is still 0xc4 against
// the map's 0x12c, so its body is still 26 instructions short of retail's --
// the standalone copy is the remaining lead.
void TMapWarp::init(JSUMemoryInputStream& stream)
{
	u32 data;
	stream >> data;
	unk0 = data;
	if (!unk0)
		return;

	stream >> data;
	unk8 = data;
	unk4 = new TMapWarpInfo[unk0 * 2];

	JGeometry::TVec3<f32> local_130[20];
	u32 local_180[20];
	u32 local_1d0[20];

	for (int i = 0; i < unk0; ++i) {
		stream >> data;
		local_1d0[i] = data;
		stream >> data;
		local_180[i] = data;
	}

	int cnt = unk0 * 2;
	for (int i = 0; i < cnt; ++i) {
		loadWarpPointPos(
		    stream, getWarpPointNo(stream.readString()), local_130);
	}

	for (int i = 0; i < unk0; ++i) {
		unk4[2 * i].unk8.x = local_130[2 * i].x - local_130[2 * i + 1].x;
		unk4[2 * i].unk8.y = local_130[2 * i].y - local_130[2 * i + 1].y;
		unk4[2 * i].unk8.z = local_130[2 * i].z - local_130[2 * i + 1].z;

		unk4[2 * i].unk0 = local_180[i];
		unk4[2 * i].unk4 = local_1d0[i];

		unk4[2 * i + 1].unk8.x = -unk4[2 * i].getUnk8().x;
		unk4[2 * i + 1].unk8.y = -unk4[2 * i].getUnk8().y;
		unk4[2 * i + 1].unk8.z = -unk4[2 * i].unk8.z;

		unk4[2 * i + 1].unk0 = local_180[i];
		unk4[2 * i + 1].unk4 = local_1d0[i];
	}

	if (SMSGetMarDirector()->mMap == 4) {
		unkC = 8.0f;
	}
}

TMapWarp::TMapWarp()
    : unk0(0)
    , unk4(0)
    , unk8(0)
    , unkC(3.0f)
{
}
