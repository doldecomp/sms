.include "macros.inc"

.section .rodata, "wa"  # 0x8036FFA0 - 0x803A8380
$$21490:
  .4byte 0
  .4byte 0
  .4byte 0
$$21526:
	.incbin "baserom.dol", 0x3A44C4, 0x14
$$21536:
	.incbin "baserom.dol", 0x3A44D8, 0x1C
$$21537:
	.incbin "baserom.dol", 0x3A44F4, 0x1C
$$21538:
	.incbin "baserom.dol", 0x3A4510, 0x18
$$21539:
	.incbin "baserom.dol", 0x3A4528, 0x18
$$21540:
	.incbin "baserom.dol", 0x3A4540, 0x18
$$21541:
	.incbin "baserom.dol", 0x3A4558, 0x20
$$21542:
	.incbin "baserom.dol", 0x3A4578, 0x18
$$21543:
	.incbin "baserom.dol", 0x3A4590, 0x1C
$$21544:
	.incbin "baserom.dol", 0x3A45AC, 0x20
$$21545:
	.incbin "baserom.dol", 0x3A45CC, 0x1C
$$21546:
	.incbin "baserom.dol", 0x3A45E8, 0x1C
$$21547:
	.incbin "baserom.dol", 0x3A4604, 0x1C
$$21548:
	.incbin "baserom.dol", 0x3A4620, 0x1C
$$21549:
	.incbin "baserom.dol", 0x3A463C, 0x20
$$21550:
	.incbin "baserom.dol", 0x3A465C, 0x1C
$$21551:
	.incbin "baserom.dol", 0x3A4678, 0x1C
$$21552:
	.incbin "baserom.dol", 0x3A4694, 0x1C
$$21553:
	.incbin "baserom.dol", 0x3A46B0, 0x20
$$21554:
	.incbin "baserom.dol", 0x3A46D0, 0x20
$$21555:
	.incbin "baserom.dol", 0x3A46F0, 0x1C
$$21556:
	.incbin "baserom.dol", 0x3A470C, 0x1C
$$21557:
	.incbin "baserom.dol", 0x3A4728, 0x1C
$$21558:
	.incbin "baserom.dol", 0x3A4744, 0x1C
$$21559:
	.incbin "baserom.dol", 0x3A4760, 0x1C
$$21560:
	.incbin "baserom.dol", 0x3A477C, 0x1C
$$21561:
	.incbin "baserom.dol", 0x3A4798, 0x1C
$$21562:
	.incbin "baserom.dol", 0x3A47B4, 0x20
$$21563:
	.incbin "baserom.dol", 0x3A47D4, 0x20
$$21564:
	.incbin "baserom.dol", 0x3A47F4, 0x20
$$21565:
	.incbin "baserom.dol", 0x3A4814, 0x20
$$21566:
	.incbin "baserom.dol", 0x3A4834, 0x1C
$$21567:
	.incbin "baserom.dol", 0x3A4850, 0x20
$$21568:
	.incbin "baserom.dol", 0x3A4870, 0x1C
$$21569:
	.incbin "baserom.dol", 0x3A488C, 0x20
$$21570:
	.incbin "baserom.dol", 0x3A48AC, 0x20
$$21571:
	.incbin "baserom.dol", 0x3A48CC, 0x20
$$21572:
	.incbin "baserom.dol", 0x3A48EC, 0x1C
$$21573:
	.incbin "baserom.dol", 0x3A4908, 0x28
$$21574:
	.incbin "baserom.dol", 0x3A4930, 0x28
$$21575:
	.incbin "baserom.dol", 0x3A4958, 0x20
$$21576:
	.incbin "baserom.dol", 0x3A4978, 0x20

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 8
.global mCamShakeNameSave__12TCameraShake
mCamShakeNameSave__12TCameraShake:
  .4byte $$21536
  .4byte $$21537
  .4byte $$21538
  .4byte $$21539
  .4byte $$21540
  .4byte $$21541
  .4byte $$21542
  .4byte $$21543
  .4byte $$21544
  .4byte $$21545
  .4byte $$21546
  .4byte $$21547
  .4byte $$21548
  .4byte $$21549
  .4byte $$21550
  .4byte $$21551
  .4byte $$21552
  .4byte $$21553
  .4byte $$21554
  .4byte $$21555
  .4byte $$21556
  .4byte $$21557
  .4byte $$21558
  .4byte $$21559
  .4byte $$21560
  .4byte $$21561
  .4byte $$21562
  .4byte $$21563
  .4byte $$21564
  .4byte $$21565
  .4byte $$21566
  .4byte $$21567
  .4byte $$21568
  .4byte $$21569
  .4byte $$21570
  .4byte $$21571
  .4byte $$21572
  .4byte $$21573
  .4byte $$21574
  .4byte $$21575
  .4byte $$21576
  .4byte 0
