#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>

namespace JASystem {

namespace DSPInterface {

	u32 SEND_TABLE     = 0;
	u32 JAS_DSP_PREFIX = 0;
	u32 CH_BUF         = 0;
	u32 FX_BUF         = 0;

	void DSPBuffer::flushChannel() { }
	void DSPBuffer::allocInit() { }
	void DSPBuffer::initFilter() { }
	void DSPBuffer::setAutoMixer(u16, u8, u8, u8, u8) { }
	void DSPBuffer::setMixerVolumeOnly(u8, s16) { }
	void DSPBuffer::setPitch(u16) { }
	void DSPBuffer::setPauseFlag(u8) { }
	void DSPBuffer::setMixerVolume(u8, s16, u8) { }
	void DSPBuffer::setIIRFilterParam(s16*) { }
	void DSPBuffer::setFIR8FilterParam(s16*) { }
	void DSPBuffer::setFilterMode(u16) { }
	void DSPBuffer::setDistFilter(s16) { }
	void DSPBuffer::setWaveInfo(Driver::Wave_*, u32) { }
	void DSPBuffer::setOscInfo(u32) { }
	void DSPBuffer::setBusConnect(u8, u8) { }
	void DSPBuffer::initAutoMixer() { }
	void DSPBuffer::setMixerInitDelayMax(u8) { }
	void DSPBuffer::setMixerInitVolume(u8, s16, u8) { }
	void DSPBuffer::playStart() { }
	void DSPBuffer::playStop() { }
	void DSPBuffer::updateAMVolume(u16) { }
	void DSPBuffer::updateAMPan(u8, u8) { }
	void DSPBuffer::updateAMFX(u8) { }
	void DSPBuffer::setPitchIndirect(f32, f32) { }
	void DSPBuffer::cacheChannel() { }

	void FXBuffer::setFXLine(s16*, FxlineConfig_*) { }
	void FXBuffer::changeFXLineParam(u8, u32) { }

	DSPBuffer* getDSPHandle(u8) { return 0; }
	DSPBuffer* getDSPHandleNc(u8) { return 0; }
	FXBuffer* getFXHandle(u8) { return 0; }
	FXBuffer* getFXHandleNc(u8) { return 0; }
	void setFilterTable(s16*, s16*, u32) { }
	void flushBuffer() { }
	void flushChannelAll() { }
	void cacheChannelAll() { }
	void invalChannelAll() { }
	void clearBuffer() { }
	void setupBuffer() { }
	void initBuffer() { }

} // namespace DSPInterface

} // namespace JASystem
