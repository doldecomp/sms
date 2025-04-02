#ifndef M3DUTIL_LOD_ANM_HPP
#define M3DUTIL_LOD_ANM_HPP

class TLiveActor;

struct TLodAnmIndex { };

class TLodAnm {
public:
	TLodAnm(TLiveActor*, const TLodAnmIndex*, int, float);
	void execChangeLod();
	void setBckAndBtpAnm(int);
	void setBtpAnm_(int);
	void setBckAnm_(int);

public:
	/* 0x0 */ char unk0[0x20];
};

#endif
