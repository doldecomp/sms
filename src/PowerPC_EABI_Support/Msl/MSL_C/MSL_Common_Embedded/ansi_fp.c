#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_fp.h"

static const double bit_values[] = {
	1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256,
};

static const double digit_values[] = {
	1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8,
};

// TODO: this TU is the last library object that cannot be source-linked.
// The map records a second, deadstripped function here:
//   UNUSED 0x2a4 __dec2num   ansi_fp.c
// along with its own deadstripped .sdata2 literal @269. Because that function
// is missing, our float literal pool allocates in the wrong order: the map has
// @268=0.0, @270=1.0, @272=<int->double magic 0x4330000080000000>, @362=0.1,
// @363=10.0, while we emit the magic constant last. The code below already
// matches byte-for-byte -- objdiff compares the symbolic reference, so it
// reports 100% -- but after linking, four @sda21 displacements inside
// __num2dec and the literal block itself differ, 23 bytes in all.
// Reconstructing __dec2num (decimal -> double, the inverse of this function)
// should restore the pool order. Its 0x2a4 size is the verification
// constraint. Until then the object stays out of config/GMSE01/objects.json.
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
