#ifndef JAIINTER_HPP
#define JAIINTER_HPP

#include <types.h>
#include <dolphin/dvd.h>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASDSPChannel.hpp>

namespace JAInter {
namespace StreamLib {
	void stop();
	void setOutputMode(u32 mode);
	void setPauseFlag(u8 flag);
	void clearPauseFlag(u8 flag);
	bool getPlayingFlag();
	void setVolume(f32 volume);
	void setPitch(f32 pitch);
	void setPan(f32 pan);
	void setPrepareFlag(u8 flag);
	void start(char* filename, u32 mode, void* param);
	void Play_DirectPCM(JASystem::TDSPChannel* channel, s16* buffer, u16 param,
	                    u32 size);
	void init(bool mode);
	u32 getNeedBufferSize();
	void allocBuffer(void* buffer, s32 size);
	void deallocBuffer();
	bool checkBufferStatus();
	void setBufferMode(bool mode);
	void sync(s32 param);
	void Hvqm_SetAudioDmaBuffers(u32 buffers);
	void getPrepareFlag();
	void setDecodedBufferBlocks(u32 blocks);
	void LoopInit();
	void* Get_DirectPCM_LoopRemain(JASystem::DSPInterface::DSPBuffer* buffer);
	void* Get_DirectPCM_Counter(JASystem::DSPInterface::DSPBuffer* buffer);
	void* Get_DirectPCM_Remain(JASystem::DSPInterface::DSPBuffer* buffer);
} // namespace StreamLib
} // namespace JAInter

#endif // JAIINTER_HPP
