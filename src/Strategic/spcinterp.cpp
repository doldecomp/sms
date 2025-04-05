#include <Strategic/spcinterp.hpp>
#include <macros.h>

void spcYield(TSpcInterp* interp, u32 arg_count)
{
	interp->mStepsLeft = 0;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcExit(TSpcInterp* interp, u32 arg_count)
{
	interp->unk4       = 0;
	interp->mStepsLeft = 0;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcLock(TSpcInterp* interp, u32 arg_count)
{
	interp->mLocked = TRUE;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcUnlock(TSpcInterp* interp, u32 arg_count)
{
	interp->mLocked = FALSE;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcPrint(TSpcInterp* interp, u32 arg_count)
{
	for (int i = 0; i < (int)arg_count; ++i) {
		const TSpcSlice& slice
		    = interp->mProcessStack
		          .mData[interp->mProcessStack.mSize - (arg_count - i)];
		switch (slice.mType) {
		case TSpcSlice::TYPE_INT:
			SpcTrace("%d", slice.getDataInt());
			break;
		case TSpcSlice::TYPE_FLOAT:
			SpcTrace("%f", slice.getDataFloat());
			break;
		case TSpcSlice::TYPE_STRING:
			SpcTrace("%s", slice.getDataString());
			break;
		}
	}

	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcDump(TSpcInterp* interp, u32 arg_count)
{
	interp->dump();

	for (int i = 0; i < (int)arg_count; ++i)
		interp->mProcessStack.pop();
	interp->mProcessStack.push(TSpcSlice());
}

void spcInt(TSpcInterp* interp, u32 arg_count)
{
	if (arg_count != 1)
		interp->verifyArgNum(1, &arg_count);

	TSpcSlice slice = interp->mProcessStack.pop();
	interp->mProcessStack.push(slice.getDataInt());
}

void spcFloat(TSpcInterp* interp, u32 arg_count)
{
	interp->verifyArgNum(1, &arg_count);

	TSpcSlice slice = interp->mProcessStack.pop();
	interp->mProcessStack.push(slice.getDataFloat());
}

void spcTypeof(TSpcInterp* interp, u32 arg_count)
{
	interp->verifyArgNum(1, &arg_count);

	TSpcSlice slice = interp->mProcessStack.pop();
	interp->mProcessStack.push((int)slice.typeof());
}

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
	for (; *name != '\0'; ++name)
		result = *name + result * 3;
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
		TSpcSymbol* candidate = getSymbol(i);
		if (wantHash == candidate->mNameHash) {
			if (strcmp(name, getSymbolName(candidate)) == 0) {
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

void TSpcInterp::execint()
{
	int val = fetchS32_5();
	mProcessStack.push(val);
}

void TSpcInterp::execflt()
{
	TSpcSlice slice;
	(f32&)slice.mData = fetchF32();
	slice.mType       = TSpcSlice::TYPE_FLOAT;
	mProcessStack.push(slice);
}

void TSpcInterp::execstr()
{
	const char* val = fetchString();
	mProcessStack.push(val);
}

void TSpcInterp::execadr()
{
	int val = fetchU32_5();
	mProcessStack.push(val);
}

void TSpcInterp::execvar()
{
	u32 arg1 = fetchU32();
	u32 arg2 = fetchU32();
	mProcessStack.push(mStorageStack.getFromBottom(mDisplay[arg1] + arg2));
}

void TSpcInterp::execnop() { }

void TSpcInterp::execinc()
{
	++mProgramCounter;

	u32 arg1 = fetchU32();
	u32 arg2 = fetchU32();

	++mStorageStack.getFromBottom(mDisplay[arg1] + arg2);

	mStorageStack.push(mStorageStack.getFromBottom(mDisplay[arg1] + arg2));
}

void TSpcInterp::execdec() { }

void TSpcInterp::execadd()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();

	if (arg1.typeof() == TSpcSlice::TYPE_FLOAT
	    || arg2.typeof() == TSpcSlice::TYPE_FLOAT) {
		TSpcSlice result;
		result.setFloat(arg1.getDataFloat() + arg2.getDataFloat());
		mProcessStack.push(result);
	} else {
		mProcessStack.push(arg1.getDataInt() + arg2.getDataInt());
	}
}

void TSpcInterp::execsub() { }

void TSpcInterp::execmul() { }

void TSpcInterp::execdiv() { }

void TSpcInterp::execmod() { }

void TSpcInterp::execass() { }

void TSpcInterp::execeq()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();
	mProcessStack.push((int)(arg1 == arg2));
}

void TSpcInterp::execne()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();
	mProcessStack.push((int)(arg1 != arg2));
}

void TSpcInterp::execgt()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();
	mProcessStack.push((int)(arg1 > arg2));
}

void TSpcInterp::execlt() { }

void TSpcInterp::execge() { }

void TSpcInterp::execle() { }

void TSpcInterp::execneg() { }

void TSpcInterp::execnot()
{
	TSpcSlice slice = mProcessStack.pop();
	int v           = slice.getDataInt() == 0 ? 1 : 0;
	mProcessStack.push(v);
}

void TSpcInterp::execand()
{
	TSpcSlice slice1 = mProcessStack.pop();
	TSpcSlice slice2 = mProcessStack.pop();
	mProcessStack.push(
	    slice1.getDataInt() != 0 && slice2.getDataInt() != 0 ? 1 : 0);
}

void TSpcInterp::execor()
{
	TSpcSlice slice1 = mProcessStack.pop();
	TSpcSlice slice2 = mProcessStack.pop();
	mProcessStack.push(
	    slice1.getDataInt() != 0 || slice2.getDataInt() != 0 ? 1 : 0);
}

void TSpcInterp::execband()
{
	TSpcSlice slice1 = mProcessStack.pop();
	TSpcSlice slice2 = mProcessStack.pop();
	mProcessStack.push(slice1.getDataInt() & slice2.getDataInt());
}

void TSpcInterp::execbor()
{
	TSpcSlice slice1 = mProcessStack.pop();
	TSpcSlice slice2 = mProcessStack.pop();
	mProcessStack.push(slice1.getDataInt() | slice2.getDataInt());
}

void TSpcInterp::execshl()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();
	mProcessStack.push(TSpcSlice(arg1.getDataInt() << arg2.getDataInt()));
}

void TSpcInterp::execshr()
{
	TSpcSlice arg1 = mProcessStack.pop();
	TSpcSlice arg2 = mProcessStack.pop();
	mProcessStack.push(TSpcSlice(arg1.getDataInt() >> arg2.getDataInt()));
}

void TSpcInterp::execcall()
{
	u32 arg1    = fetchU32();
	s32 arg2    = fetchS32();
	u32 counter = mProgramCounter;
	mContextStack.push(counter);
	mContextStack.push(mStorageStack.size());

	for (int i = 0; i < arg2; ++i)
		mStorageStack.push(TSpcSlice());
	for (int i = 0; i < arg2; ++i)
		mStorageStack.getFromTop(i) = mProcessStack.pop();
	mProgramCounter = arg1;
}

void TSpcInterp::execfunc()
{
	u32 arg1 = fetchU32();
	u32 arg2 = fetchU32();
	dispatchBuiltin(arg1, arg2);
}

void TSpcInterp::execmkfr()
{
	s32 arg1 = fetchS32();
	if (!mContextStack.empty())
		for (int i = 0; i < arg1; ++i)
			mStorageStack.push(TSpcSlice());
}

void TSpcInterp::execmkds()
{
	s32 arg1       = fetchS32();
	u32 old        = mDisplay[arg1];
	mDisplay[arg1] = mContextStack.getFromTop(0);
	mContextStack.push(old);
	mContextStack.push(arg1);
}

void TSpcInterp::execret()
{
	u32 uVar1 = mContextStack.pop();
	u32 uVar2 = mContextStack.pop();
	u32 uVar5 = mContextStack.pop();

	for (int i = mStorageStack.size(); i > uVar5; --i)
		mStorageStack.pop();
	mDisplay[uVar1] = uVar2;

	mProgramCounter = mContextStack.pop();
}

void TSpcInterp::execret0()
{
	mProcessStack.push(0);

	execret();
}

void TSpcInterp::execjne()
{
	u32 arg1        = fetchU32();
	TSpcSlice slice = mProcessStack.pop();
	int val         = slice.getDataInt();
	if (val == 0)
		mProgramCounter = arg1;
}

void TSpcInterp::execjmp() { mProgramCounter = fetchU32(); }

void TSpcInterp::execpop() { mProcessStack.pop(); }

void TSpcInterp::execint0() { mProcessStack.push(0); }

void TSpcInterp::execint1() { mProcessStack.push(1); }

void TSpcInterp::execend()
{
	unk4       = 0;
	mStepsLeft = 0;
	SpcTrace("TSpcInterp : script finished\n");
}

void TSpcInterp::chooseExecFunction(u8) { }

void TSpcInterp::dispatchBuiltinDefault(u32 sym_index, u32 arg_count)
{
	TSpcSymbol* sym = mBinary->getSymbol(sym_index);
	if (sym == nullptr) {
		SpcTrace("TSpcInterp : null symbol\n");
		for (int i = 0; i < (int)arg_count; ++i)
			mProcessStack.pop();
		mProcessStack.push(TSpcSlice());
		return;
	}

	typedef void (*Call)(TSpcInterp*, u32);
	Call call = (Call)sym->mNativeCall;
	if (call == nullptr) {
		SpcTrace("TSpcInterp : unknown builtin function %s\n",
		         mBinary->getSymbolName(sym));
		for (int i = 0; i < (int)arg_count; ++i)
			mProcessStack.pop();
		mProcessStack.push(TSpcSlice());
		return;
	}

	unk58 = mBinary->getSymbolName(sym);
	call(this, arg_count);
}

void TSpcInterp::dispatchBuiltin(u32 sym_index, u32 arg_count)
{
	dispatchBuiltinDefault(sym_index, arg_count);
}

TSpcInterp::TSpcInterp(TSpcBinary* param_1, void* param_2, int param_3,
                       int param_4, int param_5, int param_6)
    : mBinary(param_1)
    , unk4(param_3)
    , mProgramCounter(0)
    , mStepsLeft(param_3)
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

	for (int i = 0; i < ARRAY_COUNT(mDisplay); ++i)
		mDisplay[i] = 0;

	for (int i = 0; i < mBinary->getHeader()->unk18; ++i) {
		mStorageStack.push(TSpcSlice(0));
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
		         mProcessStack.getFromTop(i).getDataInt());

	for (int i = 0; i < mContextStack.size(); ++i)
		SpcTrace("mContextStack[%d] = %d\n", i, mContextStack.getFromTop(i));

	for (int i = 0; i < mStorageStack.size(); ++i)
		SpcTrace("mStorageStack[%d] = %d\n", i,
		         mStorageStack.getFromTop(i).getDataInt());

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

void TSpcInterp::callByName(const char*, u32)
{
	SpcTrace("TSpcInterp : cannot call variable symbol %s\n");
}

void TSpcInterp::invokeByAddress(u32, u32) { }

static void dummy()
{
	SpcTrace("TSpcStack : stack overflow\n");
	SpcTrace("TSpcStack : stack underflow\n");
}

void TSpcInterp::invokeByName(const char*, u32)
{
	SpcTrace("TSpcInterp : unknown function, %s\n", "");
	SpcTrace("TSpcInterp : cannot invoke variable symbol %s\n", "");
	SpcTrace("TSpcInterp : cannot invoke builtin, %s\n", "");
}

void TSpcInterp::referByIndex(u32) { }

void TSpcInterp::referByName(const char*)
{
	SpcTrace("TSpcInterp : unknown variable, %s\n", "");
	SpcTrace("TSpcInterp : cannot refer function, %s\n", "");
	SpcTrace("TSpcInterp : cannot refer variable\n");
}

void TSpcInterp::update()
{
	// TODO: inline?
	if (mBinary == nullptr || mBinary->getHeader() == nullptr) {
		SpcTrace("TSpcInterp : null binary\n");
		return;
	}

	while (mStepsLeft > 0) {
		if (!mLocked)
			--mStepsLeft;

		u8 cmd = fetchU8();

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
	mStepsLeft = unk4;
}
