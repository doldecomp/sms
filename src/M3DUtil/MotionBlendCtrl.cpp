#include <M3DUtil/MotionBlendCtrl.hpp>

TMotionBlendCtrl::TMotionBlendCtrl(bool, int) { }

TMotionBlendCtrl::TMotionBlendCtrl(bool) { }

void TMotionBlendCtrl::execSimpleMotionBlend() { }

float TMotionBlendCtrl::getMotionBlendRatio() const { }

void TMotionBlendCtrl::setMotionBlendRatio(float) { }

void TMotionBlendCtrl::keepCurAnm(J3DAnmTransform*, float) { }

void TMotionBlendCtrl::setNewAnm(J3DAnmTransform*) { }

J3DAnmTransform* TMotionBlendCtrl::getOldMotionBlendAnmPtr() const { }

void TMotionBlendCtrl::setOldMotionBlendAnmPtr(J3DAnmTransform*) { }

float TMotionBlendCtrl::getOldMotionBlendFrame() const { }
