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
			return (int&)mData;
		case TYPE_FLOAT:
			return (f32&)mData;
		default:
			return 0;
		}
	}

	f32 getDataFloat() const
	{
		switch (mType) {
		case TYPE_INT:
			return (int&)mData;
		case TYPE_FLOAT:
			return (f32&)mData;
		default:
			return 0.0f;
		}
	}

	const char* getDataString() const
	{
		switch (mType) {
		case TYPE_STRING:
			return (const char*&)mData;
		default:
			return "";
		}
	}

	void setFloat(f32 f)
	{
		mType       = TYPE_FLOAT;
		(f32&)mData = f;
	}

	TSpcSlice& operator++()
	{
		switch (mType) {
		case TYPE_INT:
			(int&)mData += 1;
			break;
		case TYPE_FLOAT:
			(float&)mData += 1.0f;
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
			(int&)mData -= 1;
			break;
		case TYPE_FLOAT:
			(float&)mData -= 1.0f;
			break;
		default:
			break;
		}
		return *this;
	}

	BOOL operator==(const TSpcSlice& other) const
	{
		if (mType == TYPE_STRING && other.mType == TYPE_STRING) {
			if (strcmp((const char*&)mData, (const char*&)other.mData) == 0)
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
	BOOL operator!=(const TSpcSlice& other) const { return !(*this == other); }

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
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 mProgramCounter;
	/* 0xC */ s32 mStepsLeft;
	/* 0x10 */ void* unk10;
	/* 0x14 */ s32 mLocked;
	/* 0x18 */ TSpcStack<TSpcSlice> mProcessStack;
	/* 0x28 */ TSpcStack<TSpcSlice> mStorageStack;
	/* 0x38 */ TSpcStack<u32> mContextStack;
	/* 0x48 */ u32 mDisplay[4];
	/* 0x58 */ const char* unk58;

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

public:
	void execint();
	void execflt();
	void execstr();
	void execadr();
	void execvar();
	void execnop();
	void execinc();
	void execdec();
	void execadd();
	void execsub();
	void execmul();
	void execdiv();
	void execmod();
	void execass();
	void execeq();
	void execne();
	void execgt();
	void execlt();
	void execge();
	void execle();
	void execneg();
	void execnot();
	void execand();
	void execor();
	void execband();
	void execbor();
	void execshl();
	void execshr();
	void execcall();
	void execfunc();
	void execmkfr();
	void execmkds();
	void execret();
	void execret0();
	void execjne();
	void execjmp();
	void execpop();
	void execint0();
	void execint1();
	void execend();

	void chooseExecFunction(u8);
	void dispatchBuiltinDefault(u32 sym_index, u32 arg_count);

	virtual void dispatchBuiltin(u32 sym_index, u32 arg_count);

	TSpcInterp(TSpcBinary*, void*, int, int, int, int);
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

	// fabricated
	void skipCall(int arg_count)
	{
		for (int i = 0; i < arg_count; ++i)
			mProcessStack.pop();
		mProcessStack.push(TSpcSlice());
	}
};

#endif
