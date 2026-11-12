#ifndef JASCMDSTACK_HPP
#define JASCMDSTACK_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrack;

namespace Kernel {

	struct TPortArgs {
		enum UpdateFlag {
			UPDATE_VOLUME                = 1 << 0,
			UPDATE_PITCH                 = 1 << 1,
			UPDATE_PAN                   = 1 << 2,
			UPDATE_FXMIX                 = 1 << 3,
			UPDATE_DOLBY                 = 1 << 4,
			UPDATE_APPLICATION_INTERRUPT = 1 << 6,
			UPDATE_TEMPO                 = 1 << 7,
		};

		/* 0x00 */ TTrack* mTrack;
		/* 0x04 */ u32 mUpdateFlags;
		/* 0x08 */ f32 mTrackVolume;
		/* 0x0C */ f32 mTrackPitch;
		/* 0x10 */ f32 mTrackPan;
		/* 0x14 */ f32 mTrackFxmix;
		/* 0x18 */ f32 mTrackDolby;
		/* 0x1C */ f32 unk1C;
		/* 0x20 */ u32 mRequestApplicationInterrupt;
		/* 0x24 */ f32 mTrackTempo;
	};

	class TPortCmd;

	struct TPortHead {
		/* 0x00 */ TPortCmd* mFirst;
		/* 0x04 */ TPortCmd* mLast;
	};

	typedef void (*PortCallback)(TPortArgs*);

	class TPortCmd {
	public:
		TPortCmd();
		~TPortCmd() { }

		BOOL addPortCmdOnce();
		BOOL addPortCmdStay();
		BOOL setPortCmd(PortCallback callback, TPortArgs* args);
		BOOL addPortCmd(TPortHead* queue);
		void cancelPortCmd(TPortHead* queue);
		void cancelPortCmdStay();

		TPortHead* getHead() { return mQueue; }
		void setHead(TPortHead* queue) { mQueue = queue; }
		TPortCmd* getNext() { return mNext; }
		PortCallback getFunc() { return mCallback; }
		TPortArgs* getArgs() { return mArgs; }

	public:
		/* 0x00 */ TPortHead* mQueue;
		/* 0x04 */ TPortCmd* mNext;
		/* 0x08 */ PortCallback mCallback;
		/* 0x0C */ TPortArgs* mArgs;
	};

	void portCmdInit();
	void portCmdProcOnce(TPortHead* queue);
	void portCmdProcStay(TPortHead* queue);
	void portHeadInit(TPortHead* queue);

} // namespace Kernel

} // namespace JASystem

#endif // JASCMDSTACK_HPP
