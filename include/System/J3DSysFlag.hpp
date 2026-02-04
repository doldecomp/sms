#ifndef SYSTEM_J3D_SYS_FLAG_HPP
#define SYSTEM_J3D_SYS_FLAG_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TGXAlphaUpdate : public JDrama::TViewObj {
public:
	TGXAlphaUpdate(const char* name = "<TGXAlphaUpdate>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

class TReInitGX : public JDrama::TViewObj {
public:
	TReInitGX(const char* name = "<TReInitGX>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

class TSMSDrawInit : public JDrama::TViewObj {
public:
	TSMSDrawInit(const char* name = "<SMSDrawInit>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

class TJ3DSysSetViewMtx : public JDrama::TViewObj {
public:
	TJ3DSysSetViewMtx(const char* name = "<J3DSysSetViewMtx>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

class TJ3DSysFlag : public JDrama::TViewObj {
public:
	TJ3DSysFlag(const char* name = "<J3DSysFlag>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

#endif
