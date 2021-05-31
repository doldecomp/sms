#ifndef GPMARDIRECTOR_H
#define GPMARDIRECTOR_H

#include "GC2D/GCConsole2.hpp"

extern class  {
    public:
        char pad_000[0x70];
        int _070;
        TGCConsole2 *console;
        int _078;
        u8 map;
        u8 _07D;
        u8 _07E;
        u8 _07F;
} *gpMarDirector;

#endif
