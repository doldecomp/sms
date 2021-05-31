.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
rumbleFrame_00:
	.incbin "baserom.dol", 0x3AFB60, 0x2C
rumblePower_00:
	.incbin "baserom.dol", 0x3AFB8C, 0x2C
rumbleFrame_01:
	.incbin "baserom.dol", 0x3AFBB8, 0x60
rumblePower_01:
	.incbin "baserom.dol", 0x3AFC18, 0x60
rumbleFrame_04:
	.incbin "baserom.dol", 0x3AFC78, 0x1C
rumblePower_04:
	.incbin "baserom.dol", 0x3AFC94, 0x1C
rumbleFrame_05:
	.incbin "baserom.dol", 0x3AFCB0, 0x54
rumblePower_05:
	.incbin "baserom.dol", 0x3AFD04, 0x54
rumbleFrame_06:
	.incbin "baserom.dol", 0x3AFD58, 0x28
rumblePower_06:
	.incbin "baserom.dol", 0x3AFD80, 0x28
rumbleFrame_07:
	.incbin "baserom.dol", 0x3AFDA8, 0x50
rumblePower_07:
	.incbin "baserom.dol", 0x3AFDF8, 0x50
rumbleFrame_08:
	.incbin "baserom.dol", 0x3AFE48, 0x14
rumblePower_08:
	.incbin "baserom.dol", 0x3AFE5C, 0x14
rumbleFrame_09:
	.incbin "baserom.dol", 0x3AFE70, 0x10
rumblePower_09:
	.incbin "baserom.dol", 0x3AFE80, 0x10
rumbleFrame_19:
	.incbin "baserom.dol", 0x3AFE90, 0x14
rumblePower_19:
	.incbin "baserom.dol", 0x3AFEA4, 0x14
rumbleFrame_23:
	.incbin "baserom.dol", 0x3AFEB8, 0x134
rumblePower_23:
	.incbin "baserom.dol", 0x3AFFEC, 0x134
rumbleFrame_24:
	.incbin "baserom.dol", 0x3B0120, 0x94
rumblePower_24:
	.incbin "baserom.dol", 0x3B01B4, 0x94
.global channelDataTbl
channelDataTbl:
  .4byte rumblePoint_00
  .4byte rumbleFrame_00
  .4byte rumblePower_00
  .4byte rumblePoint_01
  .4byte rumbleFrame_01
  .4byte rumblePower_01
  .4byte rumblePoint_02
  .4byte rumbleFrame_02
  .4byte rumblePower_02
  .4byte rumblePoint_03
  .4byte rumbleFrame_03
  .4byte rumblePower_03
  .4byte rumblePoint_04
  .4byte rumbleFrame_04
  .4byte rumblePower_04
  .4byte rumblePoint_05
  .4byte rumbleFrame_05
  .4byte rumblePower_05
  .4byte rumblePoint_06
  .4byte rumbleFrame_06
  .4byte rumblePower_06
  .4byte rumblePoint_07
  .4byte rumbleFrame_07
  .4byte rumblePower_07
  .4byte rumblePoint_08
  .4byte rumbleFrame_08
  .4byte rumblePower_08
  .4byte rumblePoint_09
  .4byte rumbleFrame_09
  .4byte rumblePower_09
  .4byte rumblePoint_10
  .4byte rumbleFrame_10
  .4byte rumblePower_10
  .4byte rumblePoint_11
  .4byte rumbleFrame_11
  .4byte rumblePower_11
  .4byte rumblePoint_12
  .4byte rumbleFrame_12
  .4byte rumblePower_12
  .4byte rumblePoint_13
  .4byte rumbleFrame_13
  .4byte rumblePower_13
  .4byte rumblePoint_14
  .4byte rumbleFrame_14
  .4byte rumblePower_14
  .4byte rumblePoint_15
  .4byte rumbleFrame_15
  .4byte rumblePower_15
  .4byte rumblePoint_16
  .4byte rumbleFrame_16
  .4byte rumblePower_16
  .4byte rumblePoint_17
  .4byte rumbleFrame_17
  .4byte rumblePower_17
  .4byte rumblePoint_18
  .4byte rumbleFrame_18
  .4byte rumblePower_18
  .4byte rumblePoint_19
  .4byte rumbleFrame_19
  .4byte rumblePower_19
  .4byte rumblePoint_20
  .4byte rumbleFrame_20
  .4byte rumblePower_20
  .4byte rumblePoint_21
  .4byte rumbleFrame_21
  .4byte rumblePower_21
  .4byte rumblePoint_22
  .4byte rumbleFrame_22
  .4byte rumblePower_22
  .4byte rumblePoint_23
  .4byte rumbleFrame_23
  .4byte rumblePower_23
  .4byte rumblePoint_24
  .4byte rumbleFrame_24
  .4byte rumblePower_24
  .4byte 0

.section .sdata, "wa"  # 0x80408AC0 - 0x804097C0
rumblePoint_00:
	.incbin "baserom.dol", 0x3E3340, 0x4
rumblePoint_01:
	.incbin "baserom.dol", 0x3E3344, 0x4
rumblePoint_02:
	.incbin "baserom.dol", 0x3E3348, 0x4
rumbleFrame_02:
	.incbin "baserom.dol", 0x3E334C, 0x8
rumblePower_02:
	.incbin "baserom.dol", 0x3E3354, 0x8
rumblePoint_03:
	.incbin "baserom.dol", 0x3E335C, 0x4
rumbleFrame_03:
	.incbin "baserom.dol", 0x3E3360, 0x8
rumblePower_03:
	.incbin "baserom.dol", 0x3E3368, 0x8
rumblePoint_04:
	.incbin "baserom.dol", 0x3E3370, 0x4
rumblePoint_05:
	.incbin "baserom.dol", 0x3E3374, 0x4
rumblePoint_06:
	.incbin "baserom.dol", 0x3E3378, 0x4
rumblePoint_07:
	.incbin "baserom.dol", 0x3E337C, 0x4
rumblePoint_08:
	.incbin "baserom.dol", 0x3E3380, 0x4
rumblePoint_09:
	.incbin "baserom.dol", 0x3E3384, 0x4
rumblePoint_10:
	.incbin "baserom.dol", 0x3E3388, 0x4
rumbleFrame_10:
  .4byte 0
rumblePower_10:
  .4byte 0
rumblePoint_11:
	.incbin "baserom.dol", 0x3E3394, 0x4
rumbleFrame_11:
  .4byte 0
rumblePower_11:
  .4byte 0
rumblePoint_12:
	.incbin "baserom.dol", 0x3E33A0, 0x4
rumbleFrame_12:
  .4byte 0
rumblePower_12:
  .4byte 0
rumblePoint_13:
	.incbin "baserom.dol", 0x3E33AC, 0x4
rumbleFrame_13:
  .4byte 0
rumblePower_13:
  .4byte 0
rumblePoint_14:
	.incbin "baserom.dol", 0x3E33B8, 0x4
rumbleFrame_14:
  .4byte 0
rumblePower_14:
  .4byte 0
rumblePoint_15:
	.incbin "baserom.dol", 0x3E33C4, 0x4
rumbleFrame_15:
  .4byte 0
rumblePower_15:
  .4byte 0
rumblePoint_16:
	.incbin "baserom.dol", 0x3E33D0, 0x4
rumbleFrame_16:
  .4byte 0
rumblePower_16:
  .4byte 0
rumblePoint_17:
	.incbin "baserom.dol", 0x3E33DC, 0x4
rumbleFrame_17:
  .4byte 0
rumblePower_17:
  .4byte 0
rumblePoint_18:
	.incbin "baserom.dol", 0x3E33E8, 0x4
rumbleFrame_18:
  .4byte 0
rumblePower_18:
  .4byte 0
rumblePoint_19:
	.incbin "baserom.dol", 0x3E33F4, 0x4
rumblePoint_20:
	.incbin "baserom.dol", 0x3E33F8, 0x4
rumbleFrame_20:
	.incbin "baserom.dol", 0x3E33FC, 0x8
rumblePower_20:
	.incbin "baserom.dol", 0x3E3404, 0x8
rumblePoint_21:
	.incbin "baserom.dol", 0x3E340C, 0x4
rumbleFrame_21:
	.incbin "baserom.dol", 0x3E3410, 0x8
rumblePower_21:
	.incbin "baserom.dol", 0x3E3418, 0x8
rumblePoint_22:
	.incbin "baserom.dol", 0x3E3420, 0x4
rumbleFrame_22:
	.incbin "baserom.dol", 0x3E3424, 0x8
rumblePower_22:
	.incbin "baserom.dol", 0x3E342C, 0x8
rumblePoint_23:
	.incbin "baserom.dol", 0x3E3434, 0x4
rumblePoint_24:
	.incbin "baserom.dol", 0x3E3438, 0x4
.global channelNum
channelNum:
	.incbin "baserom.dol", 0x3E343C, 0x4
