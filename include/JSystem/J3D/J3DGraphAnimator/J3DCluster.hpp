#ifndef J3D_CLUSTER_HPP
#define J3D_CLUSTER_HPP

#include <dolphin/types.h>

class J3DModelData;
class J3DModel;
class J3DAnmCluster;

class J3DDeformData {
public:
	void clear();
	J3DDeformData();
	void onAllFlag(u32);
	void offAllFlag(u32);
	void base(J3DModel*);
	void deform(J3DModel*);
	void setAnm(J3DAnmCluster*);
};

class J3DSkinDeform {
public:
	J3DSkinDeform();

	void initMtxIndexArray(J3DModelData*);
	void calcNrmMtx(J3DModel*);

	virtual void deform(J3DModel*);
	virtual ~J3DSkinDeform();
};

#endif
