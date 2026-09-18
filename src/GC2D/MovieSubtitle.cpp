#include <GC2D/MovieSubtitle.hpp>
#include <stdio.h>
#include <macros.h>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <System/THPRender.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>

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

// Binding level over a raw member read, worth +8 of low region in
// TMovieSubTitle::setupResource (batch 127).
static inline J2DSetScreen* MovieSubtitleUnk14(const TMovieSubTitle* p)
{
	J2DSetScreen* v14 = p->unk14;
	return v14;
}

void TMovieSubTitle::setupResource(const char* param_1, JKRArchive* param_2)
{
	if (is_longheight_movie(gpApplication.getMovie()))
		unk14 = new J2DSetScreen("demo_1.blo", param_2);
	else
		unk14 = new J2DSetScreen("demo_2.blo", param_2);

	hide();

	unk18 = (J2DTextBox*)MovieSubtitleUnk14(this)->search('me_a');
	unk1C = (J2DTextBox*)unk14->search('me_b');

	char buffer[256];

	// TODO: setupResource is instruction-identical but eight bytes of frame
	// short (0x140 vs 0x138), and the missing bytes sit *below* `buffer`:
	// retail puts `buffer` at 0x28(r1), we put it at 0x1c(r1), so there is a
	// 12-byte object between the inline-temporary region and `buffer` that we
	// do not reproduce. Measured: a `char[9..12]` declared right after
	// `buffer` reaches 100% with no instruction change; `char[5..8]` gives the
	// right frame but leaves `buffer` 4 bytes low; `char[13..16]` overshoots to
	// 0x148; anything declared *before* `buffer` never lands. Nothing in the
	// function wants a second buffer, and the two .blo names and ".bmg" are
	// all .rodata/.sdata2 literals, so the 12 bytes are more likely one more
	// inline expansion (the temp region is 32 bytes in retail, 20 in ours)
	// than a named local. Rejected: naming the movie id, naming unk14 before
	// the two search() calls (97.2%), a `char* blank` alias for the memset
	// (98.1%), sizeof instead of ARRAY_COUNT, and moving `buffer` to the top
	// of the body (all no change).

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

const JMSMesgEntry* TMovieSubTitle::getCurEntry() const
{
	// 0x90001 is the subtitle option flag (TOptionControl::writeValue writes
	// the subtitle unit's value there), so subtitles off means no entry.
	if (unk20->getMessageNum() <= unk24
	    || !TFlagManager::getInstance()->getFlag(0x90001))
		return nullptr;

	return unk20->getMessageEntry(unk24);
}

void TMovieSubTitle::setCurMessage()
{
	const char* msg
	    = (const char*)(unk20->getMessageData() + getCurEntry()->mTextOffset);

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
	J2DOrthoGraph graph(param_1->getViewport());
	graph.setup2D();
	unk14->draw(0, 0, &graph);
}
