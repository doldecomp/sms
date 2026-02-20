#include <GC2D/Option.hpp>

namespace {
void tag_to_string(char*, u32) { }

void print_pane_tree(J2DPane*, int) { }
} // namespace

void TArrowControl::update() { }

void TArrowControl::updateAlpha() { }

void TArrowControl::updateScale() { }

void TArrowControl::calcMoveX(int) const { }

TBalloonControl::TBalloonControl(int) { }

void TBalloonControl::add(TExPane*) { }

void TBalloonControl::setupAnm() { }

void TBalloonControl::startAnm() { }

void TBalloonControl::stopAnm() { }

void TBalloonControl::update() { }

TPaneScalingControl::TPaneScalingControl(J2DPane*) { }

void TPaneScalingControl::setupAnm(float, float) { }

void TPaneScalingControl::startAnm() { }

void TPaneScalingControl::stopAnm() { }

void TPaneScalingControl::update() { }

TPatternAnmControl::TPatternAnmControl(J2DScreen*) { }

void TPatternAnmControl::set(const TPatternAnmControl::TAnmChunk*, int) { }

void TPatternAnmControl::setupAnm() { }

void TPatternAnmControl::update() { }

void TPatternAnmControl::show() { }

void TPatternAnmControl::hide() { }

TToggleControl::TToggleControl(J2DScreen*) { }

void TToggleControl::setupToggle(const u32*, int) { }

void TToggleControl::toggle() { }

void TToggleControl::getNumber() const { }

void TToggleControl::setNumber(int) { }

TOptionRumbleUnit::TOptionRumbleUnit(J2DScreen*) { }

void TOptionRumbleUnit::update() { }

void TOptionRumbleUnit::checkRumble() { }

void TOptionRumbleUnit::toggle() { }

void TOptionRumbleUnit::adjust() { }

void TOptionRumbleUnit::adjustView() { }

void TOptionRumbleUnit::show() { }

void TOptionRumbleUnit::hide() { }

void TOptionRumbleUnit::deactivate(bool) { }

void TOptionRumbleUnit::activate() { }

void TOptionRumbleUnit::setValue(TOptionRumbleUnit::RumbleType) { }

void TOptionRumbleUnit::setState(TOptionRumbleUnit::State) { }

void TOptionRumbleUnit::setInfluencedAlphaRecursive(J2DPane*, bool) { }

TOptionSoundUnit::TOptionSoundUnit(J2DScreen*) { }

void TOptionSoundUnit::initMonoAnm() { }

void TOptionSoundUnit::initSteleoAnm() { }

void TOptionSoundUnit::initSurroundAnm() { }

void TOptionSoundUnit::update() { }

void TOptionSoundUnit::updatePatternAnm() { }

void TOptionSoundUnit::foreachPatternAnm(ArrayWrapper<TPatternAnmControl*>&,
                                         void (TPatternAnmControl::*)())
{
}

void TOptionSoundUnit::toggle() { }

void TOptionSoundUnit::adjust() { }

void TOptionSoundUnit::show() { }

void TOptionSoundUnit::hide() { }

void TOptionSoundUnit::deactivate(bool) { }

void TOptionSoundUnit::activate() { }

void TOptionSoundUnit::setValue(int) { }

void TOptionSoundUnit::getValue() const { }

void TOptionSoundUnit::stopSound() { }

void TOptionSoundUnit::flagToType(int) { }

void TOptionSoundUnit::typeToFlag(TOptionSoundUnit::SoundType) { }

void TOptionSoundUnit::setState(TOptionSoundUnit::State) { }

void TOptionSoundUnit::adjustView() { }

void TOptionSoundUnit::adjustSound() { }

void TOptionSoundUnit::setInfluencedAlphaRecursive(J2DPane*, bool) { }

void TOptionControl::load() { }

void TOptionControl::loadSetting() { }

void TOptionControl::movementCommon() { }

void TOptionControl::draw(J2DOrthoGraph*) { }

void TOptionControl::movementCard2Option() { }

void TOptionControl::movementOption() { }

void TOptionControl::movementOption2Card() { }

void TOptionControl::setType(TOptionControl::SelectType, bool) { }

void TOptionControl::toggleCurType() { }

void TOptionControl::checkInput() { }

void TOptionControl::writeValue() { }

void TOptionControl::isChangedSetting() const { }

void TOptionControl::resetChangedSetting() { }
