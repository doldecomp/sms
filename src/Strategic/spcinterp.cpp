#include <Strategic/spcinterp.hpp>
#include <macros.h>

void spcYield(TSpcInterp*, u32) { }

void spcExit(TSpcInterp*, u32) { }

void spcLock(TSpcInterp*, u32) { }

void spcUnlock(TSpcInterp*, u32) { }

void spcPrint(TSpcInterp*, u32) { }

void spcDump(TSpcInterp*, u32) { }

void spcInt(TSpcInterp*, u32) { }

void spcFloat(TSpcInterp*, u32) { }

void spcTypeof(TSpcInterp*, u32) { }

TSpcBinary::TSpcBinary(void* data)
    : mData((u8*)data)
{
}

TSpcBinary::~TSpcBinary() { }

void TSpcBinary::init()
{
	calcAndStoreKeys();
	initSystemBuiltin();
	initUserBuiltin();
	SpcTrace("TSpcBinary : system initialized\n");
}

u32 TSpcBinary::calcKey(const char* name)
{
	u32 result = 0;
	for (const char* p = name; *p != '\0'; ++p)
		result = *p + result * 3;
	return result;
}

void TSpcBinary::calcAndStoreKeys()
{
	for (int i = 0; i < getHeader()->mSymbolNum; ++i) {
		TSpcSymbol* symbol = getSymbol(i);
		symbol->mNameHash  = calcKey(getSymbolName(symbol));
	}
}

void TSpcBinary::initSystemBuiltin()
{
	bindSystemDataToSymbol("yield", (u32)&spcYield);
	bindSystemDataToSymbol("exit", (u32)&spcExit);
	bindSystemDataToSymbol("lock", (u32)&spcLock);
	bindSystemDataToSymbol("unlock", (u32)&spcUnlock);
	bindSystemDataToSymbol("print", (u32)&spcPrint);
	bindSystemDataToSymbol("dump", (u32)&spcDump);
	bindSystemDataToSymbol("int", (u32)&spcInt);
	bindSystemDataToSymbol("float", (u32)&spcFloat);
	bindSystemDataToSymbol("typeof", (u32)&spcTypeof);
	SpcTrace("TSpcBinary : system symbol installed\n");
}

void TSpcBinary::initUserBuiltin() { }

TSpcSymbol* TSpcBinary::searchSymbol(const char* name)
{
	u32 wantHash = calcKey(name);

	for (int i = 0; i < getHeader()->mSymbolNum; ++i) {
		if (wantHash == getSymbol(i)->mNameHash) {
			TSpcSymbol* candidate = getSymbol(i);
			const char* candName  = getSymbolName(candidate);
			if (strcmp(name, candName) == 0) {
				return candidate;
			}
		}
	}

	return nullptr;
}

void TSpcBinary::bindSystemDataToSymbol(const char* name, u32 param_2)
{
	TSpcSymbol* symbol = searchSymbol(name);
	if (symbol == nullptr) {
		SpcTrace("TSpcBinary : unknown symbol %s\n", name);
	} else {
		symbol->mNativeCall = param_2;
	}
}

void TSpcInterp::execint() { }

void TSpcInterp::execflt() { }

void TSpcInterp::execstr() { }

void TSpcInterp::execadr() { }

void TSpcInterp::execvar() { }

void TSpcInterp::execnop() { }

void TSpcInterp::execinc() { }

void TSpcInterp::execdec() { }

void TSpcInterp::execadd() { }

void TSpcInterp::execsub() { }

void TSpcInterp::execmul() { }

void TSpcInterp::execdiv() { }

void TSpcInterp::execmod() { }

void TSpcInterp::execass() { }

void TSpcInterp::execeq() { }

void TSpcInterp::execne() { }

void TSpcInterp::execgt() { }

void TSpcInterp::execlt() { }

void TSpcInterp::execge() { }

void TSpcInterp::execle() { }

void TSpcInterp::execneg() { }

void TSpcInterp::execnot() { }

void TSpcInterp::execand() { }

void TSpcInterp::execor() { }

void TSpcInterp::execband() { }

void TSpcInterp::execbor() { }

void TSpcInterp::execshl() { }

void TSpcInterp::execshr() { }

void TSpcInterp::execcall() { }

void TSpcInterp::execfunc() { }

void TSpcInterp::execmkfr() { }

void TSpcInterp::execmkds() { }

void TSpcInterp::execret() { }

void TSpcInterp::execret0() { }

void TSpcInterp::execjne() { }

void TSpcInterp::execjmp() { }

void TSpcInterp::execpop() { }

void TSpcInterp::execint0() { }

void TSpcInterp::execint1() { }

void TSpcInterp::execend() { }

void TSpcInterp::chooseExecFunction(unsigned char) { }

void TSpcInterp::dispatchBuiltinDefault(u32, u32) { }

void TSpcInterp::dispatchBuiltin(u32, u32) { }

TSpcInterp::TSpcInterp(TSpcBinary* param_1, void* param_2, int param_3,
                       int param_4, int param_5, int param_6)
    : mBinary(param_1)
    , unk4(param_3)
    , unk8(0)
    , unkC(param_3)
    , unk10(param_2)
    , mProcessStack(param_4)
    , mStorageStack(param_5)
    , mContextStack(param_6)
{
	if (mBinary == nullptr || mBinary->getHeader() == nullptr) {
		SpcTrace("TSpcInterp : null binary\n");
		return;
	}

	if (mBinary->getHeader()->mMagic[0] != 'S'
	    || mBinary->getHeader()->mMagic[1] != 'P'
	    || mBinary->getHeader()->mMagic[2] != 'C'
	    || mBinary->getHeader()->mMagic[3] != 'B') {
		SpcTrace("TSpcInterp : invalid magic\n");
	}

	for (int i = 0; i < 4; ++i)
		mDisplay[i] = 0;

	for (int i = 0; i < mBinary->getHeader()->unk18; ++i) {
		mStorageStack.push(TSpcSlice());
	}
}

TSpcInterp::~TSpcInterp() { }

void TSpcInterp::dump()
{
	SpcTrace("TSpcInterp : internal status\n");
	SpcTrace("textofs = %u\n", mBinary->getHeader()->mTextOffset);
	SpcTrace("dataofs = %u\n", mBinary->getHeader()->mDataOffset);
	SpcTrace("datanum = %u\n", mBinary->getHeader()->mDataNum);
	SpcTrace("symofs  = %u\n", mBinary->getHeader()->mSymbolOffset);
	SpcTrace("symnum  = %u\n", mBinary->getHeader()->mSymbolNum);

	for (int i = 0; i < 4; ++i)
		SpcTrace("mDisplay[%d] = %d\n", i, mDisplay[i]);

	for (int i = 0; i < mProcessStack.size(); ++i)
		SpcTrace("mProcessStack[%d] = %d\n", i,
		         mProcessStack[mProcessStack.size() - 1 - i].getDataInt());

	for (int i = 0; i < mContextStack.size(); ++i)
		SpcTrace("mContextStack[%d] = %d\n", i,
		         mContextStack[mContextStack.size() - 1 - i]);

	for (int i = 0; i < mStorageStack.size(); ++i)
		SpcTrace("mStorageStack[%d] = %d\n", i,
		         mStorageStack[mStorageStack.size() - 1 - i].getDataInt());

	for (int i = 0; i < mBinary->getHeader()->mSymbolNum; ++i) {
		TSpcSymbol* symbol = mBinary->getSymbol(i);
		SpcTrace("symbol [%03u] %s : type = %u, ofs = %u, data = %u\n", i,
		         mBinary->getSymbolName(symbol), symbol->mType,
		         symbol->mNameOffset, symbol->mData);
	}
}

void TSpcInterp::verifyArgNum(u32 param_1, u32* param_2)
{
	if (param_1 == *param_2)
		return;

	SpcTrace("TSpcInterp : argument number mismatch (%s, %d -> %d)\n", unk58,
	         *param_2, param_1);

	while (param_1 < *param_2) {
		mProcessStack.pop();
		--*param_2;
	}

	while (param_1 > *param_2) {
		mProcessStack.push(TSpcSlice());
		++*param_2;
	}
}

void TSpcInterp::callByAddress(u32, u32) { }

void TSpcInterp::callByName(const char*, u32) { }

void TSpcInterp::invokeByAddress(u32, u32) { }

void TSpcInterp::invokeByName(const char*, u32) { }

void TSpcInterp::referByIndex(u32) { }

void TSpcInterp::referByName(const char*) { }

void TSpcInterp::update()
{
	// TODO: inline?
	if (mBinary == nullptr || mBinary->getHeader() == nullptr) {
		SpcTrace("TSpcInterp : null binary\n");
		return;
	}

	while (unkC > 0) {
		if (unk14 == 0)
			--unkC;

		// TODO: inline?
		u8 cmd = *((u8*)mBinary->getHeader() + mBinary->getHeader()->mTextOffset
		           + unk8);
		++unk8;

		typedef void (TSpcInterp::*Handler)();
		static Handler handlers[] = {
			&TSpcInterp::execint,  &TSpcInterp::execflt,  &TSpcInterp::execstr,
			&TSpcInterp::execadr,  &TSpcInterp::execvar,  &TSpcInterp::execnop,
			&TSpcInterp::execinc,  &TSpcInterp::execdec,  &TSpcInterp::execadd,
			&TSpcInterp::execsub,  &TSpcInterp::execmul,  &TSpcInterp::execdiv,
			&TSpcInterp::execmod,  &TSpcInterp::execass,  &TSpcInterp::execeq,
			&TSpcInterp::execne,   &TSpcInterp::execgt,   &TSpcInterp::execlt,
			&TSpcInterp::execge,   &TSpcInterp::execle,   &TSpcInterp::execneg,
			&TSpcInterp::execnot,  &TSpcInterp::execand,  &TSpcInterp::execor,
			&TSpcInterp::execband, &TSpcInterp::execbor,  &TSpcInterp::execshl,
			&TSpcInterp::execshr,  &TSpcInterp::execcall, &TSpcInterp::execfunc,
			&TSpcInterp::execmkfr, &TSpcInterp::execmkds, &TSpcInterp::execret,
			&TSpcInterp::execret0, &TSpcInterp::execjne,  &TSpcInterp::execjmp,
			&TSpcInterp::execpop,  &TSpcInterp::execint0, &TSpcInterp::execint1,
			&TSpcInterp::execend,
		};

		Handler handler;
		if (cmd >= ARRAY_COUNT(handlers)) {
			SpcTrace("TSpcInterp : unknown command 0x%x\n", cmd);
			handler = nullptr;
		} else {
			handler = handlers[cmd];
		}

		if (handler)
			(*this.*handler)();
	}
	unkC = unk4;
}
