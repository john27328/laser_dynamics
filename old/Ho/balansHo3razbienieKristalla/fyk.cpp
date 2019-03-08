#include "balansrk.h"

#define n1 y[1]
#define n2 y[2]
#define q y[0]
#define PRINT(name) //qDebug()<<#name<<name;


#define NY 3  //число уравнений
#define NKr 10 // число разбиений кр

double BalansRK::dyk(int ny, int nKr, double t, double yfk[NY][NKr], double dt, double kf[NY][NKr])
{

    double y[NY];

    for(int i=0; i<NY; i++) y[i] = yfk[i][nKr] + dt * kf[i][nKr];

    switch (ny) {
    case 2:
        return R0n[nKr] - A21 * n2 - K_ap_n * pow(n2,2) - q_pump * q * (B_g * n2 - B_a * n1);
        break;
    case 1:
        return -R0n[nKr] + A21 * n2 + K_ap_n * pow(n2,2) + q_pump * q * (B_g * n2 - B_a * n1);
        break;
    case 0:
        return N0 * V_a * (q + 1 / q_pump) * (B_g * n2 - B_a * n1) - q * rho;
        break;
    default:
        return 0;
        break;

    }

}
