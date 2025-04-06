#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

void TLiveActor::getMActor() const { }

TLiveActor::TLiveActor(const char* name)
    : TTakeActor(name)
{
	unk70 = nullptr;
	unk74 = nullptr;
	unk78 = nullptr;
	unk7C = 0;
	unk80 = nullptr;
	unk84 = nullptr;
	unk88 = nullptr;
	unk8C = nullptr;
	unk90 = nullptr;
	unk94 = 0.0f;
	unk98 = 0.0f;
	unk9C = 0.0f;
	unkA0 = 0.0f;
	unkA4 = 0.0f;
	unkA8 = 0.0f;
	unkAC = 0.0f;
	unkB0 = 0.0f;
	unkB4 = 0.0f;
	unkB8 = 10.0f;
	unkBC = 25.0f;
	unkC0 = 50.0f;
	unkC4 = nullptr;
	unkC8 = 0.0;
	unkCC = 0.15;
	unkD0 = nullptr;
	unkD4 = nullptr;
	unkE4 = 0.0;
	unkE8 = 1;
	unkEC = nullptr;
	unkF0 = 0x100;

	unkD8.x = unkD8.y = unkD8.z = 0.0;
}

TLiveActor::~TLiveActor() { }

void TLiveActor::calcRidePos()
{
	if (!unkD4)
		return;

	Mtx mtx;
	if (!unkD4->getRootJointMtx())
		SMS_GetActorMtx(*unkD4, mtx);
	else
		MTXCopy(*unkD4->getRootJointMtx(), mtx);
	MTXInverse(mtx, mtx);
	MTXMultVec(mtx, &mPosition, &unkD8);
}

bool TLiveActor::belongToGround() const
{
	if (unkC4 && ((unkC4->unk4 & 0x10 ? true : false) == true ? false : true)
	    && unkC4->unk44 != nullptr && !(unkF0 & 0x80 ? TRUE : FALSE))
		return true;

	return false;
}

void TLiveActor::calcRideMomentum() { }

J3DModel* TLiveActor::getModel() const { return unk74->unk4; }

Mtx* TLiveActor::getRootJointMtx() const { return nullptr; }

void TLiveActor::initLodAnm(const TLodAnmIndex* param_1, int param_2,
                            f32 param_3)
{
	if (!unkD0)
		unkD0 = new TLodAnm(this, param_1, param_2, param_3);
}

void TLiveActor::init(TLiveManager* manager) { }

void TLiveActor::load(JSUMemoryInputStream& stream)
{
	char str[256];

	JDrama::TActor::load(stream);
	stream.readString(str, 256);
	JDrama::TNameRef* ref
	    = JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(str);
	unkC4 = TMap::getIllegalCheckData();

	TLiveManager* casted = (TLiveManager*)ref;
	init(casted);
}

void TLiveActor::bind() { }

void TLiveActor::control() { }
void TLiveActor::calcRootMatrix() { }
void TLiveActor::kill() { }
void TLiveActor::receiveMessage(THitActor*, u32) { }
void TLiveActor::getShadowType() { }
void TLiveActor::setGroundCollision() { }
void TLiveActor::moveObject() { }
void TLiveActor::requestShadow() { }
void TLiveActor::drawObject(JDrama::TGraphics*) { }
void TLiveActor::perform(u32, JDrama::TGraphics*) { }
void TLiveActor::performOnlyDraw(u32, JDrama::TGraphics*) { }
void TLiveActor::calcVelocityToJumpToY(const JGeometry::TVec3<f32>&, f32,
                                       f32) const
{
}
void TLiveActor::getGravityY() const { }
void TLiveActor::hasMapCollision() const { }
void TLiveActor::getJointTransByIndex(int, JGeometry::TVec3<f32>*) const { }
void TLiveActor::getFocalPoint() const { }
MtxPtr TLiveActor::getTakingMtx() { }
void TLiveActor::initAnmSound() { }
void TLiveActor::updateAnmSound() { }
void TLiveActor::setAnmSound(const char*) { }
void TLiveActor::setCurAnmSound() { }
void TLiveActor::getBasNameTable() const { }
void TLiveActor::stopAnmSound() { }
