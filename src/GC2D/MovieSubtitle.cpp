#include <GC2D/MovieSubtitle.hpp>
#include <stdio.h>
#include <macros.h>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <System/THPRender.hpp>
#include <System/Application.hpp>

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
	if (is_longheight_movie(gpApplication.getMovie()))
		unk14 = new J2DSetScreen("demo_1.blo", param_2);
	else
		unk14 = new J2DSetScreen("demo_2.blo", param_2);

	hide();

	unk18 = (J2DTextBox*)unk14->search('me_a');
	unk1C = (J2DTextBox*)unk14->search('me_b');

	char buffer[256];

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

void TMovieSubTitle::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1)
		movement();

	if (param_1 & 8)
		draw(param_2);
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
	if (unk20->getMessageNum() <= unk24)
		return nullptr;

	return unk20->getMessageEntry(unk24);
}

void TMovieSubTitle::setCurMessage()
{
	const char* msg
	    = (const char*)(unk20->getMessageData() + getCurEntry()->unk0);

	snprintf(unk18->getStringPtr(), 256, "%s", msg);
	snprintf(unk1C->getStringPtr(), 256, "%s", msg);
}

void TMovieSubTitle::makeBmgName(char* buffer, int, const char* param_3)
{
	sprintf(buffer, "/subtitle/%s", param_3);
	char* it = strrchr(buffer, '.');
	strcpy(it, ".bmg");
}

void TMovieSubTitle::draw(JDrama::TGraphics* param_1)
{
	J2DOrthoGraph graph(param_1->getUnk54());
	graph.setup2D();
	unk14->draw(0, 0, &graph);
}
