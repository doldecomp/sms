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
	J2DSetScreen* v14 = p->getScreen();
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

	// Closed by re-pass 172.  The last 4 bytes of low region are a
	// **nested direct-return fork**: `MovieSubtitleUnk14`'s binding now reads
	// the member through `TMovieSubTitle::getScreen()` instead of `p->unk14`
	// directly, which is +4 on top of the binding's +8 and puts `buffer` at
	// retail's 0x28 with zero instruction change.  Batch 170's ladder said the
	// nested fork had to be over a global or a file-local static; it is in fact
	// +4 over a plain **member** read too, and the fork has to be nested inside
	// the binding (spelled at the call site it is +0).
	//
	// Earlier history, all still true.  Batch 127's binding took the frame from
	// 0x138 to the exact 0x140 and left `buffer` at 0x24; batch 129 sized the
	// missing object from the callee side (one uninitialised non-trivial 12-byte
	// local in the UNUSED `makeBmgName` lands it exactly, 8 bytes gives the
	// frame but `buffer` 0x24, 16 overshoots to 0x148) but no 12-byte class is a
	// candidate in a string-building helper.
	//
	// Exhausted spellings for the missing 4, measured here and in batches
	// 127/129/151: a binding inside a new `getScreen()` accessor is +8 and two
	// of them +0x10; a fork over `gpApplication` at the call site is +0 both as
	// `&gpApplication` and as a `u32` wrapper, and so is the same fork nested
	// one level deep (`get_movie_id()` over `get_application()`); a fork over
	// the file-local `cLongHeightMovieIdList` inside `is_longheight_movie` is +0
	// (a bool-returning host does not price it); a pointer level inside
	// `is_longheight_movie` is +8 plus one instruction; a consumed pointer
	// binding inside `hide()` is +4 and does close this function, but it pays
	// the same +4 into `perform`'s chain (the `J2DOrthoGraph` temp moves 0x54 ->
	// 0x58), so it cannot be spent here; naming the movie id, naming `unk14`
	// before the two `search()` calls, a `char* blank` alias for the memset,
	// `sizeof` instead of `ARRAY_COUNT`, and moving `buffer` to the top of the
	// body are all inert.

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

// TODO: the map sizes this UNUSED body at 0x4c = 19 instructions and ours
// compiles to 21, so two instructions of it are still wrong even though its
// inlined image in setupResource is exact (sprintf with the format string in
// .rodata, strrchr on '.', strcpy of the .sdata2 ".bmg"). The two extra are in
// the out-of-line prologue/argument shuffle -- `this` takes r3, so `buffer`
// has to be moved to r31 and back -- so the candidate is a body that does not
// need `buffer` live across sprintf. Adding a `char* name = buffer;` binding
// is +2 more instructions and breaks setupResource, so it is not that.
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
