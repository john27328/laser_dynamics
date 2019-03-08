#include "balansrk.h"
#define y3 y[3]
#define y2 y[2]
#define y1 y[1]

double BalansRK::dyk(int ny, double t, double *yfk, double dt, double *kf)
{

    double y[NY];
    double yf;

    for(int i=0; i<NY; i++) y[i] = yfk[i] + dt * kf[i];

    switch (ny) {
    case 3:
        return t*exp(-t) - A*y3 - 3*y2 - y1;
        break;
    case 2:
        return y3;
        break;
    case 1:
        return y2;
        break;
    case 0:
        return 0;
        break;
    default:
        break;
    }
}
