#ifndef J3D_TEVS_HPP
#define J3D_TEVS_HPP

#include <dolphin/gx.h>

void J3DLoadCPCmd(u8, u32);

void J3DLoadArrayBasePtr(_GXAttr, void*);
void J3DSetVtxAttrFmtv(_GXVtxFmt, _GXVtxAttrFmtList*, bool);

struct J3DLightObj {
  void load(u32) const;
};

struct J3DTexMtx {
  void calc();
  void load(u32) const;
};

void J3DGDSetTexLookupMode(_GXTexMapID, _GXTexWrapMode, _GXTexWrapMode, _GXTexFilter, _GXTexFilter, float, float, float, u8, u8, _GXAnisotropy);

struct J3DTevOrder {
  typedef void IDK;
  IDK load(u32) const;
};

struct J3DIndTexOrder {
  typedef void IDK;
  IDK load(u32) const;
};

struct J3DIndTexCoordScale {
  typedef void IDK;
  IDK load(unsigned long) const;
};

#endif
