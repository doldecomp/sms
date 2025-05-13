#ifndef RENDERMODEOBJ_HPP
#define RENDERMODEOBJ_HPP

#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

void SMSSetupMovieRenderingInfo(JDrama::TDisplay*);
void SMSSetupGameRenderingInfo(JDrama::TDisplay*, bool);
void SMSSetupTitleRenderingInfo(JDrama::TDisplay*);
void SMSSetupTitleRenderMode(GXRenderModeObj*);
void SMSSetupGCLogoRenderingInfo(JDrama::TDisplay*);
void SMSSetupGCLogoRenderMode(GXRenderModeObj*);
JDrama::TRect SMSGetRederRect_Game();

extern u8 SMSAASamplePattern_non[];
extern u8 SMSAASamplePattern_aa[];
extern u8 SMSVFilter_non[];
extern u8 SMSVFilter_flicker[];

#endif // RENDERMODEOBJ_HPP
