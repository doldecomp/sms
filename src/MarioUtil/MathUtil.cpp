#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JMath.hpp>

static u16 atntable[] = {
	0,    10,   20,   31,   41,   51,   61,   71,   81,   92,   102,  112,
	122,  132,  143,  153,  163,  173,  183,  194,  204,  214,  224,  234,
	244,  255,  265,  275,  285,  295,  305,  316,  326,  336,  346,  356,
	367,  377,  387,  397,  407,  417,  428,  438,  448,  458,  468,  478,
	489,  499,  509,  519,  529,  539,  550,  560,  570,  580,  590,  600,
	610,  621,  631,  641,  651,  661,  671,  681,  692,  702,  712,  722,
	732,  742,  752,  763,  773,  783,  793,  803,  813,  823,  833,  844,
	854,  864,  874,  884,  894,  904,  914,  924,  935,  945,  955,  965,
	975,  985,  995,  1005, 1015, 1025, 1036, 1046, 1056, 1066, 1076, 1086,
	1096, 1106, 1116, 1126, 1136, 1146, 1156, 1166, 1177, 1187, 1197, 1207,
	1217, 1227, 1237, 1247, 1257, 1267, 1277, 1287, 1297, 1307, 1317, 1327,
	1337, 1347, 1357, 1367, 1377, 1387, 1397, 1407, 1417, 1427, 1437, 1447,
	1457, 1467, 1477, 1487, 1497, 1507, 1517, 1527, 1537, 1547, 1557, 1567,
	1577, 1587, 1597, 1607, 1617, 1627, 1637, 1646, 1656, 1666, 1676, 1686,
	1696, 1706, 1716, 1726, 1736, 1746, 1756, 1765, 1775, 1785, 1795, 1805,
	1815, 1825, 1835, 1845, 1854, 1864, 1874, 1884, 1894, 1904, 1914, 1923,
	1933, 1943, 1953, 1963, 1973, 1982, 1992, 2002, 2012, 2022, 2031, 2041,
	2051, 2061, 2071, 2080, 2090, 2100, 2110, 2120, 2129, 2139, 2149, 2159,
	2168, 2178, 2188, 2198, 2207, 2217, 2227, 2237, 2246, 2256, 2266, 2275,
	2285, 2295, 2305, 2314, 2324, 2334, 2343, 2353, 2363, 2372, 2382, 2392,
	2401, 2411, 2421, 2430, 2440, 2450, 2459, 2469, 2478, 2488, 2498, 2507,
	2517, 2526, 2536, 2546, 2555, 2565, 2574, 2584, 2594, 2603, 2613, 2622,
	2632, 2641, 2651, 2660, 2670, 2679, 2689, 2699, 2708, 2718, 2727, 2737,
	2746, 2756, 2765, 2775, 2784, 2793, 2803, 2812, 2822, 2831, 2841, 2850,
	2860, 2869, 2879, 2888, 2897, 2907, 2916, 2926, 2935, 2944, 2954, 2963,
	2973, 2982, 2991, 3001, 3010, 3019, 3029, 3038, 3047, 3057, 3066, 3075,
	3085, 3094, 3103, 3113, 3122, 3131, 3141, 3150, 3159, 3168, 3178, 3187,
	3196, 3206, 3215, 3224, 3233, 3243, 3252, 3261, 3270, 3279, 3289, 3298,
	3307, 3316, 3325, 3335, 3344, 3353, 3362, 3371, 3380, 3390, 3399, 3408,
	3417, 3426, 3435, 3444, 3453, 3463, 3472, 3481, 3490, 3499, 3508, 3517,
	3526, 3535, 3544, 3553, 3562, 3571, 3580, 3589, 3599, 3608, 3617, 3626,
	3635, 3644, 3653, 3662, 3670, 3679, 3688, 3697, 3706, 3715, 3724, 3733,
	3742, 3751, 3760, 3769, 3778, 3787, 3796, 3804, 3813, 3822, 3831, 3840,
	3849, 3858, 3867, 3875, 3884, 3893, 3902, 3911, 3920, 3928, 3937, 3946,
	3955, 3964, 3972, 3981, 3990, 3999, 4007, 4016, 4025, 4034, 4042, 4051,
	4060, 4069, 4077, 4086, 4095, 4103, 4112, 4121, 4129, 4138, 4147, 4155,
	4164, 4173, 4181, 4190, 4199, 4207, 4216, 4224, 4233, 4242, 4250, 4259,
	4267, 4276, 4284, 4293, 4302, 4310, 4319, 4327, 4336, 4344, 4353, 4361,
	4370, 4378, 4387, 4395, 4404, 4412, 4421, 4429, 4438, 4446, 4454, 4463,
	4471, 4480, 4488, 4497, 4505, 4513, 4522, 4530, 4539, 4547, 4555, 4564,
	4572, 4580, 4589, 4597, 4605, 4614, 4622, 4630, 4639, 4647, 4655, 4663,
	4672, 4680, 4688, 4697, 4705, 4713, 4721, 4730, 4738, 4746, 4754, 4762,
	4771, 4779, 4787, 4795, 4803, 4812, 4820, 4828, 4836, 4844, 4852, 4860,
	4869, 4877, 4885, 4893, 4901, 4909, 4917, 4925, 4933, 4941, 4949, 4958,
	4966, 4974, 4982, 4990, 4998, 5006, 5014, 5022, 5030, 5038, 5046, 5054,
	5062, 5070, 5078, 5086, 5094, 5101, 5109, 5117, 5125, 5133, 5141, 5149,
	5157, 5165, 5173, 5181, 5188, 5196, 5204, 5212, 5220, 5228, 5235, 5243,
	5251, 5259, 5267, 5275, 5282, 5290, 5298, 5306, 5313, 5321, 5329, 5337,
	5344, 5352, 5360, 5368, 5375, 5383, 5391, 5398, 5406, 5414, 5421, 5429,
	5437, 5444, 5452, 5460, 5467, 5475, 5483, 5490, 5498, 5505, 5513, 5521,
	5528, 5536, 5543, 5551, 5559, 5566, 5574, 5581, 5589, 5596, 5604, 5611,
	5619, 5626, 5634, 5641, 5649, 5656, 5664, 5671, 5679, 5686, 5694, 5701,
	5708, 5716, 5723, 5731, 5738, 5745, 5753, 5760, 5768, 5775, 5782, 5790,
	5797, 5804, 5812, 5819, 5826, 5834, 5841, 5848, 5856, 5863, 5870, 5878,
	5885, 5892, 5899, 5907, 5914, 5921, 5928, 5936, 5943, 5950, 5957, 5964,
	5972, 5979, 5986, 5993, 6000, 6008, 6015, 6022, 6029, 6036, 6043, 6050,
	6058, 6065, 6072, 6079, 6086, 6093, 6100, 6107, 6114, 6121, 6128, 6135,
	6142, 6150, 6157, 6164, 6171, 6178, 6185, 6192, 6199, 6206, 6213, 6220,
	6227, 6234, 6240, 6247, 6254, 6261, 6268, 6275, 6282, 6289, 6296, 6303,
	6310, 6317, 6323, 6330, 6337, 6344, 6351, 6358, 6365, 6371, 6378, 6385,
	6392, 6399, 6406, 6412, 6419, 6426, 6433, 6440, 6446, 6453, 6460, 6467,
	6473, 6480, 6487, 6493, 6500, 6507, 6514, 6520, 6527, 6534, 6540, 6547,
	6554, 6560, 6567, 6574, 6580, 6587, 6594, 6600, 6607, 6613, 6620, 6627,
	6633, 6640, 6646, 6653, 6660, 6666, 6673, 6679, 6686, 6692, 6699, 6705,
	6712, 6718, 6725, 6731, 6738, 6744, 6751, 6757, 6764, 6770, 6777, 6783,
	6790, 6796, 6803, 6809, 6815, 6822, 6828, 6835, 6841, 6848, 6854, 6860,
	6867, 6873, 6879, 6886, 6892, 6898, 6905, 6911, 6917, 6924, 6930, 6936,
	6943, 6949, 6955, 6962, 6968, 6974, 6980, 6987, 6993, 6999, 7005, 7012,
	7018, 7024, 7030, 7037, 7043, 7049, 7055, 7061, 7068, 7074, 7080, 7086,
	7092, 7098, 7105, 7111, 7117, 7123, 7129, 7135, 7141, 7147, 7154, 7160,
	7166, 7172, 7178, 7184, 7190, 7196, 7202, 7208, 7214, 7220, 7226, 7232,
	7238, 7244, 7250, 7256, 7262, 7268, 7274, 7280, 7286, 7292, 7298, 7304,
	7310, 7316, 7322, 7328, 7334, 7340, 7346, 7352, 7358, 7363, 7369, 7375,
	7381, 7387, 7393, 7399, 7405, 7411, 7416, 7422, 7428, 7434, 7440, 7446,
	7451, 7457, 7463, 7469, 7475, 7480, 7486, 7492, 7498, 7503, 7509, 7515,
	7521, 7526, 7532, 7538, 7544, 7549, 7555, 7561, 7566, 7572, 7578, 7584,
	7589, 7595, 7601, 7606, 7612, 7618, 7623, 7629, 7635, 7640, 7646, 7651,
	7657, 7663, 7668, 7674, 7679, 7685, 7691, 7696, 7702, 7707, 7713, 7718,
	7724, 7730, 7735, 7741, 7746, 7752, 7757, 7763, 7768, 7774, 7779, 7785,
	7790, 7796, 7801, 7807, 7812, 7818, 7823, 7828, 7834, 7839, 7845, 7850,
	7856, 7861, 7866, 7872, 7877, 7883, 7888, 7893, 7899, 7904, 7910, 7915,
	7920, 7926, 7931, 7936, 7942, 7947, 7952, 7958, 7963, 7968, 7974, 7979,
	7984, 7990, 7995, 8000, 8005, 8011, 8016, 8021, 8026, 8032, 8037, 8042,
	8047, 8053, 8058, 8063, 8068, 8074, 8079, 8084, 8089, 8094, 8100, 8105,
	8110, 8115, 8120, 8125, 8131, 8136, 8141, 8146, 8151, 8156, 8161, 8166,
	8172, 8177, 8182, 8187, 8192,
};

#define qr0 0

int IConverge(int param_1, int param_2, int param_3, int param_4)
{
	int result = param_1;
	if (result < param_2) {
		result += param_3;
		if (result > param_2)
			result = param_2;
	} else {
		result -= param_4;
		if (result < param_2)
			result = param_2;
	}
	return result;
}

f32 FConverge(f32 param_1, f32 param_2, f32 param_3, f32 param_4)
{
	f32 result = param_1;
	if (result < param_2) {
		result += param_3;
		if (result > param_2)
			result = param_2;
	} else {
		result -= param_4;
		if (result < param_2)
			result = param_2;
	}
	return result;
}

static u16 GetAtanTable(f32 param_1, f32 param_2)
{
	if (param_1 == 0)
		return atntable[0];

	int idx = param_2 * __fres(param_1) * 1024.0f + 0.5f;
	return atntable[idx];
}

s16 matan(f32 param_1, f32 param_2)
{
	// TODO: currently too lazy to figure out how exactly they use symmetries
	// here and what exact result transforms are needed in various branches.
	// Probably should be something nice and symmetric and not this.
	if (param_2 >= 0.0f) {
		if (param_1 >= 0.0f) {
			if (param_1 >= param_2)
				return 0x0000 + GetAtanTable(param_1, param_2);
			else
				return 0x4000 - GetAtanTable(param_1, param_2);
		} else {
			param_1 = -param_1;
			if (param_1 < param_2)
				return GetAtanTable(param_1, param_2) + 0x4000;
			else
				return GetAtanTable(param_1, param_2) + 0x8000;
		}
	} else {
		param_2 = -param_2;

		if (param_1 < 0.0f) {
			param_1 = -param_1;
			if (param_1 <= param_2)
				return GetAtanTable(param_1, param_2) + 0x8000;
			else
				return GetAtanTable(param_1, param_2) + -0x4000;
		} else {
			if (param_1 < param_2)
				return GetAtanTable(param_1, param_2) - 0x4000;
			else
				return -GetAtanTable(param_1, param_2);
		}
	}
}

static inline void MsGetRotFromZaxisY2(const JGeometry::TVec3<f32>& axis,
                                       f32* out)
{
	if (axis.z == 0.0f) {
		if (axis.x > 0.0f) {
			*out = 90.0f;
			return;
		} else {
			*out = -90.0f;
			return;
		}
	}

	if (axis.z > 0.0f) {
		*out = (360.0f / 65536.0f) * matan(axis.z, axis.x);
	} else {
		f32 theta = matan(-axis.z, axis.x) * (360.0f / 65536.0f);
		*out      = 180.0f - theta;
	}
}

// TODO: very much fake
static inline f32 fake_sqrt(f32 mag)
{
	if (mag > 0.0f) {
		f64 root = __frsqrte(mag);
		return 0.5f * root * (3.0f - mag * (root * root)) * mag;
	}
	return 0.0f;
}

// TODO: very much fake
static inline void MsGetRotFromZaxisX2(const JGeometry::TVec3<f32>& axis,
                                       f32* out)
{
	if (axis.y == 1.0f) {
		*out = 90.0f;
		return;
	} else if (axis.y == -1.0f) {
		*out = -90.0f;
		return;
	}

	f32 a = 1.0f - axis.y * axis.y;

	// TODO: it smells to me like this entire function is not real but a result
	// of MWCC optimizing out stuff for once
	f32 dVar3 = fake_sqrt(a);

	*out = -(matan(dVar3, axis.y) * (360.0f / 65536.0f));
}

JGeometry::TVec3<f32> MsGetRotFromZaxis(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> result;
	result.zero();

	JGeometry::TVec3<f32> axis = param_1;
	axis.normalize();

	MsGetRotFromZaxisX2(axis, &result.x);
	MsGetRotFromZaxisY2(axis, &result.y);

	return result;
}

void MsMtxSetRotRPH(MtxPtr param_1, f32 r, f32 p, f32 h)
{
	f32 sr = JMASin(r);
	f32 sp = JMASin(p);
	f32 sh = JMASin(h);

	f32 cr = JMACos(r);
	f32 cp = JMACos(p);
	f32 ch = JMACos(h);

	char trash[0x4]; // TODO: skill issue

	param_1[0][0] = ch * cp;
	param_1[1][0] = sh * cp;
	param_1[2][0] = -sp;

	param_1[0][1] = sr * (ch * sp) - (sh * cr);
	param_1[1][1] = sr * (sh * sp) + (ch * cr);
	param_1[2][1] = cp * sr;

	param_1[0][2] = cr * (ch * sp) + (sh * sr);
	param_1[1][2] = cr * (sh * sp) - (ch * sr);
	param_1[2][2] = cp * cr;

	param_1[0][3] = 0.0f;
	param_1[1][3] = 0.0f;
	param_1[2][3] = 0.0f;
}

void MsMtxSetXYZRPH(MtxPtr param_1, f32 x, f32 y, f32 z, s16 r, s16 p, s16 h)
{
	f32 sr = JMASSin(r);
	f32 sp = JMASSin(p);
	f32 sh = JMASSin(h);

	f32 cr = JMASCos(r);
	f32 cp = JMASCos(p);
	f32 ch = JMASCos(h);

	param_1[0][0] = ch * cp;
	param_1[1][0] = sh * cp;
	param_1[2][0] = -sp;

	param_1[0][1] = sr * (ch * sp) - (sh * cr);
	param_1[1][1] = sr * (sh * sp) + (ch * cr);
	param_1[2][1] = cp * sr;

	param_1[0][2] = cr * (ch * sp) + (sh * sr);
	param_1[1][2] = cr * (sh * sp) - (ch * sr);
	param_1[2][2] = cp * cr;

	param_1[0][3] = x;
	param_1[1][3] = y;
	param_1[2][3] = z;
}

void MsMtxSetTRS(MtxPtr param_1, f32 x, f32 y, f32 z, f32 r, f32 p, f32 h,
                 f32 sx, f32 sy, f32 sz)
{
	f32 sr = JMASin(r);
	f32 sp = JMASin(p);
	f32 sh = JMASin(h);

	f32 cr = JMACos(r);
	f32 cp = JMACos(p);
	f32 ch = JMACos(h);

	char trash[0x8]; // TODO: skill issue

	param_1[0][0] = (ch * cp) * sx;
	param_1[1][0] = (sh * cp) * sx;
	param_1[2][0] = -sp * sx;

	param_1[0][1] = (sr * (ch * sp) - (sh * cr)) * sy;
	param_1[1][1] = (sr * (sh * sp) + (ch * cr)) * sy;
	param_1[2][1] = (cp * sr) * sy;

	param_1[0][2] = (cr * (ch * sp) + (sh * sr)) * sz;
	param_1[1][2] = (cr * (sh * sp) - (ch * sr)) * sz;
	param_1[2][2] = (cp * cr) * sz;

	param_1[0][3] = x;
	param_1[1][3] = y;
	param_1[2][3] = z;
}

BOOL MsIsInSight(const JGeometry::TVec3<f32>& param_1, f32 param_2,
                 const JGeometry::TVec3<f32>& param_3, f32 param_4, f32 param_5,
                 f32 param_6)
{
	JGeometry::TVec3<f32> tmp = param_3;
	tmp -= param_1;

	char trash[0x4]; // TODO: skill issue

	if (tmp.squared() < param_6 * param_6)
		return true;

	if (tmp.squared() < param_4 * param_4
	    && abs(MsAngleDiff(MsGetRotFromZaxisY(tmp), param_2))
	           < param_5 * 0.5f) {
		return true;
	}

	return false;
}

void SMS_GoRotate(const JGeometry::TVec3<f32>& param_1,
                  const JGeometry::TVec3<f32>& param_2, f32 param_3,
                  f32* param_4)
{
	float diffx = param_2.x - param_1.x;
	float diffz = param_2.z - param_1.z;
	s16 angle   = matan(diffz, diffx);
	s16 sVar3   = param_3 * (65536.0f / 360.0f);
	int iVar1 = IConverge((s16)(angle - (int)(*param_4 * (65536.0f / 360.0f))),
	                      0, sVar3, sVar3);
	*param_4  = (s16)(angle - iVar1) * (360.0f / 65536.0f);
}

void SMSCalcJumpVelocityY(const JGeometry::TVec3<f32>& desired_pos,
                          const JGeometry::TVec3<f32>& initial_pos,
                          f32 jump_y_velocity, f32 gravity_accel, f32 min_y,
                          JGeometry::TVec3<f32>* result)
{
	f32 currY    = initial_pos.y;
	f32 desiredY = desired_pos.y;

	int ticks = 1;
	for (f32 ySpeed = jump_y_velocity;;) {
		currY += ySpeed;

		if (ySpeed < 0.0f && currY <= desiredY)
			break;

		ySpeed -= gravity_accel;

		if (ySpeed < min_y)
			ySpeed = min_y;

		++ticks;
	}

	// most probably a fakematch, but whatever
	f32 dx;
	f32 dz;

	f32 frequency = 1.0f / ticks;

	dx = desired_pos.x - initial_pos.x;
	dz = desired_pos.z - initial_pos.z;

	result->x = frequency * dx;
	result->y = jump_y_velocity;
	result->z = frequency * dz;
}

void SMSCalcJumpVelocityXZ(const JGeometry::TVec3<f32>& param_1,
                           const JGeometry::TVec3<f32>& param_2, f32 param_3,
                           f32 param_4, JGeometry::TVec3<f32>* result)
{
	JGeometry::TVec3<f32> horVec;
	horVec.set(param_1.x - param_2.x, 0.0f, param_1.z - param_2.z);

	f32 horMag = VECMag(&horVec);

	int tmp  = horMag / param_3;
	int tmp3 = (tmp + 1) * tmp / 2;

	f32 fVar3 = param_3 / horMag;
	f32 dy    = param_1.y - param_2.y;

	f32 resX = horVec.x * fVar3;
	f32 resZ = horVec.z * fVar3;
	f32 resY = fVar3 * dy + tmp3 * param_4 * fVar3;

	result->x = resX;
	result->y = resY;
	result->z = resZ;
}

asm f32 MsVECMag2(register Vec* v)
{
#ifdef __MWERKS__ // clang-format off
  psq_l   f3, Vec.x(v), 0, qr0
  ps_mul  f3, f3, f3

  lfs     f4, Vec.z(v)
  ps_madd f5, f4, f4, f3

  ps_sum0 f2, f5, f3, f3
  frsqrte f0, f2
  fneg    f1, f2
  fsel    f0, f1, f2, f0
  fmuls   f1, f2, f0
#endif // clang-format on
}

asm void MsVECNormalize(register Vec* v1, register Vec* v2)
{
#ifdef __MWERKS__ // clang-format off
  psq_l   f6, Vec.x(v1), 0, qr0
  ps_mul  f3, f6, f6

  lfs     f4, Vec.z(v1)
  ps_madd f5, f4, f4, f3

  ps_sum0 f2, f5, f3, f3

  frsqrte  f0, f2
  ps_muls0 f6, f6, f0

  psq_st f6, Vec.x(v2), 0, qr0
  fmuls  f4, f4, f0
  stfs   f4, Vec.z(v2)
#endif // clang-format on
}
