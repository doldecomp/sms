#include <JSystem/JUtility/JUTException.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/JUtility/JUTDirectFile.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <dolphin/base/PPCArch.h>
#include <macros.h>



struct CallbackObject {
	/* 0x00 */ OSErrorHandler callback;
	/* 0x04 */ u16 error;
	/* 0x06 */ u16 pad_0x06;
	/* 0x08 */ OSContext* context;
	/* 0x0C */ int param_3;
	/* 0x10 */ int param_4;
};

void search_name_part(u8*, u8*, int);

const u32 dummy1[4] = { 0, 2, 1, 3 };
const u32 dummy2[3] = { 0x3F800000, 0x3F800000, 0x3F800000 };
const u32 dummy3[3] = { 0x3F800000, 0x3F800000, 0x3F800000 };

OSMessageQueue JUTException::sMessageQueue = {};
const char* JUTException::sCpuExpName[]    = {
    "SYSTEM RESET",
    "MACHINE CHECK",
    "DSI",
    "ISI",
    "EXTERNAL INTERRUPT",
    "ALIGNMENT",
    "PROGRAM",
    "FLOATING POINT",
    "DECREMENTER",
    "SYSTEM CALL",
    "TRACE",
    "PERFORMACE MONITOR",
    "BREAK POINT",
    "SYSTEM INTERRUPT",
    "THERMAL INTERRUPT",
    "PROTECTION",
};
JUTException* JUTException::sErrorManager;
OSErrorHandler JUTException::sPreUserCallback;
OSErrorHandler JUTException::sPostUserCallback;

JUTException* JUTException::create(JUTDirectPrint* directPrint)
{
	if (!sErrorManager) {
		JKRHeap* systemHeap = JKRGetSystemHeap();
		sErrorManager       = new (systemHeap, 0) JUTException(directPrint);
		sErrorManager->resume();
	}

	return sErrorManager;
}

OSMessage JUTException::sMessageBuffer[1] = { 0 };

void* JUTException::run()
{
	OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 1);
	OSMessage message;
	while (true) {
		OSReceiveMessage(&sMessageQueue, &message, OS_MESSAGE_BLOCK);
		CallbackObject* cb      = (CallbackObject*)message;
		OSErrorHandler callback = cb->callback;
		u16 error               = cb->error;
		OSContext* context      = cb->context;
		int r24                 = cb->param_3;
		int r23                 = cb->param_4;
		mFrameMemory
		    = (JUTExternalFB*)sErrorManager->mDirectPrint->getFrameBuffer();
		if (!sErrorManager->mDirectPrint->getFrameBuffer()) {
			sErrorManager->createFB();
		}
		if (callback) {
			callback(error, context, r24, r23);
		}
		OSDisableInterrupts();
		sErrorManager->printContext(error, context, r24, r23);
	}
}

JUTException::JUTException(JUTDirectPrint* directPrint)
    : JKRThread(0x4000, 0x10, 0)
{
	mDirectPrint = directPrint;
	OSSetErrorHandler(OS_EXCEPTION_DSI, (OSErrorHandler)errorHandler);
	OSSetErrorHandler(OS_EXCEPTION_ISI, (OSErrorHandler)errorHandler);
	OSSetErrorHandler(OS_EXCEPTION_PROGRAM, (OSErrorHandler)errorHandler);
	OSSetErrorHandler(OS_EXCEPTION_ALIGNMENT, (OSErrorHandler)errorHandler);
	OSSetErrorHandler(OS_EXCEPTION_MEMORY_PROTECTION,
	                  (OSErrorHandler)errorHandler);

	sPreUserCallback  = nullptr;
	sPostUserCallback = nullptr;

	setGamePad(nullptr);

	mPrintWaitTime0 = 10;
	mPrintWaitTime1 = 10;
	mTraceSuppress  = -1;
	field_0x98      = 0;
	mPrintFlags     = 0xFF;
}

static CallbackObject exCallbackObject;
void* JUTException::sConsoleBuffer;
u32 JUTException::sConsoleBufferSize;
JUTConsole* JUTException::sConsole;
u32 JUTException::msr;
u32 JUTException::fpscr;

void JUTException::errorHandler(OSError error, OSContext* context, u32 param_3,
                                u32 param_4)
{
	msr   = PPCMfmsr();
	fpscr = getFpscr();
	OSFillFPUContext(context);
	OSSetErrorHandler(error, nullptr);
	if (error == OS_ERROR_MEMORY_PROTECTION) {
		OSProtectRange(0, nullptr, 0, 3);
		OSProtectRange(1, nullptr, 0, 3);
		OSProtectRange(2, nullptr, 0, 3);
		OSProtectRange(3, nullptr, 0, 3);
	}

	exCallbackObject.callback = sPreUserCallback;
	exCallbackObject.error    = error;
	exCallbackObject.context  = context;
	exCallbackObject.param_3  = param_3;
	exCallbackObject.param_4  = param_4;

	OSSendMessage(&sMessageQueue, &exCallbackObject, OS_MESSAGE_BLOCK);
	OSEnableScheduler();
	OSYieldThread();
}

void JUTException::showFloatSub(int index, f32 value)
{
	if (isnan(value)) {
		sConsole->print_f("F%02d: Nan      ", index);
	} else if (isinf(value)) {
		if (*((u8*)&value) & 0x80) {
			sConsole->print_f("F%02d:+Inf     ", index);
		} else {
			sConsole->print_f("F%02d:-Inf     ", index);
		}
	} else if (value == 0.0f) {
		sConsole->print_f("F%02d: 0.0      ", index);
	} else {
		sConsole->print_f("F%02d:%+.3E", index, value);
	}
}

void JUTException::showFloat(OSContext* context)
{
	if (!sConsole) {
		return;
	}

	sConsole->print("-------------------------------- FPR\n");
	for (int i = 0; i < 10; i++) {
		showFloatSub(i, context->fpr[i]);
		sConsole->print(" ");
		showFloatSub(i + 11, context->fpr[i + 11]);
		sConsole->print(" ");
		showFloatSub(i + 22, context->fpr[i + 22]);
		sConsole->print("\n");
	}
	showFloatSub(10, context->fpr[10]);
	sConsole->print(" ");
	showFloatSub(21, context->fpr[21]);
	sConsole->print("\n");
}

bool JUTException::searchPartialModule(u32 address, u32* module_id,
                                       u32* section_id, u32* section_offset,
                                       u32* name_offset)
{
	if (!address) {
		return false;
	}

	OSModuleInfo* module = *(OSModuleInfo**)0x800030C8;
	for (; module != nullptr; module = (OSModuleInfo*)module->link.next) {
		OSSectionInfo* section = (OSSectionInfo*)module->sectionInfoOffset;
		for (u32 i = 0; i < module->numSections; section++, i++) {
			if (section->size != 0) {
				u32 addr = section->offset & ~0x01;
				if ((addr <= address) && (address < addr + section->size)) {
					if (module_id)
						*module_id = module->id;
					if (section_id)
						*section_id = i;
					if (section_offset)
						*section_offset = address - addr;
					if (name_offset)
						*name_offset = module->nameOffset;
					return true;
				}
			}
		}
	}

	return false;
}

void search_name_part(u8* src, u8* dst, int dst_length)
{
	for (u8* p = src; *p; p++) {
		if (*p == '\\') {
			src = p;
		}
	}

	if (*src == '\\') {
		src++;
	}

	for (int i = 0; (*src != 0) && (i < dst_length);) {
		if (*src == '.')
			break;
		*dst++ = *src++;
		i++;
	}

	*dst = '\0';
}

void JUTException::showStack(OSContext* context)
{
	if (!sConsole) {
		return;
	}

	u32 i;
	u32* stackPointer;
	sConsole->print("-------------------------------- TRACE\n");
	sConsole->print_f("Address:   BackChain   LR save\n");

	for (i = 0, stackPointer = (u32*)context->gpr[1];
	     (stackPointer != nullptr) && (stackPointer != (u32*)0xFFFFFFFF)
	     && (i++ < 0x10);) {
		if (i > mTraceSuppress) {
			sConsole->print("Suppress trace.\n");
			return;
		}

		sConsole->print_f("%08X:  %08X    %08X\n", stackPointer,
		                  stackPointer[0], stackPointer[1]);
		showMapInfo_subroutine(stackPointer[1], false);
		JUTConsoleManager* manager = JUTConsoleManager::sManager;
		manager->drawDirect(true);
		waitTime(mPrintWaitTime1);
		stackPointer = (u32*)stackPointer[0];
	}
}

void JUTException::showMainInfo(u16 error, OSContext* context, u32 dsisr,
                                u32 dar)
{
	if (!sConsole) {
		return;
	}

	sConsole->print_f("CONTEXT:%08XH  (%s EXCEPTION)\n", context,
	                  sCpuExpName[error]);
	sConsole->print_f("SRR0:   %08XH   SRR1:%08XH\n", context->srr0,
	                  context->srr1);
	sConsole->print_f("DSISR:  %08XH   DAR: %08XH\n", dsisr, dar);
}

void JUTException::showGPR(OSContext* context)
{
	if (!sConsole) {
		return;
	}

	sConsole->print("-------------------------------- GPR\n");
	for (int i = 0; i < 10; i++) {
		sConsole->print_f("R%02d:%08XH  R%02d:%08XH  R%02d:%08XH\n", i,
		                  context->gpr[i], i + 11, context->gpr[i + 11], i + 22,
		                  context->gpr[i + 22]);
	}
	sConsole->print_f("R%02d:%08XH  R%02d:%08XH\n", 10, context->gpr[10], 21,
	                  context->gpr[21]);
}

JSUList<JUTException::JUTExMapFile> JUTException::sMapFileList(false);

bool JUTException::showMapInfo_subroutine(u32 address, bool begin_with_newline)
{
	if ((address < 0x80000000) || (0x82ffffff < address)) {
		return false;
	}

	const char* new_line = "\n";
	if (begin_with_newline == false) {
		new_line = "";
	}

	u32 name_offset;
	u32 module_id;
	u32 section_id;
	u32 section_offset;
	bool result = searchPartialModule(address, &module_id, &section_id,
	                                  &section_offset, &name_offset);
	u8 name_part[36];
	if (result == true) {
		search_name_part((u8*)name_offset, name_part, 32);
		sConsole->print_f("%s %s:%x section:%d\n", new_line, name_part,
		                  section_offset, section_id);
		begin_with_newline = false;
	}

	if (sMapFileList.getFirst() != sMapFileList.getEnd()) {
		u32 out_addr;
		u32 out_size;
		char out_line[256];

		if (result == true) {
			result = queryMapAddress((char*)name_part, section_offset,
			                         section_id, &out_addr, &out_size, out_line,
			                         ARRAY_COUNT(out_line), true,
			                         begin_with_newline);
		} else {
			result = queryMapAddress(nullptr, address, -1, &out_addr, &out_size,
			                         out_line, ARRAY_COUNT(out_line), true,
			                         begin_with_newline);
		}

		if (result == true) {
			return true;
		}
	}

	return false;
}

void JUTException::showGPRMap(OSContext* context)
{
	if (!sConsole) {
		return;
	}

	bool found_address_register = false;
	sConsole->print("-------------------------------- GPRMAP\n");

	for (int i = 0; i < 31; i++) {
		u32 address = context->gpr[i];

		if (address >= 0x80000000 && 0x83000000 - 1 >= address) {
			found_address_register = true;

			sConsole->print_f("R%02d: %08XH", i, address);
			if (!showMapInfo_subroutine(address, true)) {
				sConsole->print("  no information\n");
			}
			JUTConsoleManager::sManager->drawDirect(true);
			waitTime(mPrintWaitTime1);
		}
	}

	if (!found_address_register) {
		sConsole->print("  no register which seem to address.\n");
	}
}

void JUTException::printDebugInfo(JUTException::EInfoPage page, OSError error,
                                  OSContext* context, u32 param_3, u32 param_4)
{
	switch (page) {
	case EINFO_PAGE_GPR:
		return showGPR(context);
	case EINFO_PAGE_FLOAT:
		showFloat(context);
		if (sConsole) {
			sConsole->print_f(" MSR:%08XH\t FPSCR:%08XH\n", msr, fpscr);
		}
		break;
	case EINFO_PAGE_STACK:
		return showStack(context);
	case EINFO_PAGE_GPR_MAP:
		return showGPRMap(context);
	}
}

bool JUTException::isEnablePad() const
{
	if (mGamePad == (JUTGamePad*)0xFFFFFFFF)
		return true;

	if (mGamePadPort >= 0)
		return true;

	return mGamePad ? true : false;
}

bool JUTException::readPad(u32* out_trigger, u32* out_button)
{
	bool result       = false;
	OSTime start_time = OSGetTime();
	OSTime ms;
	do {
		OSTime ticks = OSGetTime() - start_time;
		ms           = ticks / (OS_TIMER_CLOCK / 1000);
	} while (ms < 0x32);

	if (mGamePad == (JUTGamePad*)0xffffffff) {
		JUTGamePad gamePad0(JUTGamePad::EPort1);
		JUTGamePad gamePad1(JUTGamePad::EPort2);
		JUTGamePad gamePad2(JUTGamePad::EPort3);
		JUTGamePad gamePad3(JUTGamePad::EPort4);
		JUTGamePad::read();

		if (out_trigger) {
			*out_trigger = gamePad0.getTrigger() | gamePad1.getTrigger()
			               | gamePad2.getTrigger() | gamePad3.getTrigger();
		}
		if (out_button) {
			*out_button = gamePad0.getButton() | gamePad1.getButton()
			              | gamePad2.getButton() | gamePad3.getButton();
		}

		result = true;
	} else if (mGamePadPort >= 0) {
		JUTGamePad gamePad(mGamePadPort);

		JUTGamePad::read();
		if (out_trigger) {
			*out_trigger = gamePad.getTrigger();
		}
		if (out_button) {
			*out_button = gamePad.getButton();
		}

		result = true;
	} else if (mGamePad) {
		JUTGamePad::read();
		if (out_trigger) {
			*out_trigger = mGamePad->getTrigger();
		}
		if (out_button) {
			*out_button = mGamePad->getButton();
		}

		result = true;
	}

	return result;
}

void JUTException::printContext(OSError error, OSContext* context, u32 dsisr,
                                u32 dar)
{
	bool is_pad_enabled = isEnablePad() ? false : true;
	if (!sErrorManager->mDirectPrint->isActive()) {
		return;
	}

	if (!sConsole) {
		return;
	}

	sConsole->print_f(
	    "******** EXCEPTION OCCURRED! ********\nFrameMemory:%XH\n",
	    getFrameMemory());

	int post_callback_executed = false;
	while (true) {
		showMainInfo(error, context, dsisr, dar);

		JUTConsoleManager::sManager->drawDirect(true);
		waitTime(mPrintWaitTime0);

		if ((mPrintFlags & JUT_PRINT_GPR) != 0) {
			printDebugInfo(EINFO_PAGE_GPR, error, context, dsisr, dar);
			JUTConsoleManager::sManager->drawDirect(true);
			waitTime(mPrintWaitTime0);
		}
		if ((mPrintFlags & JUT_PRINT_GPR_MAP) != 0) {
			printDebugInfo(EINFO_PAGE_GPR_MAP, error, context, dsisr, dar);
			JUTConsoleManager::sManager->drawDirect(true);
			waitTime(mPrintWaitTime0);
		}
		if ((mPrintFlags & JUT_PRINT_FLOAT) != 0) {
			printDebugInfo(EINFO_PAGE_FLOAT, error, context, dsisr, dar);
			JUTConsoleManager::sManager->drawDirect(true);
			waitTime(mPrintWaitTime0);
		}
		if ((mPrintFlags & JUT_PRINT_STACK) != 0) {
			printDebugInfo(EINFO_PAGE_STACK, error, context, dsisr, dar);
			JUTConsoleManager::sManager->drawDirect(true);
			waitTime(mPrintWaitTime1);
		}

		sConsole->print("--------------------------------\n");
		JUTConsoleManager::sManager->drawDirect(true);

		if (post_callback_executed == 0 && sPostUserCallback) {
			BOOL enable            = OSEnableInterrupts();
			post_callback_executed = true;
			(*sPostUserCallback)(error, context, dsisr, dar);
			OSRestoreInterrupts(enable);
		}

		if (field_0x98 == 0 || !is_pad_enabled) {
			break;
		}

		sConsole->setOutput(sConsole->getOutput() & 1);
	}

	if (!is_pad_enabled) {
		OSEnableInterrupts();

		int down = 0;
		int up   = 0;
		do {
			u32 button;
			u32 trigger;
			readPad(&trigger, &button);

			bool draw = false;
			if (trigger == 0x100) {
				sConsole->scrollToLastLine();
				draw = true;
			}

			if (trigger == 0x200) {
				sConsole->scrollToFirstLine();
				draw = true;
			}

			if (button == 8) {
				sConsole->scroll(
				    (down < 3) ? -1
				               : ((down < 5) ? -2 : ((down < 7) ? -4 : -8)));
				draw = true;
				up   = 0;
				down++;
			} else if (button == 4) {
				sConsole->scroll(
				    (up < 3) ? 1 : ((up < 5) ? 2 : ((up < 7) ? 4 : 8)));
				draw = true;
				down = 0;
				up++;
			} else {
				down = 0;
				up   = 0;
			}

			if (draw == true) {
				u32 start = VIGetRetraceCount();
				while (start == VIGetRetraceCount())
					;
				JUTConsoleManager::sManager->drawDirect(true);
			}

			waitTime(30);
		} while (true);
	}

	while (true) {
		sConsole->scrollToFirstLine();
		JUTConsoleManager::sManager->drawDirect(true);
		waitTime(2000);

		int line_offset;
		int used_line;
		u32 height;
	next:
		for (u32 i = sConsole->getHeight(); i > 0; i--) {
			sConsole->scroll(1);
			JUTConsoleManager::sManager->drawDirect(true);

			height              = sConsole->getHeight();
			JUTConsole* console = sConsole;
			line_offset         = console->getLineOffset();
			used_line           = console->getUsedLine();
			if ((used_line - height) + 1U <= line_offset)
				break;
			waitTime(20);
		}

		waitTime(3000);
		height              = sConsole->getHeight();
		JUTConsole* console = sConsole;
		line_offset         = console->getLineOffset();
		used_line           = console->getUsedLine();
		if ((used_line - height) + 1U <= line_offset) {
			continue;
		}
		goto next;
	}
}

void JUTException::waitTime(s32 timeout_ms)
{
	if (!timeout_ms)
		return;

	OSTime start_time = OSGetTime();
	OSTime ms;
	do {
		OSTime ticks = OSGetTime() - start_time;
		ms           = ticks / (OS_TIMER_CLOCK / 1000);
	} while (ms < timeout_ms);
}

void JUTException::createFB()
{
	GXRenderModeObj* renderMode = &GXNtsc480Int;

	void* end  = OSGetArenaHi();
	u16 width  = ALIGN_NEXT(renderMode->fbWidth, 16);
	u16 height = renderMode->xfbHeight;
	u32 size   = width * height * 2;

	void* begin  = (void*)ALIGN_PREV((u32)end - size, 32);
	void* object = (void*)ALIGN_PREV((s32)begin - sizeof(JUTExternalFB), 32);
	new (object) JUTExternalFB(renderMode, GX_GM_1_7, begin, size);

	mDirectPrint->changeFrameBuffer(object);
	VIConfigure(renderMode);
	VISetNextFrameBuffer(begin);
	VISetBlack(FALSE);
	VIFlush();

	mFrameMemory = (JUTExternalFB*)object;
}

// TODO: this looks very asm-y but peephole is still on after
// it so maybe not? IDK!
asm u32 JUTException::getFpscr() {
#ifdef __MWERKS__ // clang-format off
  stwu  r1, -0x10(r1)

  // Enable the "floating-point available" flag just in case?
  mfmsr r5
  ori   r5, r5, 0x2000
  mtmsr r5

  isync

  // Get contents of the "floating point status and control register"
  mffs f1
  stfd f1, 0x8(r1)
  lwz  r3, 0xc(r1)

  addi r1, r1, 0x10
#endif // clang-format on
}

#pragma peephole on

OSErrorHandler JUTException::setPreUserCallback(OSErrorHandler callback)
{
	OSErrorHandler previous = sPreUserCallback;
	sPreUserCallback        = callback;
	return previous;
}

void JUTException::appendMapFile(char* path)
{
	if (!path) {
		return;
	}

	JSUListIterator<JUTExMapFile> iterator;
	for (iterator = sMapFileList.getFirst(); iterator != sMapFileList.getEnd();
	     ++iterator) {
		if (strcmp(path, iterator->mPath) == 0) {
			return;
		}
	}

	JUTExMapFile* mapFile = new JUTExMapFile(path);
	sMapFileList.append(&mapFile->mLink);
}

bool JUTException::queryMapAddress(char* mapPath, u32 address, s32 section_id,
                                   u32* out_addr, u32* out_size, char* out_line,
                                   u32 line_length, bool print,
                                   bool begin_with_newline)
{
	if (mapPath) {
		char buffer[80];
		strcpy(buffer, mapPath);
		strcat(buffer, ".map");
		if (queryMapAddress_single(buffer, address, section_id, out_addr,
		                           out_size, out_line, line_length, print,
		                           begin_with_newline)
		    == true) {
			return true;
		}
	} else if (sMapFileList.getFirst() != sMapFileList.getEnd()) {
		if (queryMapAddress_single(sMapFileList.getFirst()->getObject()->mPath,
		                           address, -1, out_addr, out_size, out_line,
		                           line_length, print, begin_with_newline)
		    == true) {
			return true;
		}
	}

	return false;
}

bool JUTException::queryMapAddress_single(char* mapPath, u32 address,
                                          s32 section_id, u32* out_addr,
                                          u32* out_size, char* out_line,
                                          u32 line_length, bool print,
                                          bool begin_with_newline)
{
	if (!mapPath) {
		return false;
	}

	char section_name[16];
	JUTDirectFile file;
	int section_idx = 0;
	if (!file.fopen(mapPath)) {
		return false;
	}

	bool result = false;
	bool found_section;

	while (true) {
		section_idx++;
		found_section = false;
		char buffer[0x200];

		while (true) {
			char* src;
			char* dst;

			if (file.fgets(buffer, ARRAY_COUNT(buffer)) < 0)
				break;
			if (buffer[0] != '.')
				continue;

			int i = 0;
			src   = buffer + 1;
			while (*src != '\0') {
				section_name[i] = *src;
				if (*src == ' ' || i == 0xf)
					break;
				i++;
				src++;
			}

			section_name[i] = 0;
			if (*src == 0)
				break;

			if (src[1] == 's' && src[2] == 'e' && src[3] == 'c'
			    && src[4] == 't') {
				found_section = true;
				break;
			}
		}

		if (!found_section)
			break;

		if (section_id >= 0 && section_id != section_idx)
			continue;

		int length;

		while (true) {
			if ((length = file.fgets(buffer, ARRAY_COUNT(buffer))) <= 4)
				break;
			if ((length < 28))
				continue;
			if (buffer[28] == '4') {
				u32 addr = ((buffer[18] - '0') << 28)
				           | strtol(buffer + 19, nullptr, 16);
				int size = strtol(buffer + 11, nullptr, 16);
				if ((addr <= address && address < addr + size)) {
					if (out_addr)
						*out_addr = addr;

					if (out_size)
						*out_size = size;

					if (out_line) {
						const u8* src = (const u8*)&buffer[0x1e];
						u8* dst       = (u8*)out_line;
						u32 i         = 0;

						for (i = 0; i < line_length - 1; ++src) {
							if ((u32)(*src) < ' ' && (u32)*src != '\t')
								break;
							if ((*src == ' ' || (u32)*src == '\t')
							    && (i != 0)) {
								if (dst[-1] != ' ') {
									*dst = ' ';
									dst++;
									++i;
								}
							} else {
								*dst++ = *src;
								i++;
							}
						}
						if (i != 0 && dst[-1] == ' ') {
							dst--;
							i--;
						}
						*dst = 0;
						if (print) {
							if (begin_with_newline) {
								sConsole->print("\n");
							}
							sConsole->print_f(
							    "  [%08X]: .%s [%08X: %XH]\n  %s\n", address,
							    section_name, addr, size, out_line);
							begin_with_newline = false;
						}
					}
					result = true;
					break;
				}
			}
		}

		if ((section_id < 0 || section_id != section_idx)) {
			continue;
		}
		if (print && begin_with_newline) {
			sConsole->print("\n");
		}
		break;
	}

	file.fclose();
	return result ? true : false;
}

void JUTException::createConsole(void* console_buffer, u32 console_buffer_size)
{
	if (!console_buffer || !console_buffer_size) {
		return;
	}

	u32 lines = JUTConsole::getLineFromObjectSize(console_buffer_size, 0x32);
	if (lines != 0) {
		sConsoleBuffer     = console_buffer;
		sConsoleBufferSize = console_buffer_size;
		sConsole = JUTConsole::create(0x32, sConsoleBuffer, sConsoleBufferSize);

		JUTConsoleManager* manager = JUTConsoleManager::sManager;
		manager->setDirectConsole(sConsole);

		sConsole->setFontSize(10.0, 6.0);
		sConsole->setPosition(15, 26);
		sConsole->setHeight(23);
		sConsole->setVisible(true);
		sConsole->setOutput(JUTConsole::OUTPUT_OSREPORT
		                    | JUTConsole::OUTPUT_CONSOLE);
	}
}

JUTExternalFB::JUTExternalFB(GXRenderModeObj* renderMode, GXGamma gamma,
                             void* buffer, u32 size)
{
	mRenderMode = renderMode;
	mSize       = size;
	field_0x0C  = 1;
	mGamma      = gamma;
	field_0x10  = false;
}
