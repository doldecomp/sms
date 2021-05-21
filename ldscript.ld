ENTRY(__start)
PHDRS
{
    init PT_LOAD;
    text PT_LOAD;
    extab_ PT_LOAD;
    extabindex_ PT_LOAD;
    ctors PT_LOAD;
    dtors PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    sdata PT_LOAD;
    sdata2 PT_LOAD;
    bss PT_LOAD;
    sbss PT_LOAD;
}
SECTIONS
{
    .init 0x80003100 : { *(.init) } : init
    extab_ 0x80005540 : { *(extab_) } : extab_
    extabindex_ 0x800055A0 : { *(extabindex_) } : extabindex_
    .text 0x80005600 : { *(.text) } : text
    .ctors 0x8036FBA0 : { *(.ctors) } : ctors
    .dtors 0x8036FF80 : { *(.dtors) } : dtors
    .rodata 0x8036FFA0 : { *(.rodata) } : rodata
    .data 0x803A8380 : { *(.data) } : data
    .bss 0x803E6000 (NOLOAD) : { *(.bss) } : bss
    .sdata 0x80408AC0 : { *(.sdata) } : sdata
    .sbss 0x804097C0 (NOLOAD) : { *(.sbss) } : sbss
    .sdata2 0x8040B460 : { *(.sdata2) } : sdata2
}
