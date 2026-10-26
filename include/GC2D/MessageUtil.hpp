#ifndef GC2D_MESSAGE_UTIL_HPP
#define GC2D_MESSAGE_UTIL_HPP

#include <dolphin/types.h>

class J2DTextBox;

const char* SMSGetMessageData(void* messageArchive, u32 messageIndex);
void SMSMakeTextBuffer(J2DTextBox* textbox, int size);

#endif // GC2D_MESSAGE_UTIL_HPP
