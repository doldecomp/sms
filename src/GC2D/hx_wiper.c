#include <dolphin.h>
#include <GC2D/hx_wiper.h>

void Hx_CameraInit() {}
void Hx_GxInit() {}
void Hgx_init_tobj_resource() {}
void Hgx_ReadTexture() {}
void Hx_GetFrBuffer() {}
void Hx_SetVFilter() {}

void __Hx_FrBufferMorf(int Param_1, int Param_2) {}

void Hx_FrBufferMorf() {    

    Hx_SetVFilter();
    __Hx_FrBufferMorf(hx.m_0x8 - 0x18, hx.m_0xC - 0x18);
}

void Frb2_InitGx() {}
void Frb2_InitBlackBox() {}
void Frb2_RendBox() {}

int HxWarning(int Param_1) {

    return Param_1;
}

void Hx_ResetWipe(int Param_1, int Param_2) {
    hx.m_0x10 = 0;
    hx.m_0x0 = Param_1;
    hx.m_0x4 = Param_2;
    hx.m_0x8 = hx.m_0x0 / 2;
    hx.m_0xC = hx.m_0x4 / 2;
    hx.m_0x24 = 0;
    hx.m_0x28 = 0;
}

void Hx_ProvideResource(int Param_1, int Param_2) {

    if (hx.m_0x10 == 2) Hx_Warning(1);
    if (hx.m_0x24 != 0) Hx_Warning(3);
    hx.m_0x24 = 1;
    hx.m_0x2C = Param_1;
    hx.m_0x34 = Param_2;
}

void Hx_ProvideResourceEx(int Param_1) {

    if (hx.m_0x10 == 2) Hx_Warning(1);
    hx.m_0x28 = 1;
    hx.m_0x30 = Param_1;
}

void Hx_RemoveResource() {

    if (hx.m_0x10 == 2) Hx_Warning(1);
    if (hx.m_0x24 == 0) Hx_Warning(2);
    hx.m_0x24 = 0;
    hx.m_0x28 = 0;
}

void dummy_handler() {}

unsigned int Hx_TimerCountDown() {

    if (hx.Timer != 0) hx.Timer--;
    return hx.Timer;
}

void Hx_MotionSet(float Param_1, float Param_2, float Param_3, float Param_4, Mtx Matrix) {

    float Param_5 = Param_2 + Param_3;

    Matrix[0][0] = Param_2;
    Matrix[0][1] = Matrix[0][0] + Param_3;
    Matrix[0][2] = Matrix[0][1] + Param_4;

    Param_1 = (2.0f * Param_1) / (Param_4 + (Param_3 + Param_5));

    if (Param_2 != 10.0f) Matrix[0][3] = Param_1 / Param_2;
    if (Param_4 != 10.0f) Matrix[1][1] = -Param_1 / Param_4;

    Matrix[1][0] = 10.0f;
    Matrix[1][2] = 10.0f;
    Matrix[2][0] = 10.0f;
    Matrix[1][3] = 10.0f;
    return;
}

float Hx_MotionUpdate(Mtx Matrix) {

    if (Matrix[0][0] > Matrix[1][3]) Matrix[1][2] += Matrix[0][3];
    else if (Matrix[0][1] <= Matrix[1][3]) Matrix[1][2] += Matrix[1][1];

    Matrix[1][3] += 1.0f;
    Matrix[2][0] += Matrix[1][2]; 
    return Matrix[2][0];
}

void Hx_Circle() {}
void Hxs1_Circle() {}
void Hxs2_Circle() {}
void Hxs_FrBufferMorf2() {}
void Hxs_FrBufferMorf2B() {}
void Hx_Door() {}
void Hxs_GameOver() {}
void Hx_GameOver() {}
void Hxs_Logo_ExtraDraw() {}
void Hxs_Logo_TexSetup() {}
void Hxs_Logo_TexDraw() {}
void Hxs_Logo_MagDraw() {}
void Hxs_PenDraw() {}
void Hx_Logo() {}

int Hx_MovieStartSyncEx() {

    if (hx.m_0x11 != 0xC) return 0;

    if (hx.m_0x38 >= 2 && hx.m_0x38 <= 5) {

        if (hxs_logodraw_resetflag == 0) return 0;
        else {

            hxs_logodraw_resetflag = 0;
            return 1;
        }

    } else if (hx.m_0x38 >= 6) {

        if (hxs_logo_resetflag == 0) return 0;
        else {

            if (hx.m_0x38 == 6 && hx.Timer > 192) return 0;
            else {

                hxs_logo_resetflag = 0;
                return 2;
            }
        }

    } else return 0;
}

void Hx_Test1() {}
void Hxs1_Test1() {}
void Hx_Test2() {}
void Hx_Test2R() {}
void Hxs1_Test2() {}
void Hx_Test4() {}
void Hx_Test5() {}
