#include "dolphin/types.h"
#include "ansi_fp.h"

static const double bit_values[] = {
	10.0,
	100.0,
	10000.0,
	100000000.0,
	10000000000000000.0,
	1.00000000000000005366e+32,
	1.00000000000000002132e+64,
	1.00000000000000007517e+128,
	1.00000000000000003013e+256,
};
static const double digit_values[] = {
	10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0,
};

void __num2dec(const decform* fmt, double value, decimal* d)
{
	int digits = fmt->digits;
	int i;

	if (digits > SIGDIGLEN) {
		digits = SIGDIGLEN;
	}
	d->sign       = 0;
	d->exp        = 0;
	d->sig.length = 1;

	if (value == 0.0) {
		d->sig.text[0] = '0';
	} else {
		if (!isfinite(value)) {
			d->sig.text[0] = isnan(value) ? 'N' : 'I';
		} else {
			const double* pBitValues;
			int zeros_to_add;
			int exp;
			int exponent_base10;
			unsigned char* text_ptr;

			d->sig.length = 0;
			if (value < 0.0) {
				value   = -value;
				d->sign = 1;
			}

			frexp(value, &exp);

			i               = 0x497E5 * exp / 1000000;
			exponent_base10 = i;
			pBitValues      = bit_values;
			if (i < 0) {
				i = -i;
				for (; i; i >>= 1) {
					if ((i & 1) != 0)
						value *= *pBitValues;
					++pBitValues;
				}
			} else if (i > 0) {
				double v14 = 1.0;
				while (i) {
					if ((i & 1) != 0)
						v14 *= *pBitValues;
					i >>= 1;
					++pBitValues;
				}
				value /= v14;
			}

			while (value >= 1.0) {
				value *= 0.1;
				++exponent_base10;
			}
			while (value < 0.1) {
				value *= 10.0;
				--exponent_base10;
			}

			text_ptr = d->sig.text;

			while (digits) {
				int integer_part;
				int num_digits_to_gen = digits;

				if (digits > 8)
					num_digits_to_gen = 8;

				d->sig.length += num_digits_to_gen;
				digits -= num_digits_to_gen;
				exponent_base10 -= num_digits_to_gen;
				value *= digit_values[num_digits_to_gen - 1];
				integer_part = (int)value;
				value -= (double)integer_part;

				text_ptr += num_digits_to_gen;
				i = num_digits_to_gen + 1;
				while (--i) {
					*(--text_ptr) = (integer_part % 10) + '0';
					integer_part /= 10;
				}
				text_ptr += num_digits_to_gen;
			}

			zeros_to_add
			    = (fmt->digits > 36 ? 36 : fmt->digits) - d->sig.length;
			if (zeros_to_add > 0) {
				i = zeros_to_add + 1;
				while (--i)
					*text_ptr++ = '0';
				exponent_base10 -= zeros_to_add;
				d->sig.length += zeros_to_add;
			}

			d->exp = exponent_base10;
		}
	}
}
