#ifndef JASCMDSTACK_HPP
#define JASCMDSTACK_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrack;

namespace Kernel {

	struct TPortArgs {
		/* 0x00 */ TTrack* mTrack;
		/* 0x04 */ u32 mFlags;
		/* 0x08 */ f32 mTrackVolume;
		/* 0x0C */ f32 mTrackPitch;
		/* 0x10 */ f32 mTrackPan;
		/* 0x14 */ f32 mTrackFxmix;
		/* 0x18 */ f32 mTrackDolby;
		/* 0x1C */ f32 unk1C;
		/* 0x20 */ u32 unk20;
		/* 0x24 */ f32 mTrackTempo;
	};

	class TPortCmd;

	struct TPortHead {
		/* 0x00 */ TPortCmd* unk0;
		/* 0x04 */ TPortCmd* unk4;
	};

	typedef void (*PortCallback)(TPortArgs*);

	class TPortCmd {
	public:
		TPortCmd();
		BOOL addPortCmdOnce();
		BOOL addPortCmdStay();
		BOOL setPortCmd(PortCallback func, TPortArgs* args);
		BOOL addPortCmd(TPortHead* head);
		void cancelPortCmd(TPortHead* head);
		void cancelPortCmdStay();

	public:
		/* 0x00 */ TPortHead* unk0;
		/* 0x04 */ TPortCmd* unk4;
		/* 0x08 */ PortCallback unk8;
		/* 0x0C */ TPortArgs* unkC;
	};

	void portCmdInit();
	void portCmdProcOnce(TPortHead* head);
	void portCmdProcStay(TPortHead* head);
	void portHeadInit(TPortHead* head);

} // namespace Kernel

} // namespace JASystem

#endif // JASCMDSTACK_HPP
