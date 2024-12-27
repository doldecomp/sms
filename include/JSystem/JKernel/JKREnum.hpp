#ifndef JKR_ENUM_H
#define JKR_ENUM_H

typedef int JKRCompression;
const JKRCompression JKR_COMPRESSION_NONE = 0;
const JKRCompression JKR_COMPRESSION_YAY0 = 1;
const JKRCompression JKR_COMPRESSION_YAZ0 = 2;
const JKRCompression JKR_COMPRESSION_ASR  = 3;

enum JKRExpandSwitch {
	EXPAND_SWITCH_DEFAULT,    /* Do nothing? treated same as 2 */
	EXPAND_SWITCH_DECOMPRESS, /* Check for compression and decompress */
	EXPAND_SWITCH_NONE        /* Do nothing */
};

#endif
