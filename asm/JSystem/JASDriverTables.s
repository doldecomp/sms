.include "macros.inc"

.section .data, "wa"  # 0x803A8380 - 0x803E6000
    .balign 32
.global C5BASE_PITCHTABLE__Q28JASystem6Driver
C5BASE_PITCHTABLE__Q28JASystem6Driver:
	.incbin "baserom.dol", 0x3A8000, 0x200
.global DSPADPCM_FILTER__Q28JASystem12DSPInterface
DSPADPCM_FILTER__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3A8200, 0x40
.global DSPRES_FILTER__Q28JASystem12DSPInterface
DSPRES_FILTER__Q28JASystem12DSPInterface:
	.incbin "baserom.dol", 0x3A8240, 0x500
