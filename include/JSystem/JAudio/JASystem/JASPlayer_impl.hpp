#ifndef JASPLAYER_IMPL_HPP
#define JASPLAYER_IMPL_HPP

namespace JASystem {

namespace Player {
	extern float* sEnvelopeDef;
	extern float* sRelTable;
	extern float* sVibratoDef;
	extern float* sVibTable;
	extern float* s_key_table;
	extern float* sAdsTable;
	extern float* sTremoroDef;
	extern float* sTreTable;
	extern float* sAdsrDef;
	extern float* CUTOFF_TO_IIR_TABLE;
	extern float* sV0;
	extern float* sV1;

	float pitchToCent(float, float);
	unsigned short extend8to16(unsigned char);
	int getRandomS32();
} // namespace Player

} // namespace JASystem

#endif // JASPLAYER_IMPL_HPP
