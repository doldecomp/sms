#include <dolphin.h>
#include <GC2D/hx_wiper.h>

int Hx_MovieStartSyncEx() {

    if (hx._0x11 != 0xC) return 0;
    if (hx._0x38 >= 2 && hx._0x38 <= 5) {

        if (hxs_logodraw_resetflag == 0) return 0;
        else {
            
            hxs_logodraw_resetflag = 0;
            return 1;
        }
        
    } else if (hx._0x38 >= 6) {

        if (hxs_logo_resetflag == 0) return 0;
        else {
            
            if (hx._0x38 == 6 && hx.Timer > 192) return 0;
            else {
                
                hxs_logo_resetflag = 0;
                return 2;
            }
        }
            
    } else return 0;
}

float Hx_MotionUpdate(Mtx Matrix) {

    if (Matrix[0][0] > Matrix[1][3]) Matrix[1][2] += Matrix[0][3];
    else if (Matrix[0][1] <= Matrix[1][3]) Matrix[1][2] += Matrix[1][1];

    Matrix[1][3] += 1.0f;
    Matrix[2][0] += Matrix[1][2]; 
    return Matrix[2][0];
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

unsigned int Hx_TimerCountDown() {

    if (hx.Timer != 0) hx.Timer--;
    return hx.Timer;
}

void dummy_handler() {

}

void Hx_RemoveResource() {

    if (hx._0x10 == 2) Hx_Warning(1);
    if (hx._0x24 == 0) Hx_Warning(2);
    hx._0x24 = 0;
    hx._0x28 = 0;
}

void Hx_ProvideResourceEx(int Param_1) {

    if (hx._0x10 == 2) Hx_Warning(1);
    hx._0x28 = 1;
    hx._0x30 = Param_1;
}

void Hx_ProvideResource(int Param_1, int Param_2) {

    if (hx._0x10 == 2) Hx_Warning(1);
    if (hx._0x24 != 0) Hx_Warning(3);
    hx._0x24 = 1;
    hx._0x2C = Param_1;
    hx._0x34 = Param_2;
}

void Hx_ResetWipe(int Param_1, int Param_2) {

    hx._0x10 = 0;
    hx._0x0 = Param_1;
    hx._0x4 = Param_2;
    hx._0x8 = hx._0x0 / 2;
    hx._0xC = hx._0x4 / 2;
    hx._0x24 = 0;
    hx._0x28 = 0;
}

int HxWarning(int Param_1) {

    return Param_1;

}