#ifdef __cplusplus
extern "C" {
#endif

__declspec(section ".init") extern void __init_hardware(void);
__declspec(section ".init") extern void
__flush_cache(register void* address, register unsigned int size);

#ifdef __cplusplus
}
#endif
