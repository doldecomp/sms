#ifndef JKR_ENUM_H
#define JKR_ENUM_H


#define JKRCOMPRESSION_NONE 0
#define JKRCOMPRESSION_YAY0 1
#define JKRCOMPRESSION_YAZ0 2
#define JKRCOMPRESSION_ASR 3

enum JKRExpandSwitch
{
  EXPAND_SWITCH_DEFAULT,    /* Do nothing? treated same as 2 */
  EXPAND_SWITCH_DECOMPRESS, /* Check for compression and decompress */
  EXPAND_SWITCH_NONE        /* Do nothing */
};

#endif
