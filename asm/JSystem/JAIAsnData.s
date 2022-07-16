.include "macros.inc"

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@395":
	.asciz "JaiInfo.asn"
"@415":
	.4byte 0x4A414941
	.4byte 0x736E4461
	.4byte 0x74613A3A
	.4byte 0x72656164
	.4byte 0x41736E46
	.4byte 0x696C6520
	.4byte 0x3A204153
	.4byte 0x4E204649
	.4byte 0x4C452825
	.4byte 0x73292082
	.4byte 0xAA8CA982
	.4byte 0xC282A982
	.4byte 0xE882DC82
	.4byte 0xB982F181
	.4byte 0x420A0000
"@416":
	.asciz "JAIAsnData.cpp"
	.balign 4
"@417":
	.4byte 0x4A414941
	.4byte 0x736E4461
	.4byte 0x74613A3A
	.4byte 0x72656164
	.4byte 0x41736E46
	.4byte 0x696C6520
	.4byte 0x3A204153
	.4byte 0x4E204649
	.4byte 0x4C452825
	.4byte 0x732982CC
	.4byte 0x83548343
	.4byte 0x83592830
	.4byte 0x78257829
	.4byte 0x82AA3332
	.4byte 0x836F8343
	.4byte 0x83678341
	.4byte 0x83898343
	.4byte 0x83818393
	.4byte 0x836782B3
	.4byte 0x82EA82C4
	.4byte 0x82A282DC
	.4byte 0x82B982F1
	.4byte 0x81498149
	.4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global asnFileName__Q27JAInter8TAsnData
asnFileName__Q27JAInter8TAsnData:
  .4byte "@395"
  .4byte 0

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
.global asnData__Q27JAInter8TAsnData
asnData__Q27JAInter8TAsnData:
	.skip 0x8
