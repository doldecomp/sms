#include <System/Application.hpp>

// BSS
TApplication gpApplication;

void main(void)
{
	gpApplication.initialize();
	gpApplication.proc();
	gpApplication.finalize();
}
