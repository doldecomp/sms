.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
.global rumbleFrame_00
rumbleFrame_00:
	.incbin "baserom.dol", 0x3AFB60, 0x2C
.global rumblePower_00
rumblePower_00:
	.incbin "baserom.dol", 0x3AFB8C, 0x2C
.global rumbleFrame_01
rumbleFrame_01:
	.incbin "baserom.dol", 0x3AFBB8, 0x60
.global rumblePower_01
rumblePower_01:
	.incbin "baserom.dol", 0x3AFC18, 0x60
.global rumbleFrame_04
rumbleFrame_04:
	.incbin "baserom.dol", 0x3AFC78, 0x1C
.global rumblePower_04
rumblePower_04:
	.incbin "baserom.dol", 0x3AFC94, 0x1C
.global rumbleFrame_05
rumbleFrame_05:
	.incbin "baserom.dol", 0x3AFCB0, 0x54
.global rumblePower_05
rumblePower_05:
	.incbin "baserom.dol", 0x3AFD04, 0x54
.global rumbleFrame_06
rumbleFrame_06:
	.incbin "baserom.dol", 0x3AFD58, 0x28
.global rumblePower_06
rumblePower_06:
	.incbin "baserom.dol", 0x3AFD80, 0x28
.global rumbleFrame_07
rumbleFrame_07:
	.incbin "baserom.dol", 0x3AFDA8, 0x50
.global rumblePower_07
rumblePower_07:
	.incbin "baserom.dol", 0x3AFDF8, 0x50
.global rumbleFrame_08
rumbleFrame_08:
	.incbin "baserom.dol", 0x3AFE48, 0x14
.global rumblePower_08
rumblePower_08:
	.incbin "baserom.dol", 0x3AFE5C, 0x14
.global rumbleFrame_09
rumbleFrame_09:
	.incbin "baserom.dol", 0x3AFE70, 0x10
.global rumblePower_09
rumblePower_09:
	.incbin "baserom.dol", 0x3AFE80, 0x10
.global rumbleFrame_19
rumbleFrame_19:
	.incbin "baserom.dol", 0x3AFE90, 0x14
.global rumblePower_19
rumblePower_19:
	.incbin "baserom.dol", 0x3AFEA4, 0x14
.global rumbleFrame_23
rumbleFrame_23:
	.incbin "baserom.dol", 0x3AFEB8, 0x134
.global rumblePower_23
rumblePower_23:
	.incbin "baserom.dol", 0x3AFFEC, 0x134
.global rumbleFrame_24
rumbleFrame_24:
	.incbin "baserom.dol", 0x3B0120, 0x94
.global rumblePower_24
rumblePower_24:
	.incbin "baserom.dol", 0x3B01B4, 0x94
.global channelDataTbl
channelDataTbl:
	.incbin "baserom.dol", 0x3B0248, 0x130

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
.global rumblePoint_00
rumblePoint_00:
	.incbin "baserom.dol", 0x3E3340, 0x4
.global rumblePoint_01
rumblePoint_01:
	.incbin "baserom.dol", 0x3E3344, 0x4
.global rumblePoint_02
rumblePoint_02:
	.incbin "baserom.dol", 0x3E3348, 0x4
.global rumbleFrame_02
rumbleFrame_02:
	.incbin "baserom.dol", 0x3E334C, 0x8
.global rumblePower_02
rumblePower_02:
	.incbin "baserom.dol", 0x3E3354, 0x8
.global rumblePoint_03
rumblePoint_03:
	.incbin "baserom.dol", 0x3E335C, 0x4
.global rumbleFrame_03
rumbleFrame_03:
	.incbin "baserom.dol", 0x3E3360, 0x8
.global rumblePower_03
rumblePower_03:
	.incbin "baserom.dol", 0x3E3368, 0x8
.global rumblePoint_04
rumblePoint_04:
	.incbin "baserom.dol", 0x3E3370, 0x4
.global rumblePoint_05
rumblePoint_05:
	.incbin "baserom.dol", 0x3E3374, 0x4
.global rumblePoint_06
rumblePoint_06:
	.incbin "baserom.dol", 0x3E3378, 0x4
.global rumblePoint_07
rumblePoint_07:
	.incbin "baserom.dol", 0x3E337C, 0x4
.global rumblePoint_08
rumblePoint_08:
	.incbin "baserom.dol", 0x3E3380, 0x4
.global rumblePoint_09
rumblePoint_09:
	.incbin "baserom.dol", 0x3E3384, 0x4
.global rumblePoint_10
rumblePoint_10:
	.incbin "baserom.dol", 0x3E3388, 0x4
.global rumbleFrame_10
rumbleFrame_10:
	.incbin "baserom.dol", 0x3E338C, 0x4
.global rumblePower_10
rumblePower_10:
	.incbin "baserom.dol", 0x3E3390, 0x4
.global rumblePoint_11
rumblePoint_11:
	.incbin "baserom.dol", 0x3E3394, 0x4
.global rumbleFrame_11
rumbleFrame_11:
	.incbin "baserom.dol", 0x3E3398, 0x4
.global rumblePower_11
rumblePower_11:
	.incbin "baserom.dol", 0x3E339C, 0x4
.global rumblePoint_12
rumblePoint_12:
	.incbin "baserom.dol", 0x3E33A0, 0x4
.global rumbleFrame_12
rumbleFrame_12:
	.incbin "baserom.dol", 0x3E33A4, 0x4
.global rumblePower_12
rumblePower_12:
	.incbin "baserom.dol", 0x3E33A8, 0x4
.global rumblePoint_13
rumblePoint_13:
	.incbin "baserom.dol", 0x3E33AC, 0x4
.global rumbleFrame_13
rumbleFrame_13:
	.incbin "baserom.dol", 0x3E33B0, 0x4
.global rumblePower_13
rumblePower_13:
	.incbin "baserom.dol", 0x3E33B4, 0x4
.global rumblePoint_14
rumblePoint_14:
	.incbin "baserom.dol", 0x3E33B8, 0x4
.global rumbleFrame_14
rumbleFrame_14:
	.incbin "baserom.dol", 0x3E33BC, 0x4
.global rumblePower_14
rumblePower_14:
	.incbin "baserom.dol", 0x3E33C0, 0x4
.global rumblePoint_15
rumblePoint_15:
	.incbin "baserom.dol", 0x3E33C4, 0x4
.global rumbleFrame_15
rumbleFrame_15:
	.incbin "baserom.dol", 0x3E33C8, 0x4
.global rumblePower_15
rumblePower_15:
	.incbin "baserom.dol", 0x3E33CC, 0x4
.global rumblePoint_16
rumblePoint_16:
	.incbin "baserom.dol", 0x3E33D0, 0x4
.global rumbleFrame_16
rumbleFrame_16:
	.incbin "baserom.dol", 0x3E33D4, 0x4
.global rumblePower_16
rumblePower_16:
	.incbin "baserom.dol", 0x3E33D8, 0x4
.global rumblePoint_17
rumblePoint_17:
	.incbin "baserom.dol", 0x3E33DC, 0x4
.global rumbleFrame_17
rumbleFrame_17:
	.incbin "baserom.dol", 0x3E33E0, 0x4
.global rumblePower_17
rumblePower_17:
	.incbin "baserom.dol", 0x3E33E4, 0x4
.global rumblePoint_18
rumblePoint_18:
	.incbin "baserom.dol", 0x3E33E8, 0x4
.global rumbleFrame_18
rumbleFrame_18:
	.incbin "baserom.dol", 0x3E33EC, 0x4
.global rumblePower_18
rumblePower_18:
	.incbin "baserom.dol", 0x3E33F0, 0x4
.global rumblePoint_19
rumblePoint_19:
	.incbin "baserom.dol", 0x3E33F4, 0x4
.global rumblePoint_20
rumblePoint_20:
	.incbin "baserom.dol", 0x3E33F8, 0x4
.global rumbleFrame_20
rumbleFrame_20:
	.incbin "baserom.dol", 0x3E33FC, 0x8
.global rumblePower_20
rumblePower_20:
	.incbin "baserom.dol", 0x3E3404, 0x8
.global rumblePoint_21
rumblePoint_21:
	.incbin "baserom.dol", 0x3E340C, 0x4
.global rumbleFrame_21
rumbleFrame_21:
	.incbin "baserom.dol", 0x3E3410, 0x8
.global rumblePower_21
rumblePower_21:
	.incbin "baserom.dol", 0x3E3418, 0x8
.global rumblePoint_22
rumblePoint_22:
	.incbin "baserom.dol", 0x3E3420, 0x4
.global rumbleFrame_22
rumbleFrame_22:
	.incbin "baserom.dol", 0x3E3424, 0x8
.global rumblePower_22
rumblePower_22:
	.incbin "baserom.dol", 0x3E342C, 0x8
.global rumblePoint_23
rumblePoint_23:
	.incbin "baserom.dol", 0x3E3434, 0x4
.global rumblePoint_24
rumblePoint_24:
	.incbin "baserom.dol", 0x3E3438, 0x4
.global channelNum
channelNum:
	.incbin "baserom.dol", 0x3E343C, 0x4
