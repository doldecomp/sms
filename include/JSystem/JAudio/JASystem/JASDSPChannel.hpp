#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

#include <dolphin/types.h>

namespace JASystem {

class TDSPChannel {
private:
public:
	TDSPChannel();
	~TDSPChannel();

	void initAll();
	void updateAll();
	TDSPChannel* alloc(u32 param1, u32 param2);
	void free(TDSPChannel* channel, u32 param);
	TDSPChannel* getHandle(u32 handle);
	u32 getNumUse();
	u32 getNumFree();
	void setLimitDSP(f32 limit);
	f32* getHistory();

	void init(u8 param);
	void allocate(u32 param);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	void forceStop();
	void forceDelete();
	u8 getLower();
	u8 getLowerActive();
	void breakLower(u8 param);
	void breakLowerActive(u8 param);

	static TDSPChannel* DSPCH;
	static u32 smnUse;
	static u32 smnFree;
	static f32 DSP_LIMIT_RATIO;
	static f32* history;
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
