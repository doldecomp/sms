#include "dolphin/types.h"

__declspec(section ".init") extern void __init_hardware(void);
__declspec(section
           ".init") extern void __flush_cache(register void* address,
                                              register unsigned int size);

extern void __init_cpp(void);
