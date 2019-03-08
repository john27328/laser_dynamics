#include "y.h"
#include <qdebug.h>
Y::Y()
{   //qDebug()<<"add Y";
P=200;
}


double Y::f()
{
    dy3=K*y1-beta0*y1*y3-(W31+W32)*y3;
    dy2=2*beta0*y1*y3+W32*y3-W21*y2-y0*(B_e*y2-B_a*y1);
    dy1=-beta0*y1*y3-K*y1+W31*y3+W21*y2+y0*(B_e*y2-B_a*y1);
    dy0=N*Va*(y0+1)*(B_e*y2-B_a*y1)-y0*rho;
}



double Y::P_gen()
{
    //Pgen = gamma_2*c/2/Le*h*c/lambda_g*y(4,:);
    return gamma_2*c/2/Le*h*c/lambda_g*y0;
}

void Y::tm_YLF_3pr()
{
    beta0 = 3500;
    W31 = 360;
    W32 = 40;
    W21 = 67;
    h = 6.62e-34;
    c=3e8;
    sigma_a_pump = 1e-24;
    N=4.1e26;
    n=1.7;
    lambda_p=793e-9;
    lambda_g=1.908e-6;
    sigma_e_gen=0.25e-24;
    sigma_a_gen= 0.04e-24;
    //P=200;
    d=3e-3;
    alpha_i = 1;
    beta_j=0.2;
    L=0.15;
    l=0.042;
    Le=L+(n-1)*l;
    Va=3.14*d*d/4*l;
    V=Le/l*Va;
    gamma_1=-log(1-0.005);
    gamma_2=-log(1-0.2);
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);
}

void Y::tm_YAP_4pr()
{
    beta0 = 3500/4.1*7.8;
    W31 = 964;
    W32 = 126;
    W21 = 207;
    h = 6.62e-34;
    c=3e8;
    //sigma_a_pump = 1e-24;
    N=7.8e26;
    n=1.93;
    lambda_p=793e-9;
    lambda_g=1.930e-6;
    sigma_e_gen=0.25e-24; //
    sigma_a_gen= 0.04e-24;
    //P=200;
    d=3e-3;
    alpha_i = 1;
    beta_j=0.2;
    L=0.15;
    l=0.042;
    Le=L+(n-1)*l;
    Va=3.14*d*d/4*l;
    V=Le/l*Va;
    gamma_1=-log(1-0.005);
    gamma_2=-log(1-0.2);
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);
}
