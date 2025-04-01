#include <JSystem/JParticle/JPADrawVisitor.hpp>
#include <JSystem/JSupport/JSUList.hpp>

JPADrawClipBoard* JPADrawContext::pcb;

void JPADrawExecLoadExTex::exec(const JPADrawContext*) { }
void JPADrawExecGenPrjMtx::exec(const JPADrawContext*) { }
void JPADrawExecGenPrjTexMtx::exec(const JPADrawContext*) { }
void JPADrawExecGenTexMtx0::exec(const JPADrawContext*) { }
void JPADrawExecGenIdtMtx::exec(const JPADrawContext*) { }
void JPADrawExecSetTexMtx::exec(const JPADrawContext*) { }
void JPADrawExecLoadDefaultTexture::exec(const JPADrawContext*) { }
void JPADrawExecLoadTexture::exec(const JPADrawContext*) { }
void JPADrawExecSetPointSize::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecSetLineWidth::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRegisterPrmColorAnm::exec(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawExecRegisterPrmAlphaAnm::exec(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawExecRegisterEnvColorAnm::exec(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawExecRegisterPrmCEnv::exec(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawExecRegisterPrmAEnv::exec(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawExecSetTexMtx::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecLoadTexture::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecYBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotYBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void dirTypeVel(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&) { }
void dirTypePos(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&) { }
void dirTypePosInv(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&) {
}
void dirTypeEmtrDir(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&)
{
}
void dirTypePrevPtcl(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&)
{
}
void rotTypeY(f32, f32, Mtx&) { }
void rotTypeX(f32, f32, Mtx&) { }
void rotTypeZ(f32, f32, Mtx&) { }
void rotTypeXYZ(f32, f32, Mtx&) { }
void rotTypeYJiggle(f32, f32, Mtx&) { }
void JPADrawExecDirectional::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotDirectional::exec(const JPADrawContext*, JPABaseParticle*) {
}
void JPADrawExecDirectionalCross::exec(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawExecRotDirectionalCross::exec(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawExecDirBillBoard::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotation::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecRotationCross::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecPoint::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawExecLine::exec(const JPADrawContext*, JPABaseParticle*) { }
void stripeGetNext(JSULink<JPABaseParticle>*) { }
void stripeGetPrev(JSULink<JPABaseParticle>*) { }
void JPADrawExecStripe::exec(const JPADrawContext*) { }
void JPADrawExecStripeCross::exec(const JPADrawContext*) { }
void JPADrawExecRegisterColorEmitterPE::exec(const JPADrawContext*) { }
void JPADrawExecRegisterColorEmitterP::exec(const JPADrawContext*) { }
void JPADrawExecRegisterColorEmitterE::exec(const JPADrawContext*) { }
void JPADrawExecRegisterColorChildPE::exec(const JPADrawContext*) { }

void JPADrawCalcColorPrm::calc(const JPADrawContext*) { }
void JPADrawCalcColorEnv::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext*) { }
void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext*) { }
void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext*) { }
void JPADrawExecCallBack::exec(const JPADrawContext*) { }
void JPADrawExecCallBack::exec(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleX::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleY::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleXBySpeed::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleYBySpeed::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleCopyX2Y::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcScaleAnmTimingNormal::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingRepeatX::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingRepeatY::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingReverseX::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcScaleAnmTimingReverseY::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcColorPrm::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcColorEnv::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcColorCopyFromEmitter::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameNormal::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameRepeat::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameReverse::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameMerge::calc(const JPADrawContext*,
                                         JPABaseParticle*)
{
}
void JPADrawCalcColorAnmFrameRandom::calc(const JPADrawContext*,
                                          JPABaseParticle*)
{
}
void JPADrawCalcAlpha::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcAlphaFlickNrmSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcAlphaFlickAddSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcAlphaFlickMultSin::calc(const JPADrawContext*, JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexNormal::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexRepeat::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexReverse::calc(const JPADrawContext*,
                                             JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexMerge::calc(const JPADrawContext*,
                                           JPABaseParticle*)
{
}
void JPADrawCalcTextureAnmIndexRandom::calc(const JPADrawContext*,
                                            JPABaseParticle*)
{
}
void JPADrawCalcChildAlphaOut::calc(const JPADrawContext*, JPABaseParticle*) { }
void JPADrawCalcChildScaleOut::calc(const JPADrawContext*, JPABaseParticle*) { }
