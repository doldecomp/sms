#ifndef JSU_STREAM_ENUM_H
#define JSU_STREAM_ENUM_H

enum JSUStreamSeekFrom {
	JSUStreamSeekFrom_SET = 0,
	JSUStreamSeekFrom_CUR = 1,
	JSUStreamSeekFrom_END = 2
};

enum EIoState { GOOD = 0, EOF = 1 };

#endif
