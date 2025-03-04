#ifndef MSOUND_DEBUG_H
#define MSOUND_DEBUG_H

#include <dolphin/types.h>

class MSD {
public:
	static MSD* get();
};

class MSPrintBase {
public:
	MSPrintBase();
	MSPrintBase(u8 param1, u8 param2, u8 param3, u8 param4, u8 param5);
};

class MSPrintCat {
public:
	MSPrintCat();
	void print();
};

class MSPrintEntry {
public:
	MSPrintEntry();
	void print();
};

class MSPrint {
public:
	MSPrint();
	void print();
};

#endif
