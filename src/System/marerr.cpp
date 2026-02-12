#include <dolphin/os/OSContext.h>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/JUtility/JUTException.hpp>

static void MarErrException(u16, OSContext*, u32, u32);

namespace {

const u32 cDispExceptionCommand[6] = { 0x100, 0x200, 0x400, 0x800, 0x1, 0x0 };

}

static void MarErrException(u16, OSContext*, u32, u32)
{
	for (int i = 0; cDispExceptionCommand[i] != 0;) {
		JUTException::waitTime(100);
		u32 local_18;
		JUTException::getManager()->readPad(&local_18, nullptr);
		if (local_18 != 0) {
			i = ((local_18 & 0xffff) == cDispExceptionCommand[i]) ? i + 1 : 0;
		}
	}
}

void MarErrInit()
{
	JUTDirectPrint* print = JUTDirectPrint::start();
	JUTConsoleManager::createManager(nullptr);
	JUTException::create(print);
	JUTException::createConsole(new u8[0x1400], 0x1400);
	JUTException::appendMapFile("/mario.MAP");
	JUTException::setPreUserCallback(&MarErrException);
	// fullptr :D
	JUTException::getManager()->setGamePad((JUTGamePad*)0xffffffff);
	JUTException::getManager()->setPrintFlags(8);
}
