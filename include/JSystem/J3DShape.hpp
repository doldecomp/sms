#ifndef J3D_SHAPE_HPP
#define J3D_SHAPE_HPP

#include <dolphin/types.h>
#include <dolphin/vec.h>

class J3DShape {
public:
	typedef void IDK;
	void initialize();
	~J3DShape();
	IDK calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);
	IDK countBumpMtxNum() const;
	IDK makeVtxArrayCmd();
	IDK makeVcdVatCmd();
	IDK loadVtxArray() const;
	IDK draw() const;

	u32 unk0;
	u16 unk4;
	u16 unk6;
	u32 unk8;
	float unkC;
	float unk10;
	float unk14;
	float unk18;
	float unk1C;
	float unk20;
	float unk24;
	void* unk28;
	u32 unk2c;
	u8 unk30;
	char unk31[3];
	u32 unk34;
	u32 unk38;
	char unk3c[8];
	u32 unk44;
	u32 unk48;
	u32 unk4c;
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
	static u32 currentPipeline;
};

#endif
