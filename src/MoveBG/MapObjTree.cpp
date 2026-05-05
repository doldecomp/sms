#include <MoveBG/MapObjTree.hpp>

#include <stdio.h>

#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

TMapObjLeaf::TMapObjLeaf()
    : unk0(0.0f)
    , unk4(0.0f)
    , unk8(nullptr)
{
	unkC.set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	         1.0f);
	unk8 = new TMapCollisionMove();
}

void TMapObjTree::touchPlayer(THitActor* player)
{
	unk158   = 0;
	s16 data = gpMarioGroundPlane[0]->getData();
	if (!(gpMarioGroundPlane[0]->getActor() == this && data >= 0
	      && data < unk150)) {
		return;
	}
	if (marioHipAttack()) {
		unk154[data].unk4 = unk154[data].unk4 + unk160;
	} else {
		if (marioIsOn()) {
			unk154[data].unk4 = unk154[data].unk4 + unk15C;
		}
	}
}

void TMapObjTree::initEach()
{
	switch (mActorType) {
	case 0x40000034:
	case 0x40000038:
		unk148 = 20.0f;
		unk14C = 95.0f;
		unk150 = 12;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000035:
		unk148 = 20.0f;
		unk14C = 100.0f;
		unk150 = 8;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000036:
		unk148 = 50.0f;
		unk14C = 100.0f;
		unk150 = 12;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000037:
		unk148 = 95.0f;
		unk14C = 60.0f;
		unk150 = 8;
		unk15C = 0.001f;
		unk160 = 0.006f;
		unk164 = 0.01f;
		unk168 = 0.97f;
		return;
	case 0x40000039:
		unk148 = 70.0f;
		unk14C = 100.0f;
		unk150 = 8;
		unk15C = 0.004f;
		unk160 = 0.008f;
		unk164 = 0.03f;
		unk168 = 0.9f;
		return;
	}
	return;
}

void TMapObjTree::initMapObj()
{
	TMapObjGeneral::initMapObj();
	initEach();
	unk154 = new TMapObjLeaf[unk150];
	for (s32 i = 0; i < unk150; ++i) {
		TMapObjLeaf& leaf = unk154[i];
		leaf.unk8         = new TMapCollisionMove();
		char buffer[72];
		if (isActorType(0x40000038)) {
			snprintf(buffer, 0x100, "/mapObj/palmLeaf%02d", i + 1);
		} else {
			snprintf(buffer, 0x100, "/mapObj/%sLeaf%02d", getUnkF4(), i + 1);
		}
		leaf.unk8->init(buffer, 0, this);
		leaf.unk8->setAllData(i);
		leaf.unk8->remove();

		leaf.unkC.set(getModel()->getAnmMtx(unk150 - i));
		TMapCollisionMove* move = leaf.unk8;
		// Possibly inline?
		PSMTXCopy(leaf.unkC, (MtxPtr)move->unk20);
		move->setUp();
	}

	if (getMapCollisionManager() != nullptr) {
		getMapCollisionManager()->unk10 = nullptr;
	}
}
