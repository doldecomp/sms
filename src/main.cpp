#include <System/Application.hpp>

// BSS
TApplication gpApplication;

void main(void)
{
	SMSGetApplication()->initialize();
	SMSGetApplication()->proc();
	SMSGetApplication()->finalize();
}
