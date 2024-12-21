#ifndef J3D_SHAPE_HPP
#define J3D_SHAPE_HPP

#include <dolphin/types.h>
#include <dolphin/vec.h>

class J3DShape {
public:
	typedef void IDK;
	IDK initialize();
	~J3DShape();
	IDK calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);
	IDK countBumpMtxNum() const;
	IDK makeVtxArrayCmd();
	IDK makeVcdVatCmd();
	IDK loadVtxArray() const;
	IDK draw() const;

	char unk0[0x50];
	u32 unk50;
	u32 unk54;
	void* unk58;
};

class J3DShapeMtx {
public:
	typedef void IDK;
	virtual IDK getUseMtxIndex(u16) const;
	IDK loadMtxIndx_PNGP(int, u16) const;
	IDK loadMtxIndx_PCPU(int, u16) const;
	IDK loadMtxIndx_NCPU(int, u16) const;
	IDK loadMtxIndx_PNCPU(int, u16) const;
	virtual IDK load() const;
	virtual IDK calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);
	virtual ~J3DShapeMtx();
	virtual IDK getUseMtxNum() const;
	virtual IDK getType() const;

	static IDK* mtxLoadPipeline;
	static IDK* currentPipeline;
};

#endif
