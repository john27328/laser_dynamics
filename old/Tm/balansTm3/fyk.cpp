#include "balansrk.h"

double BalansRK::dyk(int ny, double t, double y0, double y1, double y2, double y3)
{
    double rhoQS;
    if((t*fQS - (int)(t*fQS))<tOpenQS*fQS)
        rhoQS=rho;
    else
        rhoQS=rho*10;
    //qDebug()<<t*fQS - (int)(t*fQS)<<tOpenQS*fQS<<t<<rhoQS/rho;

    switch (ny) {
    case 3:
        return K*y1-beta*y1*y3-(W31+W32)*y3;
        break;
    case 2:
        return 2*beta*y1*y3+W32*y3-W21*y2-y0*(B_e*y2-B_a*y1);
        break;
    case 1:
        return -beta*y1*y3-K*y1+W31*y3+W21*y2+y0*(B_e*y2-B_a*y1);
        break;
    case 0:
        return N * Va * (y0 + 1) * (B_e * y2 - B_a * y1) - ( y0 ) * rhoQS;
        break;
    default:
        break;
    }
}
