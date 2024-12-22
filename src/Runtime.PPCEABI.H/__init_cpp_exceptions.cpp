#include "Runtime.PPCEABI.H/NMWException.h"
#include "Runtime.PPCEABI.H/__ppc_eabi_linker.h"

static int fragmentID = -2;

/**
 * @note Address: 0x800C22C4
 * @note Size: 0x8
 */
asm static char* GetR2()
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mr r3, r2
	blr
#endif // clang-format on
}
extern "C" {
/**
 * @note Address: 0x800C2300
 * @note Size: 0x40
 */
void __init_cpp_exceptions()
{
	if ((s32)fragmentID == -2) {
		char* R2   = GetR2();
		fragmentID = __register_fragment(_eti_init_info, R2);
	}
}

/**
 * @note Address: 0x800C22CC
 * @note Size: 0x34
 */
void __fini_cpp_exceptions()
{
	if ((s32)fragmentID != -2) {
		__unregister_fragment(fragmentID);
		fragmentID = -2;
	}
}
}

__declspec(section ".ctors") extern void* const __init_cpp_exceptions_reference  = __init_cpp_exceptions;
__declspec(section ".dtors") extern void* const __destroy_global_chain_reference = __destroy_global_chain;
__declspec(section ".dtors") extern void* const __fini_cpp_exceptions_reference  = __fini_cpp_exceptions;
