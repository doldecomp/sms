#ifndef CAMERA_CUBE_MANAGER_BASE_HPP
#define CAMERA_CUBE_MANAGER_BASE_HPP

#include <Camera/CubeMapTool.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/std-vector.hpp>
#include <dolphin/mtx.h>

// TODO: probably shouldn't be here
template <class T, class U = JDrama::TNameRef>
class TNameRefPtrAryT : public U, public JGadget::TVector_pointer<T> {
public:
	TNameRefPtrAryT()
	    : U("<NameRefPtrAryT>")
	{
	}
	~TNameRefPtrAryT();
	virtual void load(JSUMemoryInputStream&) { }
	virtual void loadAfter() { }
	virtual JDrama::TNameRef* searchF(u16 key, char const* name) { }
};

class TCubeManagerBase;

extern TCubeManagerBase* gpCubeStream;

class TCubeManagerBase : public JDrama::TViewObj {
public:
	TCubeManagerBase(const char*, const char*);
	TCubeManagerBase(const char*, u8);

	// fabricated
	void initializer();

	virtual ~TCubeManagerBase() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	s32 getDataNo(s32) const;
	u32 getInCubeNo(const Vec&) const;
	bool isInCube(const Vec&, s32) const;
	bool isInCube(const Vec&, const char*) const;
	void calcPointInCubeRatio(const Vec&, s32, float*, float*, float*) const;

public:
	/* 0x10 */ u8 unk10;
	/* 0x14 */ TNameRefPtrAryT<TCubeGeneralInfo>* unk14;
	/* 0x18 */ const char* unk18;
};

class TCubeManagerArea;

extern TCubeManagerArea* gpCubeArea;

class TCubeManagerArea : public TCubeManagerBase {
public:
	TCubeManagerArea();

	bool isInAreaCube(const Vec&) const;
};

class TCubeManagerFast;

extern TCubeManagerFast* gpCubeFastA;
extern TCubeManagerFast* gpCubeFastB;
extern TCubeManagerFast* gpCubeFastC;

class TCubeManagerFast : public TCubeManagerBase {
public:
	TCubeManagerFast();

	bool isInOtherCube(const Vec&) const;

public:
	/* 0x1C */ u32 unk1C;
};

bool SMS_IsInOtherFastCube(const Vec&);
bool SMS_IsInSameCameraCube(const Vec&);

extern TCubeManagerBase* gpCubeMirror;

#endif
