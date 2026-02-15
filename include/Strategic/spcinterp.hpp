#ifndef STRATEGIC_SPCINTERP_HPP
#define STRATEGIC_SPCINTERP_HPP

#include <dolphin/types.h>
#include <string.h>
#include <types.h>

void SpcTrace(const char*, ...);

template <class T> class TSpcStack {
public:
	/* 0x0 */ s32 mCapacity;
	/* 0x4 */ s32 mSize;
	/* 0x8 */ T* mData;

public:
	TSpcStack(u32 capacity)
	    : mCapacity(capacity)
	    , mSize(0)
	    , mData(nullptr)
	{
		mData = new T[mCapacity];
	}

	virtual ~TSpcStack() { delete[] mData; }

	void push(const T& v)
	{
		if (mSize >= mCapacity) {
			SpcTrace("TSpcStack : stack overflow\n");
			return;
		}

		mData[mSize] = v;
		++mSize;
	}

	// fabricated
	T pop()
	{
		if (mSize <= 0) {
			SpcTrace("TSpcStack : stack underflow\n");
			return mData[mSize];
		}
		--mSize;
		return mData[mSize];
	}

	s32 size() const { return mSize; }
	bool empty() const { return mSize <= 0; }
	T& getFromBottom(u32 idx) { return mData[idx]; }
	T& getFromTop(u32 idx) { return mData[mSize - 1 - idx]; }
};

class TSpcSlice {
public:
	/* 0x0 */ u32 mType;
	/* 0x4 */ union {
		int asInt;
		float asFloat;
		const char* asString;
	} mData;

public:
	enum { TYPE_INT, TYPE_FLOAT, TYPE_STRING };

	TSpcSlice()
	    : mType(TYPE_INT)
	{
		(int&)mData = 0;
	}

	TSpcSlice(int value)
	    : mType(TYPE_INT)
	{
		(int&)mData = value;
	}

	TSpcSlice(f32 value)
	    : mType(TYPE_FLOAT)
	{
		(float&)mData = value;
	}

	TSpcSlice(const char* value)
	    : mType(TYPE_STRING)
	{
		(const char*&)mData = value;
	}

	u32 typeof() const { return mType; }

	int getDataInt() const
	{
		switch (mType) {
		case TYPE_INT:
			return mData.asInt;
		case TYPE_FLOAT:
			return mData.asFloat;
		default:
			return 0;
		}
	}

	f32 getDataFloat() const
	{
		switch (mType) {
		case TYPE_INT:
			return mData.asInt;
		case TYPE_FLOAT:
			return mData.asFloat;
		default:
			return 0.0f;
		}
	}

	void setDataInt(int i)
	{
		mData.asInt = i;
		mType       = TYPE_INT;
	}

	void setDataFloat(f32 f)
	{
		mData.asFloat = f;
		mType         = TYPE_FLOAT;
	}

	void setDataString(const char* s)
	{
		mType = TYPE_STRING;
		if (!s)
			mData.asString = "";
		else
			mData.asString = s;
	}

	const char* getDataString() const
	{
		switch (mType) {
		case TYPE_STRING:
			return mData.asString;
		default:
			return "";
		}
	}

	void setFloat(f32 f)
	{
		mType         = TYPE_FLOAT;
		mData.asFloat = f;
	}

	TSpcSlice& operator++()
	{
		switch (mType) {
		case TYPE_INT:
			mData.asInt += 1;
			break;
		case TYPE_FLOAT:
			mData.asFloat += 1.0f;
			break;
		default:
			break;
		}
		return *this;
	}

	TSpcSlice& operator--()
	{
		switch (mType) {
		case TYPE_INT:
			mData.asInt -= 1;
			break;
		case TYPE_FLOAT:
			mData.asFloat -= 1.0f;
			break;
		default:
			break;
		}
		return *this;
	}

	void negate()
	{
		switch (mType) {
		case TSpcSlice::TYPE_INT:
			mData.asInt = -mData.asInt;
			break;
		case TSpcSlice::TYPE_FLOAT:
			mData.asFloat = -mData.asFloat;
			break;
		default:
			break;
		}
	}

	BOOL operator==(const TSpcSlice& other) const
	{
		if (mType == TYPE_STRING && other.mType == TYPE_STRING) {
			if (strcmp(mData.asString, other.mData.asString) == 0)
				return true;
			else
				return false;
		} else if (mType == TYPE_FLOAT || other.mType == TYPE_FLOAT) {
			if (getDataFloat() == other.getDataFloat())
				return true;
			else
				return false;
		} else {
			if (getDataInt() == other.getDataInt())
				return true;
			else
				return false;
		}
	}
	BOOL operator!=(const TSpcSlice& other) const
	{
		if (mType == TYPE_STRING && other.mType == TYPE_STRING) {
			if (strcmp(mData.asString, other.mData.asString) != 0)
				return true;
			else
				return false;
		} else if (mType == TYPE_FLOAT || other.mType == TYPE_FLOAT) {
			if (getDataFloat() != other.getDataFloat())
				return true;
			else
				return false;
		} else {
			if (getDataInt() != other.getDataInt())
				return true;
			else
				return false;
		}
	}

	BOOL operator>(const TSpcSlice& other) const
	{
		if (mType == TYPE_FLOAT || other.mType == TYPE_FLOAT) {
			if (getDataFloat() > other.getDataFloat())
				return true;
			else
				return false;
		} else {
			if (getDataInt() > other.getDataInt())
				return true;
			else
				return false;
		}
	}
};

class TSpcInterp;

struct TSpcSymbol {
	/* 0x0 */ u32 mType;
	/* 0x4 */ u32 mNameOffset;
	/* 0x8 */ u32 mData;
	/* 0xC */ u32 mNameHash;
	/* 0x10 */ u32 mNativeCall;
};

struct TSpcHeader {
	/* 0x0 */ char mMagic[4]; // SPCB
	/* 0x4 */ u32 mTextOffset;
	/* 0x8 */ u32 mDataOffset;
	/* 0xC */ u32 mDataNum;
	/* 0x10 */ u32 mSymbolOffset;
	/* 0x14 */ u32 mSymbolNum;
	/* 0x18 */ s32 unk18;
};

class TSpcBinary {
	/* 0x0 */ u8* mData;

public:
	TSpcBinary(void*);

	virtual ~TSpcBinary();
	virtual void initUserBuiltin();

	void init();
	u32 calcKey(const char*);
	void calcAndStoreKeys();
	void initSystemBuiltin();
	TSpcSymbol* searchSymbol(const char*);
	void bindSystemDataToSymbol(const char*, u32);

	TSpcHeader* getHeader() const { return (TSpcHeader*)mData; }

	// fabricated
	TSpcSymbol* getSymbol(u32 idx)
	{
		return &((TSpcSymbol*)(mData + getHeader()->mSymbolOffset))[idx];
	}
	const char* getSymbolName(TSpcSymbol* symbol)
	{
		return (const char*)(mData + getHeader()->mSymbolOffset
		                     + getHeader()->mSymbolNum * sizeof(TSpcSymbol)
		                     + symbol->mNameOffset);
	}
	u32 getDataOffset(u32 idx)
	{
		return *(u32*)(mData + getHeader()->mDataOffset + idx * sizeof(u32));
	}
	void* getData(u32 offset)
	{
		return mData + getHeader()->mDataOffset + getHeader()->mDataNum * 4
		       + offset;
	}
	u8* getText(u32 offset)
	{
		return mData + getHeader()->mTextOffset + offset;
	}
};

class TSpcInterp {
public:
	/* 0x0 */ TSpcBinary* mBinary;
	/* 0x4 */ u32 mStepsToDo;
	/* 0x8 */ u32 mProgramCounter;
	/* 0xC */ s32 mStepsLeft;
	/* 0x10 */ void* unk10;
	/* 0x14 */ s32 mLocked;
	/* 0x18 */ TSpcStack<TSpcSlice> mProcessStack;
	/* 0x28 */ TSpcStack<TSpcSlice> mStorageStack;
	/* 0x38 */ TSpcStack<u32> mContextStack;
	/* 0x48 */ u32 mDisplay[4];
	/* 0x58 */ const char* mCurrentlyExecutingBuiltinName;

public:
	// TODO: the fetches take up too much stack
	static void hacky_memcpy(u8* dst, u8* src, u32 count)
	{
		for (int i = 0; i < count; ++i)
			dst[i] = src[i];
	}

	f32 fetchF32()
	{
		f32 result;
		hacky_memcpy((u8*)&result, mBinary->getText(mProgramCounter), 4);
		mProgramCounter += 4;
		return result;
	}
	s32 fetchS32()
	{
		s32 result;
		hacky_memcpy((u8*)&result, mBinary->getText(mProgramCounter), 4);
		mProgramCounter += 4;
		return result;
	}

	s32 fetchS32_2() { return fetchS32(); }
	s32 fetchS32_3() { return fetchS32_2(); }
	s32 fetchS32_4() { return fetchS32_3(); }
	s32 fetchS32_5() { return fetchS32_4(); }

	u32 fetchU32()
	{
		u32 result;
		hacky_memcpy((u8*)&result, mBinary->getText(mProgramCounter), 4);
		mProgramCounter += 4;
		return result;
	}

	u32 fetchU32_2() { return fetchU32(); }
	u32 fetchU32_3() { return fetchU32_2(); }
	u32 fetchU32_4() { return fetchU32_3(); }
	u32 fetchU32_5() { return fetchU32_4(); }

	u8 fetchU8()
	{
		u8 result = *mBinary->getText(mProgramCounter);
		++mProgramCounter;
		return result;
	}

	const char* fetchString()
	{
		const char* str
		    = (const char*)mBinary->getData(mBinary->getDataOffset(fetchU32()));
		if (str == nullptr)
			str = "";
		return str;
	}

	void push(const TSpcSlice& slice) { mProcessStack.push(slice); }
	void push() { push(TSpcSlice()); }
	TSpcSlice pop() { return mProcessStack.pop(); }

public:
	/// Pushes an immediate int constant C to the process stack
	/// Immediate arguments: C
	/// Returns: C
	void execint();
	/// Pushes an immediate float constant C to the process stack
	/// Immediate arguments: C
	/// Returns: C
	void execflt();
	/// Pushes a string constant from the binary's data section with index C to
	/// the process stack
	/// Immediate arguments: C
	/// Returns: C
	void execstr();
	/// Pushes an immediate address constant C to the process stack
	/// Immediate arguments: C
	/// Returns: C
	void execadr();
	/// Reads the variable V from layer L and pushes it to the process stack
	/// Immediate arguments: L, V
	/// Returns: the current value of the variable
	void execvar();
	/// No-op
	void execnop();
	/// Increments the variable V from layer L and pushes it's new value to the
	/// process stack
	/// Immediate arguments: L, V
	/// Returns: new value of the variable
	void execinc();
	/// Decrements the variable V from layer L and pushes it's new value to the
	/// process stack
	/// Immediate arguments: L, V
	/// Returns: new value of the variable
	void execdec();
	/// Pops two values A, B from the process stack, adds them and pushes the
	/// result back to it
	/// Arguments: A, B
	/// Returns: A + B
	void execadd();
	/// Pops two values A, B from the process stack, subtracts them and pushes
	/// the result back to it
	/// Arguments: A, B
	/// Returns: A - B
	void execsub();
	/// Pops two values A, B from the process stack, multiplies them and pushes
	/// the result back to it
	/// Arguments: A, B
	/// Returns: A * B
	void execmul();
	/// Pops two values A, B from the process stack, divides them and pushes
	/// the result back to it
	/// Arguments: A, B
	/// Returns: A / B
	void execdiv();
	/// Pops two values A, B from the process stack, modulos them and pushes
	/// the result back to it
	/// Arguments: A, B
	/// Returns: A % B
	void execmod();
	/// Pops value A from the process stack and assigns it to variable V in
	/// layer L
	/// Immediate arguments: L, V
	/// Arguments: A
	void execass();
	/// Pops two values A, B from the process stack, compares them for equality
	/// and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A == B
	void execeq();
	/// Pops two values A, B from the process stack, compares them for
	/// inequality and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A != B
	void execne();
	/// Pops two values A, B from the process stack, compares them for
	/// being greater then and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A > B
	void execgt();
	/// Pops two values A, B from the process stack, compares them for
	/// being less then and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A < B
	void execlt();
	/// Pops two values A, B from the process stack, compares them for
	/// being greater then or equal and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A >= B
	void execge();
	/// Pops two values A, B from the process stack, compares them for
	/// being less then or equal and pushes the result back to it
	/// Arguments: A, B
	/// Returns: A <= B
	void execle();
	/// Pops value A from the stack, negates it and pushes -A back
	/// Arguments: A
	/// Returns: -A
	void execneg();
	/// Pops value A from the stack, logically negates it and pushes the result
	/// back to the stack
	/// Arguments: A
	/// Returns: 1 if A != 0, 0 otherwise
	void execnot();
	/// Pops values A, B from the stack, checks whether both are non-zero and
	/// pushes the result back
	/// Arguments: A, B
	/// Returns: 1 if A != 0 && B != 0, 0 otherwise
	void execand();
	/// Pops values A, B from the stack, checks whether at least one is non-zero
	/// and pushes the result
	/// Arguments: A, B
	/// Returns: 1 if A != 0 || B != 0, 0 otherwise
	void execor();
	/// Pops two values A, B from the process stack, bitwise ANDs them and
	/// pushes the result back to it
	/// Arguments: A, B
	/// Returns: A & B
	void execband();
	/// Pops two values A, B from the process stack, bitwise ORs them and
	/// pushes the result back to it
	/// Arguments: A, B
	/// Returns: A | B
	void execbor();
	/// Pops two values A, B from the process stack, bitwise shifts one of them
	/// left by the other and pushes the result back
	/// Arguments: A, B
	/// Returns: A << B
	void execshl();
	/// Pops two values A, B from the process stack, bitwise shifts one of them
	/// right by the other and pushes the result back
	/// Arguments: A, B
	/// Returns: A >> B
	void execshr();
	/// Calls an SPC function present in the binary at offset A with N topmost
	/// values from the process stack as arguments
	/// Immediate arguments: A, N
	/// Arguments: depends
	/// Result: depends
	void execcall();
	/// Calls a native function with id I bound to SPC with the topmost N values
	/// from the process stack as arguments
	/// Immediate arguments: I, N
	/// Arguments: depends
	/// Result: depends
	void execfunc();
	/// Allocates N new variables in the current function's stack frame with
	/// integral value of 0
	/// Immediate arguments: N
	void execmkfr();
	/// Makes a "stack frame" for a function in layer L. Expected to be called
	/// at the start of any SPC function
	/// Immediate arguments: L
	void execmkds();
	/// Returns from an SPC function. If call and mkds were not called before it
	/// in that exact order, everything will blow up in flames
	void execret();
	/// Pushes 0 to the process stack and returns from an SPC function,
	// equivalent to int 0, ret.
	void execret0();
	/// Pops value B from the process stack and if it is zero, jumps to the
	/// immediate address A
	/// Immediate arguments: A
	/// Arguments: B
	void execjne();
	/// Jumps to the immediate address A
	/// Immediate arguments: A
	void execjmp();
	/// Pops a value from the process stack
	void execpop();
	/// Pushes an int 0 to the process stack
	void execint0();
	/// Pushes an int 1 to the process stack
	void execint1();
	/// Concludes execution of an SPC script
	void execend();

	void chooseExecFunction(u8);
	void dispatchBuiltinDefault(u32 sym_index, u32 arg_count);

	virtual void dispatchBuiltin(u32 sym_index, u32 arg_count);

	TSpcInterp(TSpcBinary* binary, void* owner, int steps,
	           int process_stack_size, int storage_stack_size,
	           int context_stack_size);
	virtual ~TSpcInterp();

	void dump();
	void verifyArgNum(u32, u32*);
	void callByAddress(u32, u32);
	void callByName(const char*, u32);
	void invokeByAddress(u32, u32);
	void invokeByName(const char*, u32);
	void referByIndex(u32);
	void referByName(const char*);

	virtual void update();
};

template <class T> class TSpcTypedBinary : public TSpcBinary {
public:
	TSpcTypedBinary(void* data)
	    : TSpcBinary(data)
	{
	}

	virtual ~TSpcTypedBinary() { }
	virtual void initUserBuiltin();
};

template <class T> class TSpcTypedInterp : public TSpcInterp {
public:
	TSpcTypedInterp(TSpcBinary* binary, void* owner, int steps,
	                int process_stack_size, int storage_stack_size,
	                int context_stack_size)
	    : TSpcInterp(binary, owner, steps, process_stack_size,
	                 storage_stack_size, context_stack_size)
	{
	}

	virtual void dispatchBuiltin(u32 sym_index, u32 arg_count)
	{
		typedef void (*TypedNativeCall)(TSpcTypedInterp<T>*, u32);
		TSpcSymbol* sym = mBinary->getSymbol(sym_index);

		if (sym && sym->mNativeCall) {
			TypedNativeCall call = (TypedNativeCall)sym->mNativeCall;

			mCurrentlyExecutingBuiltinName = mBinary->getSymbolName(sym);
			call(this, arg_count);
		} else {
			TSpcInterp::dispatchBuiltin(sym_index, arg_count);
		}
	}

	virtual ~TSpcTypedInterp() { }

	// fabricated
	T* getOwner() const { return (T*)unk10; }
};

#endif
