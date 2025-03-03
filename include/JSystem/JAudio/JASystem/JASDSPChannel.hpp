#ifndef JASDSPCHANNEL_HPP
#define JASDSPCHANNEL_HPP

namespace JASystem {

class TDSPChannel {
public:
	TDSPChannel();
	~TDSPChannel();

	static void initAll();
	static void updateAll();
	static TDSPChannel* alloc(unsigned long, unsigned long);
	static void free(TDSPChannel*, unsigned long);
	static TDSPChannel* getHandle(unsigned long);
	static unsigned long getNumUse();
	static unsigned long getNumFree();
	static void setLimitDSP(float);
	static float* getHistory();

	void init(unsigned char);
	void allocate(unsigned long);
	void free();
	void play();
	void stop();
	void pause();
	void restart();
	void forceStop();
	void forceDelete();
	unsigned char getLower();
	unsigned char getLowerActive();
	void breakLower(unsigned char);
	void breakLowerActive(unsigned char);

	static TDSPChannel* DSPCH;
	static unsigned long smnUse;
	static unsigned long smnFree;
	static float DSP_LIMIT_RATIO;
	static float* history;
	static unsigned long old_time;
};

} // namespace JASystem

#endif // JASDSPCHANNEL_HPP
