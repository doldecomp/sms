#ifndef JDR_NAME_REF_GEN_HPP
#define JDR_NAME_REF_GEN_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>

class JSUMemoryInputStream;

namespace JDrama {

class TNameRefGen {
public:
	virtual TNameRef* load(JSUMemoryInputStream&);
	virtual TNameRef* getNameRef(const char*) const;

	TNameRef* getRootNameRef() { return mRootNameRef; }
	static TNameRefGen* getInstance() { return instance; }

	// TODO: starting to have real doubts that this was a template.
	// fabricated
	//
	// The cast is applied straight to the call on purpose. Binding the
	// TNameRef* first (`TNameRef* ref = ...; return static_cast<T*>(ref);`)
	// is worth +12 of frame per expansion with no instruction change, and
	// header round 21 measured it project-wide on this population (~250
	// sites, a different one from `search2`'s below): it breaks the DOL and
	// is total -60 exact functions, matched_code 51.94 -> 51.33, with 99
	// function regressions against 23 improvements. The losses are one
	// pattern -- the `init`/`load`/`loadAfter` families that reach `search`
	// once and are byte-exact now (TBee, TButterfly, TYumboSeed,
	// TSpineEnemy::load, TLiveActor::load, TYoshiTongue::initInLoadAfter,
	// TAirportSwitch, TSakuCasino, ... and twenty EventWatcher builtins).
	// So this is per call site exactly like `search2`: the sites that want
	// the binding get it from the `PreEntrySearch` wrapper parked in
	// MarDirectorPreEntry.cpp.
	template <class T> static T* search(const char* name)
	{
		return static_cast<T*>(
		    JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(name));
	}

	// The result is returned straight out of the call on purpose. Binding it
	// first (`TNameRef* result = ...; return result;`) costs +8 or +16 of
	// frame per expansion with no instruction change, and header round 19
	// measured that project-wide: it wins only the four MarDirectorInitECT
	// functions (`setupPerformList_console` 99.75 -> 99.90, `initECDisp`
	// 97.50 -> 97.54, `initECTGft` 94.16 -> 94.18) and loses eighteen
	// byte-exact ones across the other ~40 users -- three TItemManager
	// makeShineAppear* overloads, `TSandCastle::findTriggerActor` and
	// `loadAfter`, `TLightWithDBSetManager::loadAfter`,
	// `TBathtubPeach::calcRootMatrix`, `newAndRegisterObjByEventID`,
	// `TPerformList::push_back` and nine EventWatcher builtins. Total
	// matched_code 49.81 -> 49.65. The level is per-call-site: the sites that
	// want it already get it from the `ECTSearch` wrapper parked in
	// MarDirectorInitECT.cpp.
	static JDrama::TNameRef* search2(const char* name)
	{
		return JDrama::TNameRefGen::getInstance()->getRootNameRef()->search(
		    name);
	}

	static TNameRefGen* instance;

public:
	TNameRef* mRootNameRef;
};

} // namespace JDrama

#endif
