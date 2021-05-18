#ifndef GPMARDIRECTOR_H
#define GPMARDIRECTOR_H

#include "GC2D/GCConsole2.hpp"

extern class  {
    public:
        char pad[0x70];
        int unk70;
        TGCConsole2 *console;
} *gpMarDirector;

#endif
