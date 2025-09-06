#include "dolphin/types.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_fp.h"

static const f64 bit_values[] = {
	1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256,
};

static const f64 digit_values[] = {
	1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8,
};

void __num2dec(const decform* f, f64 x, decimal* d)
{
	int sp30;
	int exp;
	u8* p;
	int digits;
	int var_r4;
	f64 var_f1;
	int var_r11;
	int var_r12;
	int temp_r5;
	int var_r6;
	int var_r6_2;
	const f64* var_r5;

	digits = f->digits;
	if (digits > 16) {
		digits = 16;
	}

	d->sign       = 0;
	d->exp        = 0;
	d->sig.length = 1;

	if (x == 0.0) {
		d->sig.text[0] = '0';
		return;
	}

	if (!isfinite(x)) {
		d->sig.text[0] = isnan(x) ? 'N' : 'I';
		return;
	}

	d->sig.length = 0;
	if (x < 0.0) {
		x       = -x;
		d->sign = 1;
	}

	frexp(x, &sp30);
	var_r4 = (sp30 * 301029) / 1000000; // log_10(2)
	exp    = var_r4;
	var_r5 = bit_values;
	if (var_r4 < 0) {
		var_r4 = -var_r4;
		while (var_r4 != 0) {
			if (var_r4 & 1) {
				x *= *var_r5;
			}
			var_r4 >>= 1;
			var_r5++;
		}
	} else if (var_r4 > 0) {
		var_f1 = 1.0f;
		while (var_r4 != 0) {
			if (var_r4 & 1) {
				var_f1 *= *var_r5;
			}
			var_r4 >>= 1;
			var_r5++;
		}
		x /= var_f1;
	}

	while (x >= 1.0) {
		x *= 0.1;
		exp += 1;
	}

	while (x < 0.1) {
		x *= 10.0;
		exp -= 1;
	}

	p = d->sig.text;
	while (digits != 0) {
		var_r12 = digits;
		if (digits > 8) {
			var_r12 = 8;
		}
		d->sig.length += var_r12;
		digits -= var_r12;
		exp -= var_r12;
		p += var_r12;
		x *= digit_values[var_r12 - 1];
		var_r6 = (int)x;
		x      = x - var_r6;

		var_r11 = var_r12 + 1;
		while (--var_r11 != 0) {
			*--p = '0' + (var_r6 % 10);
			var_r6 /= 10;
		}
		p += var_r12;
	}

	digits = f->digits;
	if (f->digits > 36) {
		digits = 36;
	}

	temp_r5 = digits - d->sig.length;
	if (temp_r5 > 0) {
		var_r6_2 = temp_r5 + 1;
		while (--var_r6_2 != 0) {
			*p++ = '0';
		}
		exp -= temp_r5;
		d->sig.length += temp_r5;
	}

	d->exp = exp;
}

