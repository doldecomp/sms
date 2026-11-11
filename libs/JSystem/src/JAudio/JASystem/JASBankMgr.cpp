#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBNKParser.hpp>
#include <JSystem/JAudio/JASystem/JASWSParser.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASInst.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>

namespace JASystem {
namespace BankMgr {

	static s16 OSC_RELEASE_TABLE[6] = {
		TOscillator::CURVE_SQUARE, 10, 0, TOscillator::CURVE_HALT, 0, 0,
	};
	static TOscillator::Osc_ OSC_ENV
	    = { EFFECT_VOLUME, 1.0f, nullptr, OSC_RELEASE_TABLE, 1.0f, 0.0f };
	static s32 sTableSize     = 0;
	static TBank** sBankArray = nullptr;
	static u16* sVir2PhyTable = nullptr;

	void init(int bankCount)
	{
		sBankArray    = new (JASDram, 0) TBank*[bankCount];
		sVir2PhyTable = new (JASDram, 0) u16[bankCount];
		Calc::bzero(sBankArray, bankCount * sizeof(TBank*));

		for (int i = 0; i < bankCount; ++i)
			sVir2PhyTable[i] = -1;

		sTableSize = bankCount;
	}

	bool registBank(int bankIndex, TBank* bank)
	{
		sBankArray[bankIndex] = bank;
		return true;
	}

	bool registBankBNK(int bankIndex, void* bankData)
	{
		setVir2PhyTable(*((u32*)(bankData) + 2), bankIndex);
		TBasicBank* bank = BNKParser::createBasicBank(bankData);
		if (bank == nullptr)
			return false;

		return registBank(bankIndex, bank);
	}

	TBank* getBank(int bankIndex)
	{
		if (bankIndex >= sTableSize)
			return nullptr;

		return sBankArray[bankIndex];
	}

	u16 getPhysicalNumber(u16 vir_id) { return sVir2PhyTable[vir_id]; }

	void setVir2PhyTable(u32 vir_id, int banknum)
	{
		if (vir_id == 0xFFFF)
			return;

		JUT_ASSERT(vir_id < sTableSize);

		if (sVir2PhyTable[vir_id] != 0xFFFF)
			JUT_REPORT_MSG("Warning : Duplicated Bank vir_id ID %d (%d,%d)\n",
			               vir_id, sVir2PhyTable[vir_id], banknum);

		sVir2PhyTable[vir_id] = banknum;
	}

	bool assignWaveBank(int bankIndex, int waveBankIndex)
	{
		TBank* bank = getBank(bankIndex);
		if (!bank)
			return false;

		TWaveBank* waveBank = WaveBankMgr::getWaveBank(waveBankIndex);
		if (!waveBank)
			return false;

		bank->assignWaveBank(waveBank);
		return true;
	}

	f32 clamp01(f32 value)
	{
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		return value;
	}

	static TChannel* noteOnOsc(TChannelMgr* param_1, int waveform_type, u8 key,
	                           u8 velocity, u32 gate_time);

	TChannel* noteOn(TChannelMgr* chan_mgr, int bank_no, int prog_no, u8 key,
	                 u8 velocity, u32 gate_time)
	{
		if (prog_no > 0xEF)
			return noteOnOsc(chan_mgr, prog_no - 0xF0, key, velocity,
			                 gate_time);

		TBank* bank = getBank(bank_no);
		if (!bank)
			return nullptr;

		TInst* inst = bank->getInst(prog_no);
		if (!inst)
			return nullptr;

		TInstParam instParam;
		if (!inst->getParam(key, velocity, &instParam))
			return nullptr;

		TWaveBank* waveBank = bank->getWaveBank();
		if (!waveBank)
			return nullptr;

		TWaveHandle* waveHndl = waveBank->getWaveHandle(instParam.mWaveId);
		if (!waveHndl)
			return nullptr;

		const TWaveInfo* waveInfo = waveHndl->getWaveInfo();
		if (!waveInfo)
			return nullptr;

		const void* wave = waveHndl->getWavePtr();
		if (!wave)
			return nullptr;

		u32 polyphonyKey
		    = (instParam.mPolyphonyFlags << 0x18) | (bank_no << 8) | prog_no;
		switch (instParam.mPolyphonyFlags & 0xC0) {
		case 0xC0:
			polyphonyKey |= 0xffffff;
			break;
		case 0x80:
			polyphonyKey |= 0xff;
			break;
		case 0x40: {
			u32 keyRegionIdx = instParam.mKeyRegionIndex;
			keyRegionIdx <<= 0x10;
			polyphonyKey |= keyRegionIdx;
			break;
		}
		}

		TChannel* result = chan_mgr->getLogicalChannel(polyphonyKey);
		if (!result)
			return nullptr;

		result->mWaveData        = (Driver::Wave_*)waveInfo;
		result->mSourceData      = (uintptr_t)wave;
		result->mLogicalChanType = instParam.mSourceType;
		result->mVelocity        = velocity;
		result->mKey             = key;
		result->mBasePitch
		    = instParam.mPitch * (waveInfo->mSampleRate / Kernel::getDacRate());
		result->mNotePitch = result->mBasePitch * instParam.mEffectPitch;
		if (instParam.mFixedPitch == 0) {
			int var = (key + 0x3C) - waveInfo->mKey;
			if (var < 0)
				var = 0;
			if (var > 0x7f)
				var = 0x7f;
			result->mNotePitch *= Driver::C5BASE_PITCHTABLE[var];
		}
		result->mInstrumentVolume = instParam.mVolume;
		result->mNoteVolume       = result->mVelocity / 127.0f;
		result->mNoteVolume       = result->mInstrumentVolume
		                      * (result->mNoteVolume * result->mNoteVolume);
		result->mNoteVolume *= instParam.mEffectVolume;

		result->mMixInputs[MIX_PAN].mInstrument   = instParam.mPan;
		result->mMixInputs[MIX_FXMIX].mInstrument = instParam.mFxmix;
		result->mMixInputs[MIX_DOLBY].mInstrument = instParam.mDolby;

		for (int i = 0; i < 2; ++i)
			result->mMixInputs[i].mInstrument
			    = clamp01(result->mMixInputs[i].mInstrument);

		result->mMixInputs[MIX_PAN].mModulation   = instParam.mEffectPan;
		result->mMixInputs[MIX_FXMIX].mModulation = instParam.mEffectFxmix;
		result->mMixInputs[MIX_DOLBY].mModulation = instParam.mEffectDolby;

		result->mOscPitch  = 1.0f;
		result->mOscVolume = 1.0f;

		for (u32 i = 0; i < instParam.mOscCount; ++i)
			result->setOscInit(i, instParam.mOscData[i]);

		result->directReleaseOsc(0, instParam.mRelease);
		if (!result->play(gate_time))
			return nullptr;

		return result;
	}

	static TChannel* noteOnOsc(TChannelMgr* chan_mgr, int waveform_type, u8 key,
	                           u8 velocity, u32 gate_time)
	{
		TChannel* channel = chan_mgr->getLogicalChannel(0);
		if (!channel)
			return nullptr;

		channel->mSourceData      = waveform_type;
		channel->mLogicalChanType = 2;
		channel->mVelocity        = velocity;
		channel->mKey             = key;
		channel->mBasePitch       = 16736.016f / Kernel::getDacRate();
		channel->mNotePitch       = channel->mBasePitch;

		s32 pitchIdx = key;
		if (pitchIdx < 0)
			pitchIdx = 0;

		if (pitchIdx > 127)
			pitchIdx = 127;

		channel->mNotePitch *= Driver::C5BASE_PITCHTABLE[pitchIdx];
		channel->mInstrumentVolume = 1.0f;
		channel->mNoteVolume       = channel->mVelocity / 127.0f;
		channel->mNoteVolume       = channel->mInstrumentVolume
		                       * (channel->mNoteVolume * channel->mNoteVolume);

		channel->mMixInputs[MIX_PAN].mInstrument   = 0.5f;
		channel->mMixInputs[MIX_FXMIX].mInstrument = 0.0f;
		channel->mMixInputs[MIX_DOLBY].mInstrument = 0.0f;
		channel->mMixInputs[MIX_PAN].mModulation   = 0.5f;
		channel->mMixInputs[MIX_FXMIX].mModulation = 0.0f;
		channel->mMixInputs[MIX_DOLBY].mModulation = 0.0f;

		channel->mOscPitch  = 1.0f;
		channel->mOscVolume = 1.0f;
		channel->setOscInit(0, &OSC_ENV);
		channel->mPolyphonyKey = 0;

		if (!channel->play(gate_time))
			return nullptr;

		return channel;
	}

	void gateOn(TChannel* channel, u8 key, u8 velocity, u32 gate_time)
	{
		if (channel->mGateTime != -1)
			return;

		channel->mGateTime          = gate_time;
		channel->mGateTimeRemaining = channel->mGateTime;
		s32 var;
		if (channel->mLogicalChanType == 2)
			var = key;
		else
			var = (key + 0x3C) - channel->mWaveData->mKey;

		if (var < 0)
			var = 0;
		if (var > 0x7F)
			var = 0x7F;

		f32 pitch            = Driver::C5BASE_PITCHTABLE[var];
		channel->mVelocity   = velocity;
		channel->mKey        = key;
		channel->mNotePitch  = channel->mBasePitch * pitch;
		channel->mNoteVolume = channel->mVelocity / 127.0f;
		channel->mNoteVolume = channel->mInstrumentVolume
		                       * (channel->mNoteVolume * channel->mNoteVolume);
	}

	u32 getUsedHeapSize() { return BNKParser::getUsedHeapSize(); }

} // namespace BankMgr
} // namespace JASystem
