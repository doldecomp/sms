.include "macros.inc"

.section .rodata, "a"  # 0x8036FFA0 - 0x803A8380
.balign 8
"@1490":
  .4byte 0
  .4byte 0
  .4byte 0
"@1526":
	.4byte 0x83818382
	.4byte 0x838A82AA
	.4byte 0x91AB82E8
	.4byte 0x82DC82B9
	.4byte 0x82F10A00
"@1536":
	.asciz "/Camera/shakeHipDrop.prm"
	.balign 4
"@1537":
	.asciz "/Camera/shakeWallJump.prm"
	.balign 4
"@1538":
	.asciz "/Camera/shakeDamage.prm"
"@1539":
	.asciz "/Camera/shakeEnemy.prm"
	.balign 4
"@1540":
	.asciz "/Camera/shakeEnemy2.prm"
"@1541":
	.asciz "/Camera/shakeBuildingAppear.prm"
"@1542":
	.asciz "/Camera/shakeKiller.prm"
"@1543":
	.asciz "/Camera/shakeTamanoko.prm"
	.balign 4
"@1544":
	.asciz "/Camera/shakeBohaSandCrash.prm"
	.balign 4
"@1545":
	.asciz "/Camera/shakeBohaSlip.prm"
	.balign 4
"@1546":
	.asciz "/Camera/shakeBohaStep.prm"
	.balign 4
"@1547":
	.asciz "/Camera/shakeBohaRun.prm"
	.balign 4
"@1548":
	.asciz "/Camera/shakeBohaGetup.prm"
	.balign 4
"@1549":
	.asciz "/Camera/shakeSandBombAppear.prm"
"@1550":
	.asciz "/Camera/shakeBopaDown.prm"
	.balign 4
"@1551":
	.asciz "/Camera/shakeBopaPopo.prm"
	.balign 4
"@1552":
	.asciz "/Camera/shakeBopaGetup.prm"
	.balign 4
"@1553":
	.asciz "/Camera/shakeBopaDownLoop.prm"
	.balign 4
"@1554":
	.asciz "/Camera/shakeBogeBeakHit.prm"
	.balign 4
"@1555":
	.asciz "/Camera/shakeBogeTumble.prm"
"@1556":
	.asciz "/Camera/shakeBogeLand.prm"
	.balign 4
"@1557":
	.asciz "/Camera/shakeBogeTeHit.prm"
	.balign 4
"@1558":
	.asciz "/Camera/shakeBowaLand1.prm"
	.balign 4
"@1559":
	.asciz "/Camera/shakeBowaLand2.prm"
	.balign 4
"@1560":
	.asciz "/Camera/shakeBEelRoll.prm"
	.balign 4
"@1561":
	.asciz "/Camera/shakeBEelOut.prm"
	.balign 4
"@1562":
	.asciz "/Camera/shakeBEelHanukeIn.prm"
	.balign 4
"@1563":
	.asciz "/Camera/shakeBEelSuckOpen.prm"
	.balign 4
"@1564":
	.asciz "/Camera/shakeBEelSuckLoop.prm"
	.balign 4
"@1565":
	.asciz "/Camera/shakeBEelSuckClose.prm"
	.balign 4
"@1566":
	.asciz "/Camera/shakeBEelDie.prm"
	.balign 4
"@1567":
	.asciz "/Camera/shakeBTelesaDamage.prm"
	.balign 4
"@1568":
	.asciz "/Camera/shakeBTelesaHit.prm"
"@1569":
	.asciz "/Camera/shakeBTelesaDown.prm"
	.balign 4
"@1570":
	.asciz "/Camera/shakeBTelesaAppear.prm"
	.balign 4
"@1571":
	.asciz "/Camera/shakeBTelesaRoll.prm"
	.balign 4
"@1572":
	.asciz "/Camera/shakeCannonDemo.prm"
"@1573":
	.asciz "/Camera/shakeBathtubGripBreaking.prm"
	.balign 4
"@1574":
	.asciz "/Camera/shakeBathtubGripBreaking2.prm"
	.balign 4
"@1575":
	.asciz "/Camera/shakeKoopaHipdrop.prm"
	.balign 4
"@1576":
	.asciz "/Camera/shakeBathtubKiller.prm"
	.balign 4

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
