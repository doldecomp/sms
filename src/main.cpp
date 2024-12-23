#include <types.h>
#include <unsorted_types.h>

// BSS
TApplication gpApplication;

void main(void)
{
	gpApplication.initialize();
	gpApplication.proc();
	gpApplication.finalize();
}
