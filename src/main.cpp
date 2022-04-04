#include "dolphin.h"
#include "types.h"

// BSS
TApplication gpApplication;

void main(void)
{
	gpApplication.initialize();
	gpApplication.proc();
	gpApplication.finalize();
}
