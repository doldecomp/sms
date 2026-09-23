#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <dolphin/mtx.h>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>
#include "model_mock.cpp"
f32 MsVECMag2(Vec*); void MsVECNormalize(Vec*, Vec*);
f32 J3DHermiteTest(f32 t, s16*, s16*, s16*, s16*, s16*, s16*);
static int fails = 0, checks = 0;
static double rnd() { return (rand() / (double)RAND_MAX) * 20.0 - 10.0; }
static void rmtx(f32* m, int n) { for (int i = 0; i < n; i++) m[i] = (f32)rnd(); }
static void chk(const char* what, double got, double want, double tol = 1e-4) {
	checks++; double e = fabs(got - want) / (1.0 + fabs(want));
	if (!(e <= tol)) { if (fails++ < 20) printf("FAIL %s got %g want %g\n", what, got, want); }
}
// reference: double-precision 3x4 affine concat
static void refConcat(const f32 a[3][4], const f32 b[3][4], double o[3][4]) {
	for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) {
		double v = 0; for (int k = 0; k < 3; k++) v += (double)a[r][k] * b[k][c];
		if (c == 3) v += a[r][3]; o[r][c] = v; }
}
int main() {
	srand(1234);
	for (int it = 0; it < 2000; it++) {
		Mtx A, B, D; rmtx(&A[0][0], 12); rmtx(&B[0][0], 12);
		double R[3][4];
		// J3DPSCalcInverseTranspose: check dst^T * src3x3 == I
		f32 IT[3][3];
		bool ok = J3DPSCalcInverseTranspose(A, IT);
		double det = A[0][0]*(A[1][1]*A[2][2]-A[1][2]*A[2][1]) - A[0][1]*(A[1][0]*A[2][2]-A[1][2]*A[2][0]) + A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);
		chk("invT ok", ok, det != 0);
		if (fabs(det) > 1.0) for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) {
			double v = 0; for (int k = 0; k < 3; k++) v += (double)IT[k][r] * A[k][c];
			chk("invT", v, r == c, 1e-3); }
		// J3DScaleNrmMtx33
		f32 N[3][3], N0[3][3]; rmtx(&N[0][0], 9); for (int i = 0; i < 9; i++) (&N0[0][0])[i] = (&N[0][0])[i];
		Vec s = { (f32)rnd(), (f32)rnd(), (f32)rnd() }; J3DScaleNrmMtx33(N, s);
		for (int r = 0; r < 3; r++) { chk("scl", N[r][0], N0[r][0]*s.x); chk("scl", N[r][1], N0[r][1]*s.y); chk("scl", N[r][2], N0[r][2]*s.z); }
		// J3DMtxProjConcat: A (3x4) * P (4x4)
		f32 P[4][4]; rmtx(&P[0][0], 16); J3DMtxProjConcat(A, (MtxPtr)P, D);
		for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) { double v = 0; for (int k = 0; k < 4; k++) v += (double)A[r][k]*P[k][c]; chk("proj", D[r][c], v, 1e-4); }
		// copies
		f32 C3[3][3]; J3DPSMtx33Copy(N, C3); for (int i = 0; i < 9; i++) chk("33copy", (&C3[0][0])[i], (&N[0][0])[i], 0);
		J3DPSMtx33CopyFrom34(A, C3); for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) chk("33from34", C3[r][c], A[r][c], 0);
		Mtx arrS[4], arrD[4]; rmtx(&arrS[0][0][0], 48); J3DPSMtxArrayCopy(arrS[0], arrD[0], 4);
		for (int i = 0; i < 48; i++) chk("arrcopy", (&arrD[0][0][0])[i], (&arrS[0][0][0])[i], 0);
		JGeometry::gekko_ps_copy12(D, A); for (int i = 0; i < 12; i++) chk("copy12", (&D[0][0])[i], (&A[0][0])[i], 0);
		// J3DPSMtxArrayConcat / J3DMTXConcatArrayIndexedSrc
		J3DPSMtxArrayConcat(A, arrS[0], arrD[0], 4);
		for (int m = 0; m < 4; m++) { refConcat(A, arrS[m], R); for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) chk("arrconcat", arrD[m][r][c], R[r][c]); }
		u16 idx[3] = { 2, 0, 3 };
		J3DMTXConcatArrayIndexedSrc(A, arrS, idx, arrD, 3);
		for (int m = 0; m < 3; m++) { refConcat(A, arrS[idx[m]], R); for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) chk("idxconcat", arrD[m][r][c], R[r][c]); }
		// in-place ArrayConcat (dst == snd)
		Mtx inp; for (int i = 0; i < 12; i++) (&inp[0][0])[i] = (&B[0][0])[i];
		J3DPSMtxArrayConcat(A, inp, inp, 1); refConcat(A, B, R);
		for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) chk("inplace", inp[r][c], R[r][c]);
		// J3DPSMulMtxVec overloads
		Vec v = { (f32)rnd(), (f32)rnd(), (f32)rnd() }, o;
		J3DPSMulMtxVec(A, &v, &o);
		f32 vv[3] = { v.x, v.y, v.z }, oo[3];
		for (int r = 0; r < 3; r++) { double w = A[r][3]; for (int k = 0; k < 3; k++) w += (double)A[r][k]*vv[k]; oo[r] = (f32)w; }
		chk("mulvec", o.x, oo[0]); chk("mulvec", o.y, oo[1]); chk("mulvec", o.z, oo[2]);
		J3DPSMulMtxVec((ROMtxPtr)N, &v, &o);
		for (int r = 0; r < 3; r++) { double w = 0; for (int k = 0; k < 3; k++) w += (double)N[r][k]*vv[k]; oo[r] = (f32)w; }
		chk("romulvec", o.x, oo[0]); chk("romulvec", o.y, oo[1]); chk("romulvec", o.z, oo[2]);
		S16Vec sv = { (s16)(rand() % 2000 - 1000), (s16)(rand() % 2000 - 1000), (s16)(rand() % 2000 - 1000) }, so;
		s16 svv[3] = { sv.x, sv.y, sv.z };
		J3DPSMulMtxVec(A, &sv, &so);
		for (int r = 0; r < 3; r++) { double w = A[r][3]; for (int k = 0; k < 3; k++) w += (double)A[r][k]*svv[k]; w = w > 32767 ? 32767 : w < -32768 ? -32768 : w; chk("mulvecS16", (&so.x)[r], (double)(s16)w, 1.01 / (1.0 + fabs(w))); }
		J3DPSMulMtxVec((ROMtxPtr)N, &sv, &so);
		for (int r = 0; r < 3; r++) { double w = 0; for (int k = 0; k < 3; k++) w += (double)N[r][k]*svv[k]; w = w > 32767 ? 32767 : w < -32768 ? -32768 : w; chk("romulvecS16", (&so.x)[r], (double)(s16)w, 1.01 / (1.0 + fabs(w))); }
		// MsVECMag2 / MsVECNormalize
		double len = sqrt((double)v.x*v.x + (double)v.y*v.y + (double)v.z*v.z);
		chk("mag", MsVECMag2(&v), len); Vec nv; MsVECNormalize(&v, &nv);
		chk("norm", nv.x, v.x/len); chk("norm", nv.y, v.y/len); chk("norm", nv.z, v.z/len);
		// Hermite: reference cubic Hermite with tangents scaled by the key range
		s16 t0 = rand() % 100, t1 = t0 + 1 + rand() % 100, p0 = rand() % 2000 - 1000, p1 = rand() % 2000 - 1000, d0 = rand() % 40 - 20, d1 = rand() % 40 - 20;
		f32 t = t0 + (t1 - t0) * (rand() / (f32)RAND_MAX);
		double u = (t - t0) / (double)(t1 - t0), h = t1 - t0;
		double ref = (2*u*u*u - 3*u*u + 1)*p0 + (u*u*u - 2*u*u + u)*h*d0 + (-2*u*u*u + 3*u*u)*p1 + (u*u*u - u*u)*h*d1;
		chk("hermite", J3DHermiteTest(t, &t0, &p0, &d0, &t1, &p1, &d1), ref, 1e-4);
	}
	// J3DModel::calcWeightEnvelopeMtx
	for (int it = 0; it < 200; it++) {
		Mtx inv[6], world[6], out[4]; u8 scl[6], escl[4]; rmtx(&inv[0][0][0], 72); rmtx(&world[0][0][0], 72);
		for (int i = 0; i < 6; i++) scl[i] = rand() & 1;
		u8 mix[4]; u16 indices[24]; f32 weights[24]; int n = 0;
		for (int e = 0; e < 4; e++) { mix[e] = 1 + rand() % 4; for (int j = 0; j < mix[e]; j++) { indices[n] = rand() % 6; weights[n] = rand() / (f32)RAND_MAX; n++; } }
		MockData md = { 4, mix, indices, weights, inv };
		MockModel mm = { &md, scl, escl, world, out };
		mm.calcWeightEnvelopeMtx();
		n = 0;
		for (int e = 0; e < 4; e++) {
			double acc[3][4] = {}; int f = 1;
			for (int j = 0; j < mix[e]; j++, n++) { double Rm[3][4]; refConcat(world[indices[n]], inv[indices[n]], Rm); f &= scl[indices[n]];
				for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) acc[r][c] += weights[n] * Rm[r][c]; }
			for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) chk("envelope", out[e][r][c], acc[r][c], 1e-4);
			chk("envelope flag", escl[e], f, 0);
		}
	}
	printf("%d checks, %d failures\n", checks, fails);
	return fails != 0;
}
