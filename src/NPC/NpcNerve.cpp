#include <NPC/NpcNerve.hpp>

const TNerveNPCGraphWander& TNerveNPCGraphWander::theNerve()
{
	static TNerveNPCGraphWander instance;
	return instance;
}

void TNerveNPCGraphWander::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCUTurn& TNerveNPCUTurn::theNerve()
{
	static TNerveNPCUTurn instance;
	return instance;
}

void TNerveNPCUTurn::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCGraphWait& TNerveNPCGraphWait::theNerve()
{
	static TNerveNPCGraphWait instance;
	return instance;
}

void TNerveNPCGraphWait::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCWaitContinue& TNerveNPCWaitContinue::theNerve()
{
	static TNerveNPCWaitContinue instance;
	return instance;
}

void TNerveNPCWaitContinue::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCWaitMarioApproach& TNerveNPCWaitMarioApproach::theNerve()
{
	static TNerveNPCWaitMarioApproach instance;
	return instance;
}

void TNerveNPCWaitMarioApproach::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCTurnToMario& TNerveNPCTurnToMario::theNerve()
{
	static TNerveNPCTurnToMario instance;
	return instance;
}

void TNerveNPCTurnToMario::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCWet& TNerveNPCWet::theNerve()
{
	static TNerveNPCWet instance;
	return instance;
}

void TNerveNPCWet::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCSink& TNerveNPCSink::theNerve()
{
	static TNerveNPCSink instance;
	return instance;
}

void TNerveNPCSink::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCRecoverFromSink& TNerveNPCRecoverFromSink::theNerve()
{
	static TNerveNPCRecoverFromSink instance;
	return instance;
}

void TNerveNPCRecoverFromSink::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCRecoverAfter& TNerveNPCRecoverAfter::theNerve()
{
	static TNerveNPCRecoverAfter instance;
	return instance;
}

void TNerveNPCRecoverAfter::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCSetPosAfterSinkBottom& TNerveNPCSetPosAfterSinkBottom::theNerve()
{
	static TNerveNPCSetPosAfterSinkBottom instance;
	return instance;
}

void TNerveNPCSetPosAfterSinkBottom::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCTalk& TNerveNPCTalk::theNerve()
{
	static TNerveNPCTalk instance;
	return instance;
}

void TNerveNPCTalk::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCThrow& TNerveNPCThrow::theNerve()
{
	static TNerveNPCThrow instance;
	return instance;
}

void TNerveNPCThrow::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCMad& TNerveNPCMad::theNerve()
{
	static TNerveNPCMad instance;
	return instance;
}

void TNerveNPCMad::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCBlown& TNerveNPCBlown::theNerve()
{
	static TNerveNPCBlown instance;
	return instance;
}

void TNerveNPCBlown::execute(TSpineBase<TLiveActor>*) const { }

const TNerveNPCMareStand& TNerveNPCMareStand::theNerve()
{
	static TNerveNPCMareStand instance;
	return instance;
}

void TNerveNPCMareStand::execute(TSpineBase<TLiveActor>*) const { }
