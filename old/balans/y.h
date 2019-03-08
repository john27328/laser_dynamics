#ifndef Y_H
#define Y_H

#include <math.h>
#include <QDebug>

class Y
{
public:
    Y();

    double y0;
    double y1;
    double y2;
    double y3;


    double dy0;
    double dy1;
    double dy2;
    double dy3;

    double beta0;
    double W31;
    double W32;
    double W21;
    double h;
    double c;
    double sigma_a_pump;
    double N;
    double n;
    double lambda_p;
    double lambda_g;
    double sigma_e_gen;
    double sigma_a_gen;
    double P;
    double d;
    double alpha_i;
    double beta_j;
    double L;
    double l;
    double Le;
    double Va;
    double V;
    double gamma_1;
    double gamma_2;
    double gamma_i;
    double rho;
    double B_e;
    double B_a;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    double K;

    double f();
    double P_gen();
    void tm_YLF_3pr();
    void tm_YAP_4pr();
};

#endif // Y_H
