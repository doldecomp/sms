#ifndef MARIO_UTIL_PACKET_UTIL_HPP
#define MARIO_UTIL_PACKET_UTIL_HPP

#include <dolphin/gx/GXStruct.h>
#include <dolphin/gx/GXEnum.h>

class J3DModel;
class J3DCallBackPacket;
class J3DFogInfo;

void SMS_InitPacket_MatColor(J3DModel*, u16, GXChannelID, const GXColor*);
void SMS_InitPacket_OneTevColor(J3DModel*, u16, GXTevRegID, const GXColorS10*);
void SMS_InitPacket_TwoTevColor(J3DModel*, u16, GXTevRegID, const GXColorS10*,
                                GXTevRegID, const GXColorS10*);
void SMS_InitPacket_ThreeTevColor(J3DModel*, u16, GXTevRegID, const GXColorS10*,
                                  GXTevRegID, const GXColorS10*, GXTevRegID,
                                  const GXColorS10*);
void SMS_InitPacket_CallDL(J3DModel*, u16, u8*, u32);
void SMS_InitPacket_Fog(J3DModel*, u16);
void SMS_InitPacket_OneTevKColor(J3DModel*, u16, GXTevKColorID, const GXColor*);
void SMS_InitPacket_TwoTevKColor(J3DModel*, u16, GXTevKColorID, const GXColor*,
                                 GXTevKColorID, const GXColor*);
void SMS_InitPacket_OneTevKColorAndFog(J3DModel*, u16, GXTevKColorID,
                                       const GXColor*);
void SMS_InitPacket_OneTevColorAndOneTevKColor(J3DModel*, u16, GXTevRegID,
                                               const GXColorS10*,
                                               const GXColor*);
void SMS_InitPacket_TwoTevColorAndOneTevKColor(J3DModel*, u16, GXTevRegID,
                                               const GXColorS10*, GXTevRegID,
                                               const GXColorS10*,
                                               const GXColor*);
void SMS_HideAllShapePacket(J3DModel*);
void SMS_ShowAllShapePacket(J3DModel*);

#endif
