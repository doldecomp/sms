#ifndef MARIOMAIN_HPP
#define MARIOMAIN_HPP

#include <dolphin.h>


class TMarioOriginal {
    public:
        char _000[0x7C];
        u32 _07C;
};


// TODO: move to src/Strategic/HitActor.hpp
class THitActor {
    public:
        char _000[0x7C];
        u32 _07C; // status or action?
        char _080[0x58];

        void *_0D8; // wall pointer
        void *_0DC; // ceiling pointer
        void *_0E0; // floor pointer
};


extern class THitActor *gpMarioOriginal;


#endif
