#include "balansrk.h"

#define n1 y[1]
#define n2 y[2]
#define q y[0]
#define PRINT(name) //qDebug()<<#name<<name;

double BalansRK::dyk(int ny, double t, double* yfk, double dt, double *kf)
{

    double y[NY];

    for(int i=0; i<NY; i++) y[i] = yfk[i] + dt * kf[i];

    switch (ny) {
    case 2:
        return R0n * n1 - A21 * n2 - K_ap_n * pow(n2,2) - q * q_pump * (B_g * n2 - B_a * n1); //R0n домножил на n1
        break;
    case 1:
        return -R0n * n1 + A21 * n2 + K_ap_n * pow(n2,2) + q * q_pump * (B_g * n2 - B_a * n1);
        break;
    case 0:
        return N0 * V_a * (q + 1/q_pump) * (B_g * n2 - B_a * n1) - q * rho;
        break;
    default:
        break;
    }

}
