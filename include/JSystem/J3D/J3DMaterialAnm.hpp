#ifndef J3D_MATERIAL_ANM
#define J3D_MATERIAL_ANM

struct J3DMaterial;

class J3DMaterialAnm // size 0x6c
{
public:
	J3DMaterialAnm() { initialize(); }

	virtual ~J3DMaterialAnm() { }
	virtual void calc(J3DMaterial*) const;

	void initialize();

	char padding[0x68];
};

#endif
