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
    double N_tm;
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
    double P_gen(double q);
    void tm_YLF_3pr();
    void tm_YAP_4pr();

    // метод рунге кутта
    const double eps=.01,x_k=5;
    const double H =.01; //шаг
    const  double He=H/2.; //удвоеный шаг
    static const int Nm=1000; //максимальный размер массива
    int N=x_k/H,Ne=x_k/He; //число точек
    double Nd=N;
    double yk[3+1],y[3+1][Nm+1],yke[3+1],ye[3+1][2*Nm+1];
    double k1[3+1],k2[3+1],k3[3+1],k4[3+1];
    int k=0;

    double f(int k, double x, double y0, double y1, double y2, double y3);

    void kk (int j);

    void kke (int j);

    void fyk();


};

#endif // Y_H
