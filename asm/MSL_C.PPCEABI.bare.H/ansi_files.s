.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global __files
__files:
    .4byte 0
    .4byte 0x08800000
    .4byte 0
lbl_803AEC0C:
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte lbl_803AEC0C
    .4byte 1
    .4byte lbl_803AEC0C
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte __read_console
    .4byte __write_console
    .4byte __close_console
    .4byte 0

    .4byte 0
    .4byte 0x10800000
    .4byte 0
lbl_803AEC54:
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte lbl_803AEC54
    .4byte 1
    .4byte lbl_803AEC54
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte __read_console
    .4byte __write_console
    .4byte __close_console
    .4byte 0

    .4byte 0
    .4byte 0x10800000
    .4byte 0
lbl_803AEC9C:
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte lbl_803AEC9C
    .4byte 1
    .4byte lbl_803AEC9C
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte 0
    .4byte __read_console
    .4byte __write_console
    .4byte __close_console
    .4byte 0

