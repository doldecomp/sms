#include <dolphin/amcstubs/AmcExi2Stubs.h>

void EXI2_Init(volatile u8**, AmcEXICallback) { }

void EXI2_EnableInterrupts() { }

u32 EXI2_Poll() { return 0; }

int EXI2_ReadN(void*, u32) { return AMC_EXI_NO_ERROR; }

int EXI2_WriteN(const void*, u32) { return AMC_EXI_NO_ERROR; }

void EXI2_Reserve() { }

void EXI2_Unreserve() { }

int AMC_IsStub() { return 1; }
