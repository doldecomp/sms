#ifndef GPMARDIRECTOR_H
#define GPMARDIRECTOR_H

#include "GC2D/GCConsole2.hpp"

extern class  {
    public:
        char pad_000[0x70];
        int _070;
        TGCConsole2 *console;
} *gpMarDirector;

#endif
