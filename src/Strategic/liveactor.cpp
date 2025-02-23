#include <Strategic/LiveActor.hpp>

void TLiveActor::stopAnmSound() { }

void TLiveActor::getBasNameTable() const { }

void TLiveActor::setCurAnmSound() { }

void TLiveActor::setAnmSound(const char*) { }

void TLiveActor::updateAnmSound() { }

void TLiveActor::initAnmSound() { }

MtxPtr TLiveActor::getTakingMtx() { }

void TLiveActor::getFocalPoint() const { }

void TLiveActor::getJointTransByIndex(int, JGeometry::TVec3<f32>*) const { }

void TLiveActor::hasMapCollision() const { }

void TLiveActor::getGravityY() const { }

void TLiveActor::calcVelocityToJumpToY(const JGeometry::TVec3<f32>&, f32,
                                       f32) const
{
}

void TLiveActor::performOnlyDraw(u32, JDrama::TGraphics*) { }

void TLiveActor::perform(u32, JDrama::TGraphics*) { }

void TLiveActor::drawObject(JDrama::TGraphics*) { }

void TLiveActor::requestShadow() { }

void TLiveActor::moveObject() { }

void TLiveActor::setGroundCollision() { }

void TLiveActor::getShadowType() { }

void TLiveActor::receiveMessage(THitActor*, u32) { }

void TLiveActor::kill() { }

void TLiveActor::calcRootMatrix() { }

void TLiveActor::control() { }

void TLiveActor::bind() { }

void TLiveActor::load(JSUMemoryInputStream&) { }

void TLiveActor::init(TLiveManager*) { }

void TLiveActor::initLodAnm(const TLodAnmIndex*, int, f32) { }

void TLiveActor::getRootJointMtx() const { }

void TLiveActor::getModel() const { }

void TLiveActor::calcRideMomentum() { }

void TLiveActor::belongToGround() const { }

void TLiveActor::calcRidePos() { }

TLiveActor::~TLiveActor() { }

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
	unkE0 = 0.0;
	unkDC = 0.0;
	unkD8 = 0.0;
}

void TLiveActor::getMActor() const { }
