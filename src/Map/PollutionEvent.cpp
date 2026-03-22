#include <Map/TPollutionTest.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TPollutionTest::perform(u32, JDrama::TGraphics*) { }

TPollutionTest::~TPollutionTest() { }

void TPollutionTest::loadAfter() { TNameRef::loadAfter(); }
