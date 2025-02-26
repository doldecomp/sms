#include <M3DUtil/MActor.hpp>

void MActor::updateMatAnm() { }

void MActor::entryOut() { }

void MActor::entryIn() { }

void MActor::updateOut() { }

void MActor::updateIn() { }

void MActor::setBrkFromIndex(int) { }

void MActor::setBrk(const char*) { }

void MActor::setBlk(const char*) { }

void MActor::setBtkFromIndex(int) { }

void MActor::setBtk(const char*) { }

void MActor::setBtpFromIndex(int) { }

void MActor::setBtp(const char*) { }

void MActor::setBpkFromIndex(int) { }

void MActor::setBpk(const char*) { }

void MActor::checkCurBckFromIndex(int) { }

void MActor::setBckFromIndex(int) { }

void MActor::setBck(const char*) { }

void MActor::setFrameRate(float, int) { }

void MActor::getCurAnmIdx(int) const { }

void MActor::checkBckPass(float) { }

void MActor::getFrameCtrl(int) { }

void MActor::checkAnmFileExist(const char*, int) { }

void MActor::checkCurAnmFromIndex(int, int) { }

void MActor::checkCurAnm(const char*, int) { }

void MActor::perform(u32, JDrama::TGraphics*) { }

void MActor::matAnmFrameUpdate() { }

void MActor::frameUpdate() { }

void MActor::entry() { }

void MActor::setLightType(int) { }

void MActor::setLightData(const TBGCheckData*, const JGeometry::TVec3<f32>&) { }

void MActor::setLightID(short) { }

void MActor::viewCalc() { }

void MActor::calc() { }

void MActor::calcAnm() { }

void MActor::updateOutSubBck() { }

void MActor::updateInSubBck() { }

void MActor::setJointCallback(int, int (*)(J3DNode*, int)) { }

void MActor::offMakeDL() { }

void MActor::unlockDLIfNeed() { }

void MActor::resetDL() { }

void MActor::initDL() { }

void MActor::setAnimation(const char*, int) { }

void MActor::curAnmEndsNext(int, char*) { }

void MActor::isCurAnmAlreadyEnd(int) { }

void MActor::setModel(J3DModel*, u32) { }

MActor::MActor(MActorAnmData*) { }
