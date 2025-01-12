#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>

namespace JUTAssertion {

namespace {

	static u32 sMessageLife;

	static char sMessageFileLine[64];

	static char sMessageString[96];

	static bool mVisible = true;
}; // namespace

inline u32 flush_subroutine()
{
	if (sMessageLife == 0) {
		return 0;
	}

	if (sMessageLife != -1) {
		sMessageLife--;
	}

	if (sMessageLife < 5) {
		return 0;
	}

	return sMessageLife;
}

void flushMessage()
{
	if (flush_subroutine() && mVisible == true) {
		JUTDirectPrint::getManager()->drawString(16, 16, sMessageFileLine);
		JUTDirectPrint::getManager()->drawString(16, 24, sMessageString);
	}
}

}; // namespace JUTAssertion
