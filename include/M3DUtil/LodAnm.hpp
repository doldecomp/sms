#ifndef M3DUTIL_LOD_ANM_HPP
#define M3DUTIL_LOD_ANM_HPP

class TLiveActor;

struct TLodAnmIndex { };

class TLodAnm {
public:
	void execChangeLod();
	void setBckAndBtpAnm(int);
	void setBtpAnm_(int);
	void setBckAnm_(int);
	TLodAnm(TLiveActor*, const TLodAnmIndex*, int, float);
};

#endif
