#include <MoveBG/MapObjTumblePole.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

JUtility::TColor TMapObjTumblePole::mColorUpper = -1;
JUtility::TColor TMapObjTumblePole::mColorLower = -1;

static void dummy(const JGadget::TList<void*>::iterator& it)
{
	JGadget::TList_pointer<THitActor*>::iterator copy = it;
}
static void dummy2(const JGadget::TList<void*>::iterator& it) { dummy(it); }
static void dummy3(const JGadget::TList<void*>::iterator& it) { dummy2(it); }
static void dummy4(const JGadget::TList<void*>::iterator& it) { dummy3(it); }
static void dummy5(const JGadget::TList<void*>::iterator& it) { dummy4(it); }

void TMapObjTumblePole::draw() { }

void TMapObjTumblePole::fallDown() { }

void TMapObjTumblePole::movement() { }

void TMapObjTumblePole::init(float, float, float) { }

void TMapObjTumblePole::load(JSUMemoryInputStream&) { }

TMapObjTumblePole::TMapObjTumblePole(const char* name)
    : TMapObjBase(name)
{
}

void TMapObjTumblePoleManager::initDraw() { }

void TMapObjTumblePoleManager::loadAfter() { }

TMapObjTumblePoleManager::TMapObjTumblePoleManager(const char* name)
    : JDrama::TViewObj(name)
{
}
