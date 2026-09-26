#ifndef JKR_ARAM_PIECE_HPP
#define JKR_ARAM_PIECE_HPP

#include <stdint.h>
#include <JSystem/JSupport/JSUList.hpp>
#include <dolphin/arq.h>
#include <dolphin/os/OSMessage.h>
#include <dolphin/os/OSMutex.h>

class JKRAramBlock;
class JKRDecompCommand;
class JKRAMCommand {
public:
	typedef void (*AsyncCallback)(uintptr_t);

	JKRAMCommand();
	~JKRAMCommand();

	uintptr_t getDestination();

public:
	/* 0x00 */ ARQRequest mRequest;
	/* 0x20 */ JSULink<JKRAMCommand> mPieceLink;
	/* 0x30 */ JSULink<JKRAMCommand> field_0x30;

	/* 0x40 */ s32 mTransferDirection;
	/* 0x44 */ u32 mDataLength;
	/* 0x48 */ uintptr_t mSrc;
	/* 0x4C */ uintptr_t mDst;
	/* 0x50 */ JKRAramBlock* mAramBlock;
	/* 0x54 */ u32 field_0x54;
	/* 0x58 */ AsyncCallback mCallback;
	/* 0x5C */ OSMessageQueue* field_0x5C;
	/* 0x60 */ s32 field_0x60;
	/* 0x64 */ JKRDecompCommand* mDecompCommand;
	/* 0x68 */ OSMessageQueue mMessageQueue;
	/* 0x88 */ OSMessage mMessage;
	/* 0x8C */ void* field_0x8C;
	/* 0x90 */ void* field_0x90;
	/* 0x94 */ void* field_0x94;
};

class JKRAramPiece {
public:
	static OSMutex mMutex;
	// TODO: fix type
	static JSUList<JKRAMCommand> sAramPieceCommandList;

public:
	struct Message {
		s32 field_0x00;
		JKRAMCommand* command;
	};

public:
	JKRAramPiece();
	~JKRAramPiece();

	static JKRAMCommand* prepareCommand(int, uintptr_t, uintptr_t, u32,
	                                    JKRAramBlock*,
	                                    JKRAMCommand::AsyncCallback);
	static void sendCommand(JKRAMCommand*);

	static JKRAMCommand* orderAsync(int, uintptr_t, uintptr_t, u32,
	                                JKRAramBlock*,
	                                JKRAMCommand::AsyncCallback);
	static bool sync(JKRAMCommand*, int);
	static void syncAll(int);
	static bool orderSync(int, uintptr_t, uintptr_t, u32, JKRAramBlock*);
	static void startDMA(JKRAMCommand*);
	static void doneDMA(uintptr_t);

private:
	static void lock() { OSLockMutex(&mMutex); }
	static void unlock() { OSUnlockMutex(&mMutex); }
};

inline bool JKRAramPcs(int direction, uintptr_t source, uintptr_t destination,
                       u32 length,
                       JKRAramBlock* block)
{
	return JKRAramPiece::orderSync(direction, source, destination, length,
	                               block);
}

#endif
