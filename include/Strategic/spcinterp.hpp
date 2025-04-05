#ifndef STRATEGIC_SPCINTERP_HPP
#define STRATEGIC_SPCINTERP_HPP

#include <dolphin/types.h>
#include <string.h>
#include <types.h>

void SpcTrace(const char*, ...);

template <class T> class TSpcStack {
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

	virtual ~TSpcStack() { }

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
	void pop()
	{
		if (mSize <= 0) {
			SpcTrace("TSpcStack : stack underflow\n");
			return;
		}
		--mSize;
	}

	s32 size() { return mSize; }
	T& operator[](u32 idx) { return mData[idx]; }
};

class TSpcSlice {
	/* 0x0 */ u32 mType;
	/* 0x4 */ union {
		s32 mDataAsInt;
		f32 mDataAsFloat;
		const char* mDataAsString;
	};

	enum { TYPE_INT, TYPE_FLOAT, TYPE_STRING };

public:
	TSpcSlice()
	    : mType(0)
	{
		mDataAsInt = 0;
	}

	s32 getDataInt() const
	{
		switch (mType) {
		case TYPE_INT:
			return mDataAsInt;
		case TYPE_FLOAT:
			return mDataAsFloat;
		default:
			return 0;
		}
	}

	f32 getDataFloat() const
	{
		switch (mType) {
		case TYPE_INT:
			return mDataAsInt;
		case TYPE_FLOAT:
			return mDataAsFloat;
		default:
			return 0.0f;
		}
	}

	bool operator!=(const TSpcSlice& other) const
	{
		if (mType == TYPE_STRING && other.mType == TYPE_STRING) {
			if (strcmp(mDataAsString, other.mDataAsString) == 0)
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
	bool operator==(const TSpcSlice& other) const { return !(*this == other); }
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
};

class TSpcInterp {
	/* 0x0 */ TSpcBinary* mBinary;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ s32 unkC;
	/* 0x10 */ void* unk10;
	/* 0x14 */ s32 unk14;
	/* 0x18 */ TSpcStack<TSpcSlice> mProcessStack;
	/* 0x28 */ TSpcStack<TSpcSlice> mStorageStack;
	/* 0x38 */ TSpcStack<u32> mContextStack;
	/* 0x48 */ u32 mDisplay[4];
	/* 0x58 */ u32 unk58;

public:
	void fetchS32() { }
	void fetchU32() { }

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
	void dispatchBuiltinDefault(u32, u32);

	virtual void dispatchBuiltin(u32, u32);

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
};

#endif
