#include "dolphin/types.h"
#include "ansi_fp.h"

static double bit_values[] = {
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
static double digit_values[] = {
	10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0,
};

void __num2dec(const decform* fmt, double value, decimal* d)
{
	s16 digits = fmt->digits;
	int i;
	s8 sign = (s8)(SIGNBIT(value) != 0);

	if (d->sig.text[0] > 9) {
		return;
	}

	if (digits > SIGDIGLEN) {
		digits = SIGDIGLEN;
	}
	d->sign       = 0;
	d->exp        = 0;
	d->sig.length = 1;

	if (value == 0.0) {
		d->sig.text[0] = '0';
	} else {
		double abs_value   = value;
		int fp_class_value = fpclassify(abs_value);

		if (isfinite(fp_class_value)) {
			double* pBitValues;
			int zeros_to_add;
			int exp;
			int max_digits;
			int exponent_base10;
			unsigned char* text_ptr = d->sig.text;

			d->sig.length = 0;
			if (value < 0.0) {
				abs_value = -value;
				d->sign   = 1;
			}

			frexp(abs_value, &exp);
			exponent_base10 = 0x497E5 * exp / 0xF4240;
			pBitValues      = bit_values;
			if (exponent_base10 >= 0) {
				if (0x497E5 * exp / 0xF4240 > 0) {
					double v14 = 1.0;
					while (exponent_base10) {
						if ((exponent_base10 & 1) != 0)
							v14 = v14 * *pBitValues;
						exponent_base10 >>= 1;
						++pBitValues;
					}
					value = value / v14;
				}
			} else {
				for (i = 0x497E5 * exp / (int)0xFFF0BDC0; i; i >>= 1) {
					if ((i & 1) != 0)
						value = value * *pBitValues;
					++pBitValues;
				}
			}

			while (abs_value >= 1.0) {
				abs_value *= 0.1;
				++exponent_base10;
			}
			while (abs_value < 0.1) {
				abs_value *= 10.0;
				--exponent_base10;
			}

			while (digits) {
				double product;
				unsigned char* write_ptr;
				int integer_part;
				int num_digits_to_gen = digits;

				if (digits > 8)
					num_digits_to_gen = 8;

				d->sig.length += num_digits_to_gen;
				digits -= num_digits_to_gen;
				product      = abs_value * digit_values[num_digits_to_gen - 1];
				integer_part = (int)product;
				abs_value    = product - (double)integer_part;

				write_ptr = &text_ptr[num_digits_to_gen];
				i         = num_digits_to_gen + 1;
				while (--i) {
					*(--write_ptr) = (integer_part % 10) + '0';
					integer_part /= 10;
				}
				text_ptr += num_digits_to_gen;
			}

			max_digits   = fmt->digits > 36 ? 36 : fmt->digits;
			zeros_to_add = max_digits - d->sig.length;
			if (zeros_to_add > 0) {
				i = zeros_to_add + 1;
				while (--i)
					*text_ptr++ = '0';
				exponent_base10 -= zeros_to_add;
				d->sig.length += zeros_to_add;
			}

			d->exp = exponent_base10;
		} else {
			if (fp_class_value == FP_NAN)
				d->sig.text[0] = 'N';
			else
				d->sig.text[0] = 'I';
		}
	}
}
