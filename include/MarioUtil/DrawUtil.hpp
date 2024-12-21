#ifndef DRAW_UTIL_HPP
#define DRAW_UTIL_HPP

#include <dolphin/types.h>
#include <JSystem/JDRGraphics.hpp>
#include <JSystem/JSUMemoryStream.hpp>
#include <JSystem/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>


class TSilhouette : public JDrama::TNameRef
{
public:
  typedef void IDK;
  IDK calcSilhouetteBorder();
  virtual IDK perform(u32, JDrama::TGraphics*);
  IDK setting(float(*)[4]);
  virtual void loadAfter();
  virtual void load(JSUMemoryInputStream&);

  virtual ~TSilhouette() {};
};

class J3DModel;

class TTrembleModelEffect
{
public:
  typedef void IDK;
  IDK reset();
  IDK movement();
  IDK clash(float);
  IDK tremble(float, float, float, int);
  IDK init(J3DModel*);
};

class J3DMaterial;
class J3DModelData;
class J3DShape;

typedef void IDK;
IDK SMS_UnifyMaterial(J3DModel*);
IDK SMS_CalcMatAnmAndMakeDL(J3DModel*, u16);
IDK SMS_ShowJoint(J3DMaterial*, bool);
IDK SMS_DrawInit();
IDK SMS_MakeDLAndLock(J3DModel*);
IDK SMS_DrawShape(J3DModelData*, u16);
IDK SMS_SettingDrawShape(J3DModelData*, u16);
IDK SMS_DrawCube(const JGeometry::TVec3<float>&, const JGeometry::TVec3<float>&);
IDK SMS_CountPolygonNumInShape(J3DShape*);
IDK SMS_ResetDamageFogEffect(J3DModelData*);
IDK SMS_AddDamageFogEffect(J3DModelData*, const JGeometry::TVec3<float>&, JDrama::TGraphics*);

#endif
