#include <MoveBG/MapObjMamma.hpp>

int TSandBase::mWitherTime = 800;
f32 TSandBase::mScaleMin   = 0.00001f;

f32 TSandBombBase::mFiringFrameSpeed     = 3.0f;
f32 TSandBombBase::mFiringFrameDownSpeed = 0.2f;
f32 TSandBombBase::mExplodeFrameSpeed    = 1.0f;
f32 TSandBombBase::mMarioJumpRate        = 0.12f;
int TSandBombBase::mExlodingRumbleTime   = 20;

f32 TSandCastle::mCollisionRate = 1.7f;

int TLeanMirror::mGoTargetTime  = 600;
int TLeanMirror::mDemoWaitTime  = -1;
int TLeanMirror::mDemoLightTime = 360;

f32 TMammaBlockRotate::mRotSpeed       = 0.1f;
f32 TMammaBlockRotate::mRotReturnSpeed = 0.01f;
f32 TMammaBlockRotate::mRotEnd         = 130.0f;
f32 TMammaBlockRotate::mMapGoSpeed     = 1.0f;
f32 TMammaBlockRotate::mMapBackSpeed   = 0.1f;
int TMammaBlockRotate::mWaitTime       = 600;
