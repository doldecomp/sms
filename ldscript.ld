ENTRY(__start)
_SDA_BASE_ = 0x80410AC0;
_SDA2_BASE_ = 0x80413460;
PHDRS
{
    text0 PT_LOAD;
    text1 PT_LOAD;
    data0 PT_LOAD;
    data1 PT_LOAD;
    data2 PT_LOAD;
    data3 PT_LOAD;
    data4 PT_LOAD;
    data5 PT_LOAD;
    data6 PT_LOAD;
    data7 PT_LOAD;
    bss PT_LOAD;
}
SECTIONS
{
    .text0 0x80003100 : { *(.text0) } : text0
    .text1 0x80005600 : { *(.text1) } : text1
    .data0 0x80005540 : { *(.data0) } : data0
    .data1 0x800055A0 : { *(.data1) } : data1
    .data2 0x8036FBA0 : { *(.data2) } : data2
    .data3 0x8036FF80 : { *(.data3) } : data3
    .data4 0x8036FFA0 : { *(.data4) } : data4
    .data5 0x803A8380 : { *(.data5) } : data5
    .data6 0x80408AC0 : { *(.data6) } : data6
    .data7 0x8040B460 : { *(.data7) } : data7
    .bss 0x803E6000 (NOLOAD) : { *(.bss) } : bss
}
