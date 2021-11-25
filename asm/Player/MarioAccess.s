.include "macros.inc"

.section .text, "ax"  # 0x80005600 - 0x8036FBA0

# .global SMS_SetMarioAccessParams__Fv
# SMS_SetMarioAccessParams__Fv:
# /* 80152D58 0014FC98  81 0D 98 B8 */    lwz r8, gpMarioOriginal@sda21(r13)
# /* 80152D5C 0014FC9C  38 A8 00 94 */    addi r5, r8, 0x94
# /* 80152D60 0014FCA0  91 0D 98 D8 */    stw r8, gpMarioAddress@sda21(r13)
# /* 80152D64 0014FCA4  39 28 00 A4 */    addi r9, r8, 0xa4
# /* 80152D68 0014FCA8  38 88 00 10 */    addi r4, r8, 0x10
# /* 80152D6C 0014FCAC  90 AD 98 E0 */    stw r5, gpMarioAngleX@sda21(r13)
# /* 80152D70 0014FCB0  38 65 00 02 */    addi r3, r5, 2
# /* 80152D74 0014FCB4  38 05 00 04 */    addi r0, r5, 4
# /* 80152D78 0014FCB8  90 8D 98 DC */    stw r4, gpMarioPos@sda21(r13)
# /* 80152D7C 0014FCBC  38 E9 00 04 */    addi r7, r9, 4
# /* 80152D80 0014FCC0  38 C9 00 08 */    addi r6, r9, 8
# /* 80152D84 0014FCC4  90 6D 98 E4 */    stw r3, gpMarioAngleY@sda21(r13)
# /* 80152D88 0014FCC8  38 A8 00 F8 */    addi r5, r8, 0xf8
# /* 80152D8C 0014FCCC  38 88 01 18 */    addi r4, r8, 0x118
# /* 80152D90 0014FCD0  90 0D 98 E8 */    stw r0, gpMarioAngleZ@sda21(r13)
# /* 80152D94 0014FCD4  38 68 08 20 */    addi r3, r8, 0x820
# /* 80152D98 0014FCD8  38 08 00 E0 */    addi r0, r8, 0xe0
# /* 80152D9C 0014FCDC  91 2D 98 EC */    stw r9, gpMarioSpeedX@sda21(r13)
# /* 80152DA0 0014FCE0  90 ED 98 F0 */    stw r7, gpMarioSpeedY@sda21(r13)
# /* 80152DA4 0014FCE4  90 CD 98 F4 */    stw r6, gpMarioSpeedZ@sda21(r13)
# /* 80152DA8 0014FCE8  90 AD 98 F8 */    stw r5, gpMarioLightID@sda21(r13)
# /* 80152DAC 0014FCEC  90 8D 98 FC */    stw r4, gpMarioFlag@sda21(r13)
# /* 80152DB0 0014FCF0  90 6D 99 00 */    stw r3, gpMarioThrowPower@sda21(r13)
# /* 80152DB4 0014FCF4  90 0D 99 04 */    stw r0, gpMarioGroundPlane@sda21(r13)
# /* 80152DB8 0014FCF8  4E 80 00 20 */    blr 

# .section .sdata2, "a"  # 0x8040B460 - 0x80414020
# .balign 8
# $$22019:
#     .incbin "baserom.dol", 0x3E5E98, 0x8

.section .sbss, "wa"  # 0x804097C0 - 0x8040B45C
# .global gpMarioAddress
# gpMarioAddress:
#     .skip 0x4
# .global gpMarioPos
# gpMarioPos:
#     .skip 0x4
# .global gpMarioAngleX
# gpMarioAngleX:
#     .skip 0x4
# .global gpMarioAngleY
# gpMarioAngleY:
#     .skip 0x4
# .global gpMarioAngleZ
# gpMarioAngleZ:
#     .skip 0x4
# .global gpMarioSpeedX
# gpMarioSpeedX:
#     .skip 0x4
# .global gpMarioSpeedY
# gpMarioSpeedY:
#     .skip 0x4
# .global gpMarioSpeedZ
# gpMarioSpeedZ:
#     .skip 0x4
# .global gpMarioLightID
# gpMarioLightID:
#     .skip 0x4
# .global gpMarioFlag
# gpMarioFlag:
#     .skip 0x4
# .global gpMarioThrowPower
# gpMarioThrowPower:
#     .skip 0x4
# .global gpMarioGroundPlane
# gpMarioGroundPlane:
#     .skip 0x4
