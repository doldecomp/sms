#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_fp.h"

static const double bit_values[] = {
	1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256,
};

static const double digit_values[] = {
	1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8,
};

void __num2dec(const decform* f, double x, decimal* d)
{
	int sp30;
	int exp;
	unsigned char* p;
	int digits;
	int var_r4;
	double var_f1;
	int var_r11;
	int var_r12;
	int temp_r5;
	int var_r6;
	int var_r6_2;
	const double* var_r5;

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
		exp++;
	}

	while (x < 0.1) {
		x *= 10.0;
		exp--;
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
	if (f->digits > SIGDIGLEN) {
		digits = SIGDIGLEN;
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

// fabricated, UNUSED in the map. Defined after __num2dec so it is codegen'd
// first, pooling the 0.0/1.0/int-conversion literals into .sdata2 in the
// original order (@268/@270/@272 before @362/@363) -- required for the DOL
// to link byte-identical even though the function itself is deadstripped.
// TODO: incorrect size (0x13c here vs 0x2a4 in the map); the real body
// likely handled the '0'/'N'/'I' encodings written by __num2dec.
double __dec2num(const decimal* d)
{
	int digits;
	int var_r4;
	int chunk;
	int n;
	int count;
	double x;
	double var_f1;
	const double* var_r5;
	const unsigned char* p;

	x = 0.0;

	digits = d->sig.length;
	if (digits == 0) {
		return x;
	}

	var_f1 = 1.0;
	var_r4 = d->exp;
	var_r5 = bit_values;
	if (var_r4 < 0) {
		var_r4 = -var_r4;
		while (var_r4 != 0) {
			if (var_r4 & 1) {
				var_f1 *= *var_r5;
			}
			var_r4 >>= 1;
			var_r5++;
		}
	} else if (var_r4 > 0) {
		while (var_r4 != 0) {
			if (var_r4 & 1) {
				var_f1 *= *var_r5;
			}
			var_r4 >>= 1;
			var_r5++;
		}
	}

	p = d->sig.text;
	while (digits != 0) {
		n = digits;
		if (n > 8) {
			n = 8;
		}
		chunk = 0;
		count = n + 1;
		while (--count != 0) {
			chunk = chunk * 10 + (*p++ - '0');
		}
		x = x * digit_values[n - 1] + chunk;
		digits -= n;
	}

	if (d->exp < 0) {
		x /= var_f1;
	} else {
		x *= var_f1;
	}

	if (d->sign) {
		x = -x;
	}

	return x;
}
