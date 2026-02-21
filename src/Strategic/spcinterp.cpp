#include <Strategic/spcinterp.hpp>
#include <macros.h>

void spcYield(TSpcInterp* interp, u32 arg_count)
{
	interp->mStepsLeft = 0;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->pop();
	interp->push();
}

void spcExit(TSpcInterp* interp, u32 arg_count)
{
	interp->mStepsToDo = 0;
	interp->mStepsLeft = 0;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->pop();
	interp->push();
}

void spcLock(TSpcInterp* interp, u32 arg_count)
{
	interp->mLocked = TRUE;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->pop();
	interp->push();
}

void spcUnlock(TSpcInterp* interp, u32 arg_count)
{
	interp->mLocked = FALSE;
	for (int i = 0; i < (int)arg_count; ++i)
		interp->pop();
	interp->push();
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
		interp->pop();
	interp->push();
}

void spcDump(TSpcInterp* interp, u32 arg_count)
{
	interp->dump();

	for (int i = 0; i < (int)arg_count; ++i)
		interp->pop();
	interp->push();
}

void spcInt(TSpcInterp* interp, u32 arg_count)
{
	if (arg_count != 1)
		interp->verifyArgNum(1, &arg_count);

	int val = interp->pop().getDataInt();
	interp->push(val);
}

void spcFloat(TSpcInterp* interp, u32 arg_count)
{
	if (arg_count != 1)
		interp->verifyArgNum(1, &arg_count);

	TSpcSlice slice;
	slice   = interp->pop();
	f32 flt = slice.getDataFloat();
	interp->push(flt);
}

void spcTypeof(TSpcInterp* interp, u32 arg_count)
{
	if (arg_count != 1)
		interp->verifyArgNum(1, &arg_count);

	TSpcSlice slice = interp->pop();
	interp->push((int)slice.typeof());
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
	f32 data = fetchF32();
	slice.setDataFloat(data);
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

	int idx = mDisplay[arg1];
	++mStorageStack.getFromBottom(idx + arg2);

	mProcessStack.push(mStorageStack.getFromBottom(idx + arg2));
}

void TSpcInterp::execdec()
{
	++mProgramCounter;

	u32 arg1 = fetchU32();
	u32 arg2 = fetchU32();

	int idx = mDisplay[arg1];
	--mStorageStack.getFromBottom(idx + arg2);

	mProcessStack.push(mStorageStack.getFromBottom(idx + arg2));
}

void TSpcInterp::execadd()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();

	if (arg1.typeof() == TSpcSlice::TYPE_FLOAT
	    || arg2.typeof() == TSpcSlice::TYPE_FLOAT) {
		TSpcSlice result;
		result.setDataFloat(arg1.getDataFloat() + arg2.getDataFloat());
		mProcessStack.push(result);
	} else {
		int result = arg1.getDataInt() + arg2.getDataInt();
		mProcessStack.push(result);
	}
}

void TSpcInterp::execsub()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();

	if (arg1.typeof() == TSpcSlice::TYPE_FLOAT
	    || arg2.typeof() == TSpcSlice::TYPE_FLOAT) {
		TSpcSlice result;
		result.setDataFloat(arg1.getDataFloat() - arg2.getDataFloat());
		mProcessStack.push(result);
	} else {
		int result = arg1.getDataInt() - arg2.getDataInt();
		mProcessStack.push(result);
	}
}

void TSpcInterp::execmul()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();

	if (arg1.typeof() == TSpcSlice::TYPE_FLOAT
	    || arg2.typeof() == TSpcSlice::TYPE_FLOAT) {
		TSpcSlice result;
		result.setDataFloat(arg1.getDataFloat() * arg2.getDataFloat());
		mProcessStack.push(result);
	} else {
		int result = arg1.getDataInt() * arg2.getDataInt();
		mProcessStack.push(result);
	}
}

void TSpcInterp::execdiv()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();

	if (arg1.typeof() == TSpcSlice::TYPE_FLOAT
	    || arg2.typeof() == TSpcSlice::TYPE_FLOAT) {
		TSpcSlice result;
		result.setDataFloat(arg1.getDataFloat() / arg2.getDataFloat());
		mProcessStack.push(result);
	} else {
		int result = arg1.getDataInt() / arg2.getDataInt();
		mProcessStack.push(result);
	}
}

void TSpcInterp::execmod()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();

	int result = arg1.getDataInt() % arg2.getDataInt();
	mProcessStack.push(result);
}

void TSpcInterp::execass()
{
	TSpcSlice slice = mProcessStack.pop();
	++mProgramCounter;
	u32 arg1 = fetchU32();
	u32 arg2 = fetchU32();

	mStorageStack.getFromBottom(mDisplay[arg1] + arg2) = slice;
}

void TSpcInterp::execeq()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	mProcessStack.push(arg1 == arg2);
}

void TSpcInterp::execne()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	mProcessStack.push(arg1 != arg2);
}

void TSpcInterp::execgt()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	BOOL result    = arg1 > arg2;
	mProcessStack.push(result);
}

void TSpcInterp::execlt()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	BOOL result    = arg1 < arg2;
	mProcessStack.push(result);
}

void TSpcInterp::execge()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	BOOL result    = arg1 >= arg2;
	mProcessStack.push(result);
}

void TSpcInterp::execle()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	BOOL result    = arg1 <= arg2;
	mProcessStack.push(result);
}

void TSpcInterp::execneg()
{
	TSpcSlice slice = mProcessStack.pop();
	slice.negate();
	mProcessStack.push(slice);
}

void TSpcInterp::execnot()
{
	TSpcSlice slice = mProcessStack.pop();
	int result      = slice.getDataInt() == 0 ? 1 : 0;
	mProcessStack.push(result);
}

void TSpcInterp::execand()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() != 0 && arg2.getDataInt() != 0 ? 1 : 0;
	mProcessStack.push(result);
}

void TSpcInterp::execor()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() != 0 || arg2.getDataInt() != 0 ? 1 : 0;
	mProcessStack.push(result);
}

void TSpcInterp::execband()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() & arg2.getDataInt();
	mProcessStack.push(result);
}

void TSpcInterp::execbor()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() | arg2.getDataInt();
	mProcessStack.push(result);
}

void TSpcInterp::execshl()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() << arg2.getDataInt();
	mProcessStack.push(result);
}

void TSpcInterp::execshr()
{
	TSpcSlice arg2 = mProcessStack.pop();
	TSpcSlice arg1 = mProcessStack.pop();
	int result     = arg1.getDataInt() >> arg2.getDataInt();
	mProcessStack.push(TSpcSlice(result));
}

void TSpcInterp::execcall()
{
	u32 address = fetchU32();
	s32 argNum  = fetchS32();
	u32 counter = mProgramCounter;

	mContextStack.push(counter);
	mContextStack.push(mStorageStack.size());

	for (int i = 0; i < argNum; ++i)
		mStorageStack.push(TSpcSlice());
	for (int i = 0; i < argNum; ++i)
		mStorageStack.setFromTop(i, mProcessStack.pop());
	mProgramCounter = address;
}

void TSpcInterp::execfunc()
{
	u32 builtinIdx = fetchU32();
	u32 argNum     = fetchU32();
	dispatchBuiltin(builtinIdx, argNum);
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
	s32 displayIdx       = fetchS32();
	u32 display          = mDisplay[displayIdx];
	mDisplay[displayIdx] = mContextStack.getFromTop(0);
	mContextStack.push(display);
	mContextStack.push(displayIdx);
}

void TSpcInterp::execret()
{
	u32 restoredDisplayIdx       = mContextStack.pop();
	u32 restoredDisplay          = mContextStack.pop();
	u32 restoredStorageStackSize = mContextStack.pop();

	for (int i = mStorageStack.size(); i > restoredStorageStackSize; --i)
		mStorageStack.pop();
	mDisplay[restoredDisplayIdx] = restoredDisplay;

	mProgramCounter = mContextStack.pop();
}

void TSpcInterp::execret0()
{
	mProcessStack.push(0);

	execret();
}

void TSpcInterp::execjne()
{
	u32 arg1 = fetchU32();
	if (!pop().getDataInt())
		mProgramCounter = arg1;
}

void TSpcInterp::execjmp() { mProgramCounter = fetchU32(); }

void TSpcInterp::execpop() { mProcessStack.pop(); }

void TSpcInterp::execint0() { mProcessStack.push(0); }

void TSpcInterp::execint1() { mProcessStack.push(1); }

void TSpcInterp::execend()
{
	mStepsToDo = 0;
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

	mCurrentlyExecutingBuiltinName = mBinary->getSymbolName(sym);
	call(this, arg_count);
}

void TSpcInterp::dispatchBuiltin(u32 sym_index, u32 arg_count)
{
	dispatchBuiltinDefault(sym_index, arg_count);
}

TSpcInterp::TSpcInterp(TSpcBinary* binary, void* owner, int steps,
                       int process_stack_size, int storage_stack_size,
                       int context_stack_size)
    : mBinary(binary)
    , mStepsToDo(steps)
    , mProgramCounter(0)
    , mStepsLeft(steps)
    , unk10(owner)
    , mProcessStack(process_stack_size)
    , mStorageStack(storage_stack_size)
    , mContextStack(context_stack_size)
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

	SpcTrace("TSpcInterp : argument number mismatch (%s, %d -> %d)\n",
	         mCurrentlyExecutingBuiltinName, *param_2, param_1);

	while (param_1 < *param_2) {
		pop();
		--*param_2;
	}

	while (param_1 > *param_2) {
		push();
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

// NOTE: the stuff above is supposed to be referencing these float constants
static void dummy_7(f32& x) { x = 1.0f; }
static void dummy_6(f32& x, int y) { x = y; }
static void dummy_5(f32& x) { x = 0.0f; }

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
		static Handler table[] = {
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
		if (cmd >= ARRAY_COUNT(table)) {
			SpcTrace("TSpcInterp : unknown command 0x%x\n", cmd);
			handler = nullptr;
		} else {
			handler = table[cmd];
		}

		if (handler)
			(*this.*handler)();
	}
	mStepsLeft = mStepsToDo;
}
