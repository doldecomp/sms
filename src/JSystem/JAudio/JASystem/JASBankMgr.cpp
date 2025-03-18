#include <JSystem/JAudio/JASystem/JASBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASBNKParser.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBankMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChannelMgr.hpp>
#include <JSystem/JAudio/JASystem/JASDriverTables.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASInst.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>

namespace JASystem {
namespace BankMgr {

	static s16 OSC_RELEASE_TABLE[6] = { 0x1, 0xA, 0x0, 0xf, 0x0, 0x0 };
	static TOscillator::Osc_ OSC_ENV
	    = { 0, 1.0f, nullptr, OSC_RELEASE_TABLE, 1.0f, 0.0f };
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

	u16 getPhysicalNumber(u16 virtualNumber)
	{
		return sVir2PhyTable[virtualNumber];
	}

	void setVir2PhyTable(u32 tableAddr, int size)
	{
		if (tableAddr == 0xFFFF)
			return;

		sVir2PhyTable[tableAddr] = size;
	}

	bool assignWaveBank(int bankIndex, int waveBankIndex)
	{
		TBank* bank = getBank(bankIndex);
		if (!bank)
			return false;

		TWaveBank* waveBank = WaveBankMgr::getWaveBank(waveBankIndex);
		if (!waveBank)
			return false;

		bank->unk4 = waveBank;
		return true;
	}

	f32 clamp01(f32 value)
	{
		f32 result = value;
		if (result < 0.0f)
			result = 0.0f;
		else if (result > 1.0f)
			result = 1.0f;
		return result;
	}

	static TChannel* noteOnOsc(TChannelMgr* param_1, int param_2, u8 param_3,
	                           u8 param_4, u32 param_5);

	TChannel* noteOn(TChannelMgr* param_1, int param_2, int param_3, u8 param_4,
	                 u8 param_5, u32 param_6)
	{
		if (param_3 > 0xF0)
			return noteOnOsc(param_1, param_3 - 0xF0, param_4, param_5,
			                 param_6);

		TBank* bank = getBank(param_2);
		if (!bank)
			return nullptr;

		TInst* inst = bank->getInst(param_3);
		if (!inst)
			return nullptr;

		TInstParam params;
		if (!inst->getParam(param_4, param_5, &params))
			return nullptr;

		TWaveBank* waveBank = bank->unk4;
		if (!waveBank)
			return nullptr;

		TWaveHandle* waveHndl = waveBank->getWaveHandle(params.unk4);
		if (!waveHndl)
			return nullptr;

		const TWaveInfo* waveInfo = waveHndl->getWaveInfo();
		if (!waveInfo)
			return nullptr;

		const void* wave = waveHndl->getWavePtr();
		if (!wave)
			return nullptr;

		u32 chanKey = param_3 | params.unk40 << 0x18 | param_2 << 8;
		switch (params.unk40 & 0xC0) {
		case 0xC0:
			chanKey |= 0xffffff;
			break;
		case 0x80:
			chanKey |= 0xff;
			break;
		case 0x40:
			chanKey |= params.unk38 << 0x10;
			break;
		}

		TChannel* chan = param_1->getLogicalChannel(chanKey);
		if (!chan)
			return nullptr;

		// TODO: WTF?
		chan->unk10 = (Driver::Wave_*)waveInfo;
		chan->unk14 = (u32)wave;
		chan->unkC  = params.unk0;
		chan->unk0  = param_5;
		chan->unk1  = param_4;
		chan->unk48 = params.unk14 * (waveInfo->unk4 / Kernel::getDacRate());
		chan->unk50 = chan->unk48 * params.unk18;
		if (params.unk38 == 0) {
			int var = (param_4 + 0x3C) - waveInfo->unk2;
			if (var < 0)
				var = 0;
			if (var > 0x7f)
				var = 0x7f;
			chan->unk50 *= Driver::C5BASE_PITCHTABLE[var];
		}
		chan->unk4C          = params.unk10;
		chan->unk54          = chan->unk0 / 127.0f;
		chan->unk54          = chan->unk4C * (chan->unk54 * chan->unk54);
		chan->unk54          = chan->unk54 * params.unk18;
		chan->unk68.mSound   = params.unk20;
		chan->unk74.mSound   = params.unk24;
		chan->unk80.mChannel = params.unk28;

		chan->unk68.mSound = clamp01(chan->unk68.mSound);
		chan->unk74.mSound = clamp01(chan->unk74.mSound);

		chan->unk68.mEffect = params.unk2C;
		chan->unk74.mEffect = params.unk30;
		chan->unk80.mEffect = params.unk34;

		chan->unk8C = 1.0f;
		chan->unk90 = 1.0f;

		for (int i = 0; i < params.mOscCount; ++i)
			chan->setOscInit(i, params.mOscData[i]);

		chan->directReleaseOsc(0, params.unk3A);
		if (!chan->play(param_6))
			return nullptr;

		return chan;
	}

	static TChannel* noteOnOsc(TChannelMgr* param_1, int param_2, u8 param_3,
	                           u8 param_4, u32 param_5)
	{
		TChannel* channel = param_1->getLogicalChannel(0);
		if (!channel)
			return nullptr;

		channel->unk14 = param_2;
		channel->unkC  = 2;
		channel->unk0  = param_4;
		channel->unk1  = param_3;
		channel->unk48 = 16736.016f / Kernel::getDacRate();
		channel->unk50 = channel->unk48;

		s32 var1 = param_3;
		if (var1 < 0)
			var1 = 0;

		if (var1 > 127)
			var1 = 127;

		channel->unk50 *= Driver::C5BASE_PITCHTABLE[var1];
		channel->unk4C = 1.0f;
		channel->unk54 = channel->unk0 / 127.0f;
		channel->unk54 = channel->unk4C * (channel->unk54 * channel->unk54);

		channel->unk68.mSound  = 0.5f;
		channel->unk74.mSound  = 0.0f;
		channel->unk80.mSound  = 0.0f;
		channel->unk68.mEffect = 0.5f;
		channel->unk74.mEffect = 0.0f;
		channel->unk80.mEffect = 0.0f;

		channel->unk8C = 1.0f;
		channel->unk90 = 1.0f;
		channel->setOscInit(0, &OSC_ENV);
		channel->unkC8 = 0;

		if (!channel->play(param_5))
			return nullptr;

		return channel;
	}

	void gateOn(TChannel* channel, u8 param_2, u8 param_3, u32 param_4)
	{
		if (channel->unk30 != -1)
			return;

		channel->unk30 = param_4;
		channel->unk34 = channel->unk30;
		s32 var;
		if (channel->unkC == 2)
			var = param_2;
		else
			var = (param_2 + 0x3C) - channel->unk10->unk2;

		if (var < 0)
			var = 0;
		if (var > 0x7F)
			var = 0x7F;

		f32 pitch      = Driver::C5BASE_PITCHTABLE[var];
		channel->unk0  = param_3;
		channel->unk1  = param_2;
		channel->unk50 = channel->unk48 * pitch;
		channel->unk54 = channel->unk0 / 127.0f;
		channel->unk54 = channel->unk4C * (channel->unk54 * channel->unk54);
	}

	u32 getUsedHeapSize() { return 0; }

} // namespace BankMgr
} // namespace JASystem
