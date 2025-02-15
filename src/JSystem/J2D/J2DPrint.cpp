#include <JSystem/J2D/J2DPrint.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/gx.h>
#include <printf.h>
#include <stdlib.h>



OSMutex J2DPrint::mMutex;

char* J2DPrint::mStrBuff;
bool J2DPrint::mMutexInit;
bool J2DPrint::mHeapFlag;
u32 J2DPrint::mStrBuffSize;
u8 J2DPrint::mBufferNotEnough;

#pragma dont_inline on
extern "C" float J2DPrint_print_alpha_va(J2DPrint* print, u8 opacity,
                                         const char* format, va_list args)
{
	print->initchar();
	OSLockMutex(&J2DPrint::mMutex);
	int sz
	    = vsnprintf(J2DPrint::mStrBuff, J2DPrint::mStrBuffSize, format, args);
	if (sz < 0) {
		OSUnlockMutex(&J2DPrint::mMutex);
		return 0.0f;
	} else {
		if (sz >= J2DPrint::mStrBuffSize) {
			J2DPrint::mBufferNotEnough = true;
			sz                         = J2DPrint::mStrBuffSize - 1;
		}
		J2DPrint::TSize size;
		print->parse((u8*)J2DPrint::mStrBuff, sz, 0x7fffffff, nullptr, size,
		             opacity, true);
		OSUnlockMutex(&J2DPrint::mMutex);
		return size.unk0;
	}
}
#pragma dont_inline reset

J2DPrint::J2DPrint(JUTFont* font, int param_2)
{
	private_initiate(font, param_2, -0x80000000, JUtility::TColor(),
	                 JUtility::TColor());
}

J2DPrint::J2DPrint(JUTFont* font, int param_2, int param_3,
                   JUtility::TColor param_4, JUtility::TColor param_5)
{
	private_initiate(font, param_2, param_3, param_4, param_5);
}

J2DPrint::~J2DPrint() { }

void J2DPrint::initiate()
{
	if (!mFont)
		return;

	if (mFontSizeX == -0x80000000 && mFontSizeY == -0x80000000) {
		setFontSize();
	}
	mFont->setGX(unk3C, unk40);
}

void J2DPrint::private_initiate(JUTFont* font, int param_2, int param_3,
                                JUtility::TColor param_4,
                                JUtility::TColor param_5)
{
	if (!mStrBuff)
		setBuffer(0x400);

	if (!mMutexInit) {
		OSInitMutex(&mMutex);
		mMutexInit = true;
	}

	mFont = font;
	unk4C = param_2;
	unk50 = 0x20;
	if (mFont) {
		unk50 = param_3 != -0x80000000 ? param_3 : mFont->getLeading();
	}
	unk60 = 1;
	locate(0, 0);
	unk44 = param_4;
	unk48 = param_5;
	unk3C = 0;
	unk40 = 0xffffffff;
	if (!mFont)
		unk54 = 0x50;
	else
		unk54 = (s16)(mFont->getWidth() * 4);

	if (mFont) {
		setFontSize();
		mFont->setGX(unk3C, unk40);
	} else {
		mFontSizeX = -0x80000000;
		mFontSizeY = -0x80000000;
	}
	initchar();
}
char* J2DPrint::setBuffer(u32 size)
{
	char* old = mStrBuff;
	if (mHeapFlag)
		delete mStrBuff;

	mStrBuff     = new (JKRGetSystemHeap(), 0) char[size];
	mStrBuffSize = size;
	mHeapFlag    = true;
	return old;
}

void J2DPrint::setFontSize()
{
	if (mFont) {
		mFontSizeX = mFont->getWidth();
		mFontSizeY = mFont->getHeight();
	}
}

void J2DPrint::locate(int x, int y)
{
	unk1C    = x;
	unk20    = y;
	mCursorH = x;
	mCursorV = y;
	unk2C    = 0.0f;
}

void J2DPrint::print(int x, int y, const char* format, ...)
{
	locate(x, y);

	va_list args;
	va_start(args, format);

	char trash[0x4]; // additional inline indirection?
	J2DPrint_print_alpha_va(this, 0xff, format, args);

	va_end(args);
}

void J2DPrint::print(int x, int y, u8 opacity, const char* format, ...)
{
	locate(x, y);

	va_list args;
	va_start(args, format);

	char trash[0x4]; // additional inline indirection?
	J2DPrint_print_alpha_va(this, opacity, format, args);

	va_end(args);
}

float J2DPrint::getWidth(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	TSize size;

	OSLockMutex(&mMutex);
	s32 sz = vsnprintf(mStrBuff, mStrBuffSize, format, args);
	if (sz < 0 || sz > mStrBuffSize)
		sz = mStrBuffSize;
	parse((u8*)mStrBuff, sz, 0x7fffffff, nullptr, size, 0xff, false);
	OSUnlockMutex(&mMutex);

	va_end(args);
	return size.unk0;
}

void J2DPrint::printReturn(const char* param_1, int param_2, int param_3,
                           J2DTextBoxHBinding hbind, J2DTextBoxVBinding vbind,
                           int param_6, int param_7, u8 opacity)
{
	if (!mFont)
		return;

	initchar();
	unk1C = s32(mCursorH);
	unk20 = s32(mCursorV);

	size_t sVar6 = strlen(param_1);
	if (sVar6 >= mStrBuffSize) {
		sVar6            = mStrBuffSize - 1;
		mBufferNotEnough = 1;
	}

	u16 local_2b0[260];
	TSize aTStack_2b8;
	float dVar10 = parse((const u8*)param_1, sVar6, param_2, local_2b0,
	                     aTStack_2b8, opacity, false);
	float dVar13 = dVar10 + mFontSizeY;

	switch (vbind) {
	case VBIND_TOP:
		break;
	case VBIND_BOTTOM:
		param_7 += param_3 - (s32)(dVar13 + 0.5f);
		break;
	case VBIND_CENTER:
		param_7 += (param_3 - (s32)(dVar13 + 0.5f)) / 2;
	default:
		break;
	}

	for (int iVar8 = 0; local_2b0[iVar8] != 0xffff; iVar8++) {
		switch (hbind) {
		case HBIND_LEFT:
			local_2b0[iVar8] = 0;
			break;
		case HBIND_RIGHT:
			local_2b0[iVar8] = param_2 - local_2b0[iVar8];
			break;
		case HBIND_CENTER:
			local_2b0[iVar8] = (param_2 - local_2b0[iVar8]) / 2;
			break;
		}
	}

	initchar();
	mCursorH += param_6;
	mCursorV += param_7 + mFontSizeY;
	unk1C = s32(mCursorH);
	unk20 = s32(mCursorV);
	parse((const u8*)param_1, sVar6, param_2, local_2b0, aTStack_2b8, opacity,
	      true);
}
float J2DPrint::parse(const u8* param_1, int param_2, int param_3, u16* param_4,
                      J2DPrint::TSize& param_5, u8 opacity, bool param_7)
{
	if (mFont == nullptr) {
		return 0.0f;
	}

	const u8* local_c = param_1;
	u16 r21           = 0;
	f32 f26           = mCursorH;
	f32 f25           = mCursorV;
	f32 f24           = 0.0f;
	f32 f23           = f24;
	int r27           = *(param_1++);

	f32 f22 = mCursorH;
	f32 f21 = mCursorH;
	f32 f20 = mCursorV;
	f32 f19 = mCursorV;

	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);

	JUtility::TColor local_b8 = unk8;
	JUtility::TColor local_bc = unkC;

	local_b8.a = local_b8.a * opacity / 0xff;
	local_bc.a = local_bc.a * opacity / 0xff;
	JUtility::TColor* local_d8;
	if (unk18) {
		local_d8 = &local_bc;
	} else {
		local_d8 = &local_b8;
	}
	mFont->setGradColor(local_b8, *local_d8);
	bool r19 = false;
	bool r18;
	bool r17;
	do {
		r18 = false;
		if (mFont->isLeadByte(r27)) {
			r27 = (r27 << 8) | (*(param_1++));
			r18 = true;
		}

		if (r27 == 0 || ((u32)param_1 - (u32)local_c) > param_2) {
			if (param_7 == 0 && param_4 != nullptr) {
				param_4[r21] = 0.5f + f24;
			}
			r21++;
			break;
		}

		r17     = true;
		f32 f18 = mCursorH;
		if (r27 < 0x20) {
			if (r27 == 0x1b) {
				u16 r3 = doEscapeCode(&param_1, opacity);
				if (r3 == 'HM') {
					if ((param_7 == 0) && (param_4 != nullptr)) {
						param_4[r21] = 0.5f + f24;
					}
					mCursorH = f26;
					mCursorV = f25;
					r21++;
					if (r21 == 0x100) {
						break;
					}
					r19 = false;
					f24 = 0.0f;
				}
				if (r3) {
					r17 = false;
				}
			} else {
				doCtrlCode(r27);
				r17 = false;
				if (r27 == 10) {
					if ((!param_7) && (param_4 != nullptr)) {
						param_4[r21] = 0.5f + f24;
					}
					r21++;
					if (r21 == 0x100) {
						break;
					}
					r19 = false;
					f24 = 0.0f;
				} else if ((u32)r27 - 8 <= 1) {
					r19 = true;
				} else if (r27 == 13) {
					r19 = false;
				}
			}
		} else if (r18 && ((u32)param_1 - (u32)local_c > (u32)param_2)) {
			if ((!param_7) && (param_4 != nullptr)) {
				param_4[r21] = 0.5f + f24;
			}
			r21++;
			break;
		} else {
			if (mFont->isFixed()) {
				unk2C = mFont->getFixedWidth();
			} else {
				char trash[0x4]; // I hate this function
				JUTFont::TWidth uStack_ec;
				mFont->getWidthEntry(r27, &uStack_ec);
				if (r19) {
					unk2C = uStack_ec.field_0x1;
				} else {
					unk2C = uStack_ec.field_0x1 + uStack_ec.field_0x0;
				}
			}

			unk2C *= (float)unk34 / mFont->getWidth();

			if ((((s32)(10000.0f * ((mCursorH + unk2C) - unk1C))) / 10000.0f)
			        > param_3
			    && mCursorH > f26) {
				param_1 -= r18 ? 2 : 1;
				mCursorV += unk14;
				if (!param_7 && (param_4 != nullptr)) {
					param_4[r21] = 0.5f + f24;
				}
				r21++;
				if (r21 == 0x100) {
					break;
				}
				r17      = false;
				r19      = false;
				mCursorH = unk1C;
				f24      = 0.0f;
			} else {
				if (param_7) {
					if (param_4 != nullptr) {
						mFont->drawChar_scale(mCursorH + (f32)(s16)param_4[r21],
						                      mCursorV, unk34, unk38, r27, r19);
					} else {
						mFont->drawChar_scale(mCursorH, mCursorV, unk34, unk38,
						                      r27, r19);
					}
				}
				r19 = true;
				mCursorH += unk2C;
			}
		}

		if (r17) {
			if (mCursorH - f26 > f24) {
				f24 = mCursorH - f26;
			}
			mCursorH += unk10;
			unk2C += unk10;
			f32 f0 = ((float)unk38 / mFont->getHeight()) * mFont->getDescent();
			if (f23 < mCursorV + f0) {
				f23 = mCursorV + f0;
			}
			if (mCursorH > f21) {
				f21 = mCursorH;
			}
			if (mCursorH < f22) {
				f22 = mCursorH;
			}
			if (f18 < f22) {
				f22 = f18;
			}
			if (mCursorV > f19) {
				f19 = mCursorV;
			}
			if (mCursorV < f20) {
				f20 = mCursorV;
			}
		}
		r27 = *(param_1++);
	} while (true);

	if (param_4 != nullptr) {
		param_4[r21] = 0xffff;
	}
	param_5.unk0 = f21 - f22;
	param_5.unk4 = f19 - f20 + mFont->getLeading();
	if (!param_7) {
		mCursorH = f26;
		mCursorV = f25;
	}
	return f23;
}

void J2DPrint::doCtrlCode(int code)
{
	switch (code) {
	case 8:
		mCursorH = mCursorH - unk2C;
		unk2C    = 0.0;
		break;
	case 9:
		if (unk30 > 0) {
			float fVar1 = mCursorH;
			mCursorH    = (float)(unk30 + unk30 * ((int)fVar1 / unk30));
			unk2C       = mCursorH - fVar1;
		}
		break;
	case 10:
		unk2C    = 0.0;
		mCursorH = (float)unk1C;
		mCursorV += (float)unk14;
		break;
	case 0xd:
		unk2C    = 0.0;
		mCursorH = (float)unk1C;
		break;
	case 0x1c:
		mCursorH += 1.0f;
		break;
	case 0x1d:
		mCursorH -= 1.0f;
		break;
	case 0x1e:
		mCursorV -= 1.0f;
		break;
	case 0x1f:
		mCursorV += 1.0f;
	}
}

u16 J2DPrint::doEscapeCode(const u8** param_1, u8 opacity)
{
	u8 const* puVar9 = *param_1;

	u16 code = 0;
	for (int i = 0; i < 2; i++) {
		u16 uVar3;
		if (mFont->isLeadByte(**param_1)) {
			uVar3 = ((**param_1) << 8) | (*param_1)[1];
			(*param_1) += 2;
		} else {
			uVar3 = **param_1;
			(*param_1)++;
		}
		if (uVar3 >= 0x80 || uVar3 < 0x20) {
			*param_1 = puVar9;
			return 0;
		}
		code = ((code) << 8) | uVar3;
	}

	JUtility::TColor local_40 = unk8;
	JUtility::TColor local_44 = unkC;
	switch (code) {
	case 'CU': // Cursor Up
		mCursorV -= getNumber(param_1, 1, 0, 10);
		break;
	case 'CD': // Cursor Down
		mCursorV += getNumber(param_1, 1, 0, 10);
		break;
	case 'CL': // Cursor Left
		mCursorH -= getNumber(param_1, 1, 0, 10);
		break;
	case 'CR': // Cursor Right
		mCursorH += getNumber(param_1, 1, 0, 10);
		break;
	case 'LU':
		mCursorV -= unk14;
		break;
	case 'LD':
		mCursorV += unk14;
		break;

	case 'ST': {
		s32 val = getNumber(param_1, unk30, unk30, 10);
		if (val > 0) {
			unk30 = val;
		}
		break;
	}
	case 'CC': {
		unk8       = getNumber(param_1, unk44, unk8, 16);
		local_40   = unk8;
		local_40.a = local_40.a * opacity / 0xff;
		local_44.a = local_44.a * opacity / 0xff;
		JUtility::TColor* local_68;
		if (unk18 != 0) {
			local_68 = &local_44;
		} else {
			local_68 = &local_40;
		}
		mFont->setGradColor(local_40, *local_68);
		break;
	}
	case 'GC': {
		unkC       = getNumber(param_1, unk48, unkC, 16);
		local_44   = unkC;
		local_40.a = local_40.a * opacity / 0xff;
		local_44.a = local_44.a * opacity / 0xff;
		JUtility::TColor* local_74;
		if (unk18 != 0) {
			local_74 = &local_44;
		} else {
			local_74 = &local_40;
		}
		mFont->setGradColor(local_40, *local_74);
		break;
	}
	case 'FX': {
		int dVar13 = getNumber(param_1, mFontSizeX, unk34, 10);
		if (dVar13 >= 0) {
			unk34 = dVar13;
		}
		break;
	}
	case 'FY': {
		int dVar14 = getNumber(param_1, mFontSizeY, unk38, 10);
		if (dVar14 >= 0) {
			unk38 = dVar14;
		}
		break;
	}
	case 'SH':
		unk10 = getNumber(param_1, unk4C, unk10, 10);
		break;
	case 'SV':
		unk14 = getNumber(param_1, unk50, unk14, 10);
		break;
	case 'GM': {
		unk18      = !!getNumber(param_1, !unk18, unk18, 10);
		local_40.a = local_40.a * opacity / 0xff;
		local_44.a = local_44.a * opacity / 0xff;
		JUtility::TColor* local_80;
		if (unk18 != 0) {
			local_80 = &local_44;
		} else {
			local_80 = &local_40;
		}
		mFont->setGradColor(local_40, *local_80);
		break;
	}
	case 'HM':
		break;
	default:
		*param_1 = puVar9;
		code     = 0;
		break;
	}
	return code;
}

void J2DPrint::initchar()
{
	unk8  = unk44;
	unkC  = unk48;
	unk10 = unk4C;
	unk14 = unk50;
	unk18 = unk60;
	unk30 = unk54;
	unk34 = mFontSizeX;
	unk38 = mFontSizeY;
}

int J2DPrint::getNumber(const u8** param_1, s32 param_2, s32 param_3, int base)
{
	const u8* puVar1 = *param_1;
	s32 uVar2        = param_2;
	if (*puVar1 != '[') {
		return param_2;
	}
	(*param_1)++;
	uVar2 = 0;
	char* local_28;
	if (base == 10) {
		uVar2 = strtol((char*)*param_1, &local_28, base);
	} else if (base == 16) {
		uVar2 = strtoul((char*)*param_1, &local_28, base);
		if ((u32)local_28 - (u32)*param_1 != 8) {
			if ((u32)local_28 - (u32)*param_1 == 6) {
				uVar2 = (uVar2 << 8) | 0xff;
			} else {
				*param_1 = puVar1;
				return param_3;
			}
		}
	}
	if (local_28[0] != ']') {
		*param_1 = puVar1;
		return param_3;
	} else {
		if ((char*)*param_1 == (char*)local_28) {
			uVar2 = param_2;
		}

		*param_1 = (const u8*)local_28 + 1;
	}
	return uVar2;
}
