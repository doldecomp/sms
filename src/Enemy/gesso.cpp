#include <Enemy/Gesso.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TGessoManager* gpCurGesso;

const char* rikuGesso_bastable[] = {
	nullptr,
	nullptr,
	"/scene/rikuGesso/bas/geso_180turn1.bas",
	"/scene/rikuGesso/bas/geso_down1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_gero1.bas",
	"/scene/rikuGesso/bas/geso_hit1.bas",
	"/scene/rikuGesso/bas/geso_land1.bas",
	"/scene/rikuGesso/bas/geso_mahi_end1.bas",
	"/scene/rikuGesso/bas/geso_mahi_loop1.bas",
	"/scene/rikuGesso/bas/geso_mahi_start1.bas",
	"/scene/rikuGesso/bas/geso_punch1.bas",
	"/scene/rikuGesso/bas/geso_run1.bas",
	"/scene/rikuGesso/bas/geso_search1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_surf1.bas",
	"/scene/rikuGesso/bas/geso_turn_down1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_turn_hit1.bas",
	"/scene/rikuGesso/bas/geso_turn_up1.bas",
	nullptr,
	"/scene/rikuGesso/bas/geso_wait1.bas",
	nullptr,
};

TGessoSaveLoadParams::TGessoSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
{
}

void TGessoPolluteModelManager::init(TLiveActor*) { }

void TGessoPolluteModel::setAnm() { }

TGessoManager::TGessoManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurGesso = this;
}

void TGessoManager::load(JSUMemoryInputStream&) { }

void TGessoManager::loadAfter() { }

TSmallEnemy* TGessoManager::createEnemyInstance() { }

void TGessoManager::clipEnemies(JDrama::TGraphics*) { }

void TGessoManager::perform(u32, JDrama::TGraphics*) { }

void TGessoManager::initSetEnemies() { }

void TGessoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "geso_model1.bmd", 0x10230000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TGessoManager::requestPolluteModel(JGeometry::TVec3<float>&,
                                        JGeometry::TVec3<float>&)
{
}

static void GessoBodyCallback(J3DNode*, int) { }

u8 TGesso::mBodyJntIndex       = 12;
u8 TGesso::mMouthJntIndex      = 13;
f32 TGesso::mBodyRotSpeed      = 0.5f;
f32 TGesso::mPollRange         = 8.0f;
f32 TGesso::mThroughHoseiDistY = 200.0f;
f32 TGesso::mAngTestY          = 0.15f;

TGesso::TGesso(const char* name)
    : TWalkerEnemy(name)
{
}

void TGesso::init(TLiveManager*) { }

void TGesso::reset() { }

void TGesso::load(JSUMemoryInputStream&) { }

void TGesso::behaveToWater(THitActor*) { }

void TGesso::walkBehavior(int, float) { }

void TGesso::isResignationAttack() { }

void TGesso::doKeepDistance() { }

void TGesso::attackToMario() { }

void TGesso::setBehavior() { }

void TGesso::polluteBehavior() { }

void TGesso::setPolluteGoal() { }

void TGesso::pollute() { }

void TGesso::isUseBodyCallBack() const { }

void TGesso::setAfterDeadEffect() { }

void TGesso::setDeadAnm() { }

f32 TGesso::getGravityY() const { }

void TGesso::setWalkAnm() { }

void TGesso::setWaitAnm() { }

void TGesso::bind() { }

void TGesso::kill() { }

void TGesso::calcRootMatrix() { }

void TGesso::genRandomItem() { }

void TGesso::behaveToFindMario() { }

void TGesso::rollCheck() { }

void TGesso::rollEnd() { }

void TGesso::modifyRotate() { }

void TGesso::fallEnd() { }

void TGesso::turnIn() { }

void TGesso::turning() { }

void TGesso::turnOut() { }

void TGesso::checkDropInWater() { }

void TGesso::initAttacker(THitActor*) { }

const char** TGesso::getBasNameTable() const { return rikuGesso_bastable; }

void TSurfGesso::load(JSUMemoryInputStream&) { }

void TSurfGesso::perform(u32, JDrama::TGraphics*) { }

void TLandGesso::load(JSUMemoryInputStream&) { }

TGessoPolluteObj::TGessoPolluteObj(const char* name)
    : TEnemyAttachment(name)
{
}

void TGessoPolluteObj::loadInit(TSpineEnemy*, const char*) { }

f32 TGessoPolluteObj::getNowGravity() { }

void TGessoPolluteObj::pollute() { }

void TGessoPolluteObj::rebirth() { }

void TGessoPolluteObj::set() { }

void TGessoPolluteObj::calcRootMatrix() { }

void TGessoPolluteObj::sendMessage() { }

DEFINE_NERVE(TNerveGessoStay, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFreeze, TLiveActor) { }

DEFINE_NERVE(TNerveGessoPunch, TLiveActor) { }

DEFINE_NERVE(TNerveGessoPollute, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFall, TLiveActor) { }

DEFINE_NERVE(TNerveGessoRolling, TLiveActor) { }

DEFINE_NERVE(TNerveGessoFindMario, TLiveActor) { }

DEFINE_NERVE(TNerveGessoLand, TLiveActor) { }

DEFINE_NERVE(TNerveGessoTurn, TLiveActor) { }
