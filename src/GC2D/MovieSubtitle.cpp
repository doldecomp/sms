#include <GC2D/MovieSubtitle.hpp>
#include <stdio.h>
#include <macros.h>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <System/THPRender.hpp>
#include <System/Application.hpp>
#include <version.h>

#ifdef VERSION_GMSP01
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <System/FlagManager.hpp>
#endif

// TODO: removeme
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

namespace {

const int cLongHeightMovieIdList[] = { 9, 20 };

bool is_longheight_movie(u32 param_1)
{
	const int* i = cLongHeightMovieIdList;
	const int* e = cLongHeightMovieIdList + ARRAY_COUNT(cLongHeightMovieIdList);
	while (i != e && *i != param_1)
		++i;
	return i != e;
}

} // namespace

TMovieSubTitle::TMovieSubTitle(const TTHPRender* param_1)
    : unk10(param_1)
    , unk14(nullptr)
    , unk18(nullptr)
    , unk1C(nullptr)
{
}

void TMovieSubTitle::setupResource(const char* param_1, JKRArchive* param_2)
{
	if (is_longheight_movie(SMSGetApplication()->getMovie()))
		unk14 = new J2DSetScreen("demo_1.blo", param_2);
	else
		unk14 = new J2DSetScreen("demo_2.blo", param_2);

	hide();

	unk18 = (J2DTextBox*)unk14->search('me_a');
	unk1C = (J2DTextBox*)unk14->search('me_b');

	char buffer[VERSION_SELECT(GMSJ01(256), GMSP01(1024))];

	// inline?
	memset(buffer, ' ', ARRAY_COUNT(buffer));
	buffer[ARRAY_COUNT(buffer) - 1] = '\0';
	unk18->setString(buffer);
	unk1C->setString(buffer);

	unk20 = new TMessageLoader;
	makeBmgName(buffer, ARRAY_COUNT(buffer), param_1);
	unk20->loadMessageData(buffer);

	unk24 = 0;
}

void TMovieSubTitle::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		movement();

	if (cue & CUE_DRAW)
		draw(graphics);
}

void TMovieSubTitle::movement()
{
	if (unk28)
		checkSubTitleOff();
	else
		checkSubTitleOn();
}

void TMovieSubTitle::checkSubTitleOff()
{
	int frame = unk10->getFrameNumber();
	if (getCurEntry()) {
		if (getCurEntry()->unk6 <= frame) {
			hide();
			++unk24;
		}
	}
}

void TMovieSubTitle::checkSubTitleOn()
{
	int frame = unk10->getFrameNumber();
	if (getCurEntry()) {
		if (getCurEntry()->unk4 <= frame)
			show();
	}
}

void TMovieSubTitle::show()
{
	unk28 = true;
	unk14->show();
	setCurMessage();
}

void TMovieSubTitle::hide()
{
	unk28 = false;
	unk14->hide();
}

const TMessageLoader::EntryInfo* TMovieSubTitle::getCurEntry() const
{
#ifdef VERSION_GMSP01
	if (unk20->getMessageNum() <= unk24
	    || !TFlagManager::getInstance()->getFlag(0x90001))
		return nullptr;
#else
	if (unk20->getMessageNum() <= unk24)
		return nullptr;
#endif

	return unk20->getMessageEntry(unk24);
}

void TMovieSubTitle::setCurMessage()
{
	const char* msg
	    = (const char*)(unk20->getMessageData() + getCurEntry()->unk0);

#ifdef VERSION_GMSP01
	JSUMemoryInputStream in(msg, 0x400);
	JSUMemoryOutputStream out(unk18->getStringPtr(), 0x400);

	while (in.getAvailable() && out.getAvailable()) {
		u8 c = in.read8b();

		char buffer[256];
		JUtility::TColor color;
		switch (c) {
		case 0x1A:
			in.skip(in.read8b() - 2);
			continue;
		case '\0':
			out << c;
			return;
		case '\n':
			out << c;
			continue;
		}

		bool colored = true;
		switch (c) {
		case '@':
			color.set(100, 255, 100, 255);
			break;
		case '#':
			color.set(255, 160, 100, 255);
			break;
		case '%':
			color.set(255, 255, 0, 255);
			break;
		case '<':
		case '+':
		case '>':
		case 0xA5:
			color.set(220, 220, 220, 255);
			break;
		case '$':
			color.set(110, 230, 255, 255);
			break;
		case '*':
			color.set(220, 220, 220, 255);
			break;
		default:
			colored = false;
			break;
		}

		if (colored) {
			snprintf(buffer, 255,
			         "\033GM[0]\033CC[%02x%02x%02x]\033SH[3]\033CD[4]", color.r,
			         color.g, color.b);
			out.write(buffer, 29);
		}

		out << c;

		if (colored) {
			snprintf(buffer, 255,
			         "\033GM[0]\033CC\033FX\033FY\033SH\033CU[4]\033GM[1]");
			out.write(buffer, 30);
		}
	}

	snprintf(unk1C->getStringPtr(), 0x400, "%s", unk18->getStringPtr());
#else
	snprintf(unk18->getStringPtr(), 256, "%s", msg);
	snprintf(unk1C->getStringPtr(), 256, "%s", msg);
#endif
}

void TMovieSubTitle::makeBmgName(char* buffer, int, const char* param_3)
{
	sprintf(buffer, "/subtitle/%s", param_3);
	char* it = strrchr(buffer, '.');
	strcpy(it, ".bmg");
}

void TMovieSubTitle::draw(JDrama::TGraphics* param_1)
{
	J2DOrthoGraph graph(param_1->getViewport());
	graph.setup2D();
	unk14->draw(0, 0, &graph);
}
