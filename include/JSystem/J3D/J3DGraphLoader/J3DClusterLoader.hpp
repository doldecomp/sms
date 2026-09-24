#ifndef J3D_CLUSTER_LOADER_HPP
#define J3D_CLUSTER_LOADER_HPP

#include <types.h>
#include <JSystem/JUtility/JUTDataHeader.hpp>

class J3DDeformData;

// fabricated: the whole TU is dead-stripped, so the only evidence for this
// block's field order is the sequence of JSUConvertOffsetToPtr<T>
// instantiations readCluster() leaves behind (see the .cpp).
struct J3DClusterBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mClusterNum;
	/* 0x0A */ u16 mClusterKeyNum;
	/* 0x0C */ u16 mClusterVertexNum;
	/* 0x0E */ u16 mVtxPosNum;
	/* 0x10 */ u16 mVtxNrmNum;
	/* 0x14 */ PTR32(void) mpClusterName;
	/* 0x18 */ PTR32(void) mpClusterKeyName;
	/* 0x1C */ PTR32(void) mpCluster;
	/* 0x20 */ PTR32(void) mpClusterKey;
	/* 0x24 */ PTR32(void) mpClusterVertex;
	/* 0x28 */ PTR32(void) mpVtxPos;
	/* 0x2C */ PTR32(void) mpVtxNrm;
}; // Size: 0x30

class J3DClusterLoaderDataBase {
public:
	static J3DDeformData* load(const void*);
};

class J3DClusterLoader {
public:
	virtual J3DDeformData* load(const void*) = 0;
	virtual ~J3DClusterLoader() { }
};

class J3DClusterLoader_v15 : public J3DClusterLoader {
public:
	J3DClusterLoader_v15();
	virtual ~J3DClusterLoader_v15();
	virtual J3DDeformData* load(const void*);
	void readCluster(const J3DClusterBlock*);

	/* 0x04 */ J3DDeformData* mDeformData;
};

#endif
