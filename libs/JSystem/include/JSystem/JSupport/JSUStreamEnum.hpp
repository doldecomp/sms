#ifndef JSU_STREAM_ENUM_H
#define JSU_STREAM_ENUM_H

enum JSUStreamSeekFrom {
	JSUStreamSeekFrom_SET = 0,
	JSUStreamSeekFrom_CUR = 1,
	JSUStreamSeekFrom_END = 2
};

enum EIoState { EIoState_GOOD = 0, EIoState_EOF = 1 };

#endif
