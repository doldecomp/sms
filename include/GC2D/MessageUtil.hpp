#ifndef GC2D_MESSAGE_UTIL_HPP
#define GC2D_MESSAGE_UTIL_HPP

#include <dolphin/types.h>

class J2DTextBox;

const char* SMSGetMessageData(void*, u32);
void SMSMakeTextBuffer(J2DTextBox*, int);

#endif // GC2D_MESSAGE_UTIL_HPP
