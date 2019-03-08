#include "widget.h"
#include <math.h>
#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    double beta0 = 3500;
    double W31 = 360;
    double W32 = 40;
    double W21 = 67;
    double h = 6.62e-34;
    double c=3e8;
    double sigma_a_pump = 1e-24;
    double N=4.1e26;
    double n=1.7;
    double lambda_p=793e-9;
    double lambda_g=1.98e-6;
    double sigma_e_gen=0.25e-24;
    double sigma_a_gen= 0.04e-24;
    double P=400;
    double d=3e-3;
    double alpha_i = 1;
    double beta_j=0.2;
    double L=0.15;
    double l=0.015;
    double Le=L+(n-1)*l;
    double Va=3.14*d*d/4*l;
    double V=Le/l*Va;
    double gamma_1=-log(1-0.005);
    double gamma_2=-log(1-0.2);
    double gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
    double rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    double B_e=alpha_i*c/V*sigma_e_gen;
    double B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    double K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);

    const int i1 = 10000000;
    qDebug()<<i1*8*6*2/1024/1024 <<"MB";
    double dt = 0.0000000001;
    double di = i1/100.;
    QVector <double> y0;
    QVector <double> y1;
    QVector <double> y2;
    QVector <double> y3;
    QVector <double> y4;

    QVector <double> dy0;
    QVector <double> dy1;
    QVector <double> dy2;
    QVector <double> dy3;
   //double dy[5][i1];
    y0<<1; //q
    y1<<1;
    y2<<0;
    y3<<0;
    y4<<0;
    QVector <double> t;
    t<<0;
    qDebug()<<"t="<<dt*i1;
    for (int i=0;i<i1;i++)
    {
        dy3<<(K*y1[i]-beta0*y1[i]*y3[i]-(W31+W32)*y3[i])*dt; //4-Й ЧЛЕН =0
        dy2<<(2*beta0*y1[i]*y3[i]+W32*y3[i]-W21*y2[i]-y0[i]*(B_e*y2[i]-B_a*y1[i]))*dt;
        dy1<<(-beta0*y1[i]*y3[i]-K*y1[i]+W31*y3[i]+W21*y2[i]+y0[i]*(B_e*y2[i]-B_a*y1[i]))*dt;
        dy0<<(N*Va*(y0[i])*(B_e*y2[i]-B_a*y1[i])-y0[i]*rho)*dt;
        y1<<y1[i]+dy1[i];
        y2<<y2[i]+dy2[i];
        y3<<y3[i]+dy3[i];
        y0<<y0[i]+dy0[i];
        y4<<1-y1[i]-y2[i]-y3[i];
        t<<t[i]+dt;
        if ((i/di -(int)(i/di))==0)qDebug()<<"q"<<y0[i]<<"n1"<<y1[i]<<"n2"<<y2[i]<<"n3"<<y3[i]<<"41"<<y4[i]<<"t"<<t[i];
        if ((i/di -(int)(i/di))==0)qDebug()<<"dq"<<dy0[i]<<"dn1"<<dy1[i]<<"dn2"<<dy2[i]<<"dn3"<<dy3[i];
    }


}

Widget::~Widget()
{

}
