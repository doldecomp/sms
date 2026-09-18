#include <System/MarDirector.hpp>
#include <System/PerformList.hpp>
#include <System/RenderModeObj.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewport.hpp>

// TODO: which headers are these infections strings from?
// See InfectiousStrings.hpp also
static const char* dummy                 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE = "メモリが足りません\n";

// rogue include: the pollution-texture pair sits immediately after the
// DummyStrings pair in retail's .rodata blob (0x20 and 0x44).
#include <Player/MarioDirtyStrings.hpp>

// Parked levels for a JDRNameRefGen.hpp item (closure batch 115). The ROM's
// `JDrama::TNameRefGen::search<T>` binds its result before returning it -- the
// same "named-and-returned result" carrier batch 108 found for `search2` and
// parked as `ECTSearch` in MarDirectorInitECT.cpp -- which is worth **+12** of
// low region per expansion here; `search2` itself does *not* bind. Both are
// parked TU-locally and TU-prefixed because the header batch owns that file and
// 40-odd other units expand these.
//
// `preEntry` was 56 low bytes short (frame 0xb0 against 0xe8) with a single
// referenced local, the `SMSGetRederRect_Game()` return buffer. The full
// combination sweep over the five search sites (each raw / binding level /
// `search2` plus a cast) is:
//
//   5 raw                              0xb0, buffer 0x80
//   n binding levels, n = 1..5         0xc0 / 0xc8 / 0xd8 / 0xe0 / 0xf0
//   5 x `search2` + cast               0xb8, buffer 0x84 (the cast temporary,
//                                      one 4-byte slot for all five sites)
//   4 binding levels + `search2`       **0xe8**, buffer 0xb4
//
// so the shape is "four sites through the binding `search<T>`, the indirect
// scene through `search2`", and the only residue left is that buffer, 4 bytes
// low. Rejected for those 4 bytes: a named `JDrama::TNameRef*` intermediate for
// the cast (+0), a named `JDrama::TViewport*` for the `new` (+0 and it swaps
// the push_back argument registers), a named `JDrama::TRect` for the render
// rect (+0x10 and five instructions), and binding `search2`'s result at the
// fifth site too (0xf0).
//
// Batch 131 showed the 4 bytes are an **ordering** difference, not a missing
// pool item: the frame has no slack, so every +4 of pool immediately rounds the
// frame to 0xf0 (measured with a 4-byte non-trivial probe struct -- +4 per
// expansion inside a level, so 0x10 across the four search sites, and +8 of
// frame when placed in the single-expansion `search2` level; a probe in the
// caller's own body, first or last, is absorbed for free, which reconfirms
// batch 123's trailing-dead-scalar rule). Retail therefore has 12 bytes above
// the buffer where we have 16, with the same frame and the same seven
// callee-saved registers. Also +0 here: a bound-`search2`-plus-cast level at
// any one of the four sites (codegen-identical to the binding `search<T>`
// level), `indirectSheen` typed `JDrama::TNameRef*` with the cast moved to the
// `push_back`, the cast folded into `PreEntrySearch2`, an explicit
// `(JDrama::TViewObj*)` on the `new`, and a `static inline` level returning the
// render rect by value.
// Batch 151 read the residue as a position rather than a size: the
// `SMSGetRederRect_Game()` return temporary sits at 0xb8 in retail and 0xb4
// here, so retail holds 4 more bytes of pool *below* that temporary and 4
// fewer above it, at the same 0xe8 frame.
//
// Closed by re-pass 172 with batch 170's nested-fork step. The two header
// entry points are replaced by one TU-local direct-return **fork** over the
// global `JDrama::TNameRefGen::instance`, shared by all five sites: the four
// binding levels keep their +12 each and the fifth site's fork is the missing
// +4, which puts the return buffer at 0xb8 with the frame unchanged. The fork
// has to be spelled *inside* both wrappers -- reached through the header's
// `search<T>`/`search2` it is +0 -- and it has to be used at all five sites:
// nesting it under `search2` alone does land the 0xb8 buffer, but the extra
// inline level makes the first expansion's root-ref temporary coalesce with
// r28 (the scratch the other four expansions use) where retail coalesces it
// with r29, `setViewMtx`'s own register. Routing the four `PreEntrySearch`
// sites through the fork instead of `search<T>` is frame- and register-neutral
// on its own, so it costs nothing to reach the fifth site's +4.
//
// Exhausted for those 4 bytes: a second pointer-returning binding level over
// `PreEntrySearch` at the `camera 1` site alone (+8, 0xf0); binding
// `search2`'s result at the fifth site (+12, 0xf0); a parameterless binder at
// the fifth site with the cast inside (+12, 0xf0); a redundant fork over
// `search2` itself (+0, MWCC collapses two identical forks); a fork over
// `getInstance()` alone (+0); `const JDrama::TRect& rect =
// SMSGetRederRect_Game();` (three inserted instructions); a named
// `JDrama::TNameRef*` intermediate for the cast (+0); a named
// `JDrama::TViewport*` for the `new` (+0, and it swaps the push_back argument
// registers); a named `JDrama::TRect` for the render rect (+0x10 and five
// instructions); `indirectSheen` typed `JDrama::TNameRef*` with the cast moved
// to the `push_back` (+0); the cast folded into `PreEntrySearch2` (+0); an
// explicit `(JDrama::TViewObj*)` on the `new` (+0); and a `static inline`
// level returning the render rect by value (+0).
static inline JDrama::TNameRef* PreEntryRootNameRef()
{
	return JDrama::TNameRefGen::getInstance()->getRootNameRef();
}

static inline JDrama::TViewObj* PreEntrySearch(const char* name)
{
	JDrama::TViewObj* obj = (JDrama::TViewObj*)PreEntryRootNameRef()->search(name);
	return obj;
}

static inline JDrama::TNameRef* PreEntrySearch2(const char* name)
{
	return PreEntryRootNameRef()->search(name);
}

void TMarDirector::preEntry(TPerformList* list)
{
	JDrama::TViewObj* setViewMtx
	    = PreEntrySearch("J3D System Set View Mtx");
	JDrama::TViewObj* mirrorDisplayModelControl
	    = PreEntrySearch("鏡表示モデル管理");
	JDrama::TViewObj* mapGroup
	    = PreEntrySearch("マップグループ");
	JDrama::TViewObj* camera1
	    = PreEntrySearch("camera 1");

	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back(setViewMtx, CUE_CALC_VIEW);
	list->push_back("DrawBuf Sky Opa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Sky Xlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("空グループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("DrawBuf MapOpa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf MapXlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back(mapGroup, CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back(mirrorDisplayModelControl, CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("DrawBuf Map 半透明優先 (opa)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Map 半透明優先 (xlu)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マップ", CUE_SEMITRANSPARENT_PRIO_1 | CUE_ENTRY);
	list->push_back("DrawBuf Map 半透明優先2 (opa)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf Map 半透明優先2 (xlu)",
	                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マップ", CUE_SEMITRANSPARENT_PRIO_2 | CUE_ENTRY);
	list->push_back("DrawBuf Graffito", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("落書きグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("ライトマネージャー", CUE_SET_DRAW_BUFFER);
	list->push_back("DrawBuf ChrOpa", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("DrawBuf ChrXlu", CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
	list->push_back("マネージャーグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("コンダクター", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back(
	    new JDrama::TViewport(SMSGetRederRect_Game(), "vp WParticle 2"),
	    CUE_DRAW);
	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back("水マネージャ", CUE_CALC_VIEW);
	list->push_back("水飛沫マネージャ", CUE_CALC_VIEW);
	list->push_back("クエッションマネージャ", CUE_CALC_VIEW);

	JDrama::TViewObj* indirectSheen
	    = (JDrama::TViewObj*)PreEntrySearch2("インダイレクトシーン");
	if (indirectSheen) {
		list->push_back("DrawBuf Indirect",
		                CUE_SET_DRAW_BUFFER | CUE_DRAW_INIT);
		list->push_back(indirectSheen,
		                CUE_UNK40000000 | CUE_ENTRY | CUE_CALC_VIEW);
	}
	list->push_back(camera1, CUE_SET_PROJECTION);
	list->push_back(setViewMtx, CUE_CALC_VIEW);
	list->push_back("プレーヤーグループ", CUE_UNK10000000);
	list->push_back("プレーヤーグループ", CUE_CALC_VIEW | CUE_ENTRY);
	list->push_back("プレーヤーグループ", CUE_UNK8000000);
}
