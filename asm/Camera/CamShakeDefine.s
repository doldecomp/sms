.include "macros.inc"

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@1526":
	.incbin "baserom.dol", 0x3A44C4, 0x14
"@1536":
	.incbin "baserom.dol", 0x3A44D8, 0x1C
"@1537":
	.incbin "baserom.dol", 0x3A44F4, 0x1C
"@1538":
	.incbin "baserom.dol", 0x3A4510, 0x18
"@1539":
	.incbin "baserom.dol", 0x3A4528, 0x18
"@1540":
	.incbin "baserom.dol", 0x3A4540, 0x18
"@1541":
	.incbin "baserom.dol", 0x3A4558, 0x20
"@1542":
	.incbin "baserom.dol", 0x3A4578, 0x18
"@1543":
	.incbin "baserom.dol", 0x3A4590, 0x1C
"@1544":
	.incbin "baserom.dol", 0x3A45AC, 0x20
"@1545":
	.incbin "baserom.dol", 0x3A45CC, 0x1C
"@1546":
	.incbin "baserom.dol", 0x3A45E8, 0x1C
"@1547":
	.incbin "baserom.dol", 0x3A4604, 0x1C
"@1548":
	.incbin "baserom.dol", 0x3A4620, 0x1C
"@1549":
	.incbin "baserom.dol", 0x3A463C, 0x20
"@1550":
	.incbin "baserom.dol", 0x3A465C, 0x1C
"@1551":
	.incbin "baserom.dol", 0x3A4678, 0x1C
"@1552":
	.incbin "baserom.dol", 0x3A4694, 0x1C
"@1553":
	.incbin "baserom.dol", 0x3A46B0, 0x20
"@1554":
	.incbin "baserom.dol", 0x3A46D0, 0x20
"@1555":
	.incbin "baserom.dol", 0x3A46F0, 0x1C
"@1556":
	.incbin "baserom.dol", 0x3A470C, 0x1C
"@1557":
	.incbin "baserom.dol", 0x3A4728, 0x1C
"@1558":
	.incbin "baserom.dol", 0x3A4744, 0x1C
"@1559":
	.incbin "baserom.dol", 0x3A4760, 0x1C
"@1560":
	.incbin "baserom.dol", 0x3A477C, 0x1C
"@1561":
	.incbin "baserom.dol", 0x3A4798, 0x1C
"@1562":
	.incbin "baserom.dol", 0x3A47B4, 0x20
"@1563":
	.incbin "baserom.dol", 0x3A47D4, 0x20
"@1564":
	.incbin "baserom.dol", 0x3A47F4, 0x20
"@1565":
	.incbin "baserom.dol", 0x3A4814, 0x20
"@1566":
	.incbin "baserom.dol", 0x3A4834, 0x1C
"@1567":
	.incbin "baserom.dol", 0x3A4850, 0x20
"@1568":
	.incbin "baserom.dol", 0x3A4870, 0x1C
"@1569":
	.incbin "baserom.dol", 0x3A488C, 0x20
"@1570":
	.incbin "baserom.dol", 0x3A48AC, 0x20
"@1571":
	.incbin "baserom.dol", 0x3A48CC, 0x20
"@1572":
	.incbin "baserom.dol", 0x3A48EC, 0x1C
"@1573":
	.incbin "baserom.dol", 0x3A4908, 0x28
"@1574":
	.incbin "baserom.dol", 0x3A4930, 0x28
"@1575":
	.incbin "baserom.dol", 0x3A4958, 0x20
"@1576":
	.incbin "baserom.dol", 0x3A4978, 0x20

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 8
.global mCamShakeNameSave__12TCameraShake
mCamShakeNameSave__12TCameraShake:
  .4byte "@1536"
  .4byte "@1537"
  .4byte "@1538"
  .4byte "@1539"
  .4byte "@1540"
  .4byte "@1541"
  .4byte "@1542"
  .4byte "@1543"
  .4byte "@1544"
  .4byte "@1545"
  .4byte "@1546"
  .4byte "@1547"
  .4byte "@1548"
  .4byte "@1549"
  .4byte "@1550"
  .4byte "@1551"
  .4byte "@1552"
  .4byte "@1553"
  .4byte "@1554"
  .4byte "@1555"
  .4byte "@1556"
  .4byte "@1557"
  .4byte "@1558"
  .4byte "@1559"
  .4byte "@1560"
  .4byte "@1561"
  .4byte "@1562"
  .4byte "@1563"
  .4byte "@1564"
  .4byte "@1565"
  .4byte "@1566"
  .4byte "@1567"
  .4byte "@1568"
  .4byte "@1569"
  .4byte "@1570"
  .4byte "@1571"
  .4byte "@1572"
  .4byte "@1573"
  .4byte "@1574"
  .4byte "@1575"
  .4byte "@1576"
  .4byte 0
