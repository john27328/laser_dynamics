#include "y.h"


Y::Y()
{
P=200;
}

double Y::f()
{
    dy3=K*y1-beta0*y1*y3-(W31+W32)*y3; //4-Й ЧЛЕН =0
    dy2=2*beta0*y1*y3+W32*y3-W21*y2-y0*(B_e*y2-B_a*y1);
    dy1=-beta0*y1*y3-K*y1+W31*y3+W21*y2+y0*(B_e*y2-B_a*y1);
    dy0=N_tm*Va*(y0+1)*(B_e*y2-B_a*y1)-y0*rho;
}



double Y::P_gen()
{
    //Pgen = gamma_2*c/2/Le*h*c/lambda_g*y(4,:);
    return gamma_2*c/2/Le*h*c/lambda_g*y0;
}

double Y::P_gen(double q)
{
    return gamma_2*c/2/Le*h*c/lambda_g*q;
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
    N_tm=4.1e26;
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
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N_tm*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N_tm*l*h*c*3.14*d*d);
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
    N_tm=7.8e26;
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
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N_tm*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N_tm*l*h*c*3.14*d*d);
}

double Y::f(int k, double x, double y0, double y1, double y2, double y3)
{
    double f0;
    if (k==0) f0=N_tm*Va*(y0+1)*(B_e*y2-B_a*y1)-y0*rho;
    if (k==1) f0=-beta0*y1*y3-K*y1+W31*y3+W21*y2+y0*(B_e*y2-B_a*y1);
    if (k==2) f0=2*beta0*y1*y3+W32*y3-W21*y2-y0*(B_e*y2-B_a*y1);
    if (k==3) f0=K*y1-beta0*y1*y3-(W31+W32)*y3;
    return f0;
}

void Y::kk(int j)
{
    int i;
    double x,h=H;
    x=h*j;
    for (i=0;i<=3;i++) k1[i]=f(i,x,yk[0],yk[1],yk[2],yk[3]);
    for (i=0;i<=3;i++) k2[i]=f(i,x+.5*h,yk[0]+.5*h*k1[0],yk[1]+.5*h*k1[1],yk[2]+.5*h*k1[2],yk[3]+.5*h*k1[3]);
    for (i=0;i<=3;i++) k3[i]=f(i,x+.5*h,yk[0]+.5*h*k2[0],yk[1]+.5*h*k2[1],yk[2]+.5*h*k2[2],yk[3]+.5*h*k2[3]);
    for (i=0;i<=3;i++) k4[i]=f(i,x+h,yk[0]+h*k1[0],yk[1]+h*k1[1],yk[2]+h*k1[2],yk[3]+h*k1[3]);
}

void Y::kke(int j)
{
     int i;
     double x,h=He;
     x=h*j;
     for (i=0;i<=3;i++) k1[i]=f(i,x,yke[1],yke[1],yke[2],yke[3]);
     for (i=0;i<=3;i++) k2[i]=f(i,x+.5*h,yke[0]+.5*h*k1[0],yke[1]+.5*h*k1[1],yke[2]+.5*h*k1[2],yke[3]+.5*h*k1[3]);
     for (i=0;i<=3;i++) k3[i]=f(i,x+.5*h,yke[0]+.5*h*k2[0],yke[1]+.5*h*k2[1],yke[2]+.5*h*k2[2],yke[3]+.5*h*k2[3]);
     for (i=0;i<=3;i++) k4[i]=f(i,x+h,yke[0]+h*k1[0],yke[1]+h*k1[1],yke[2]+h*k1[2],yke[3]+h*k1[3]);
}

void Y::fyk()
{
    int I,J;
    for (int K=0; K<=4;K++) y[K][0]=yk[1];
    for (J=1;J<=N;J++)
    {
            kk(J);
            for (I=0;I<=3;I++)
                    yk[I]+=H/6.*(k1[I]+2*k2[I]+2*k3[I]+k4[I]);
            for (int k=0; k<=4;k++) y[k][J]=yk[k];
    }
    for (int K=0; K<=4;K++) ye[K][0]=yke[1];
    for (J=1;J<=Ne;J++)
    {
            kke(J);
            for (I=0;I<=3;I++)
                   yke[I]+=He/6.*(k1[I]+2*k2[I]+2*k3[I]+k4[I]);
            for (int k=0; k<=4;k++) ye[k][J]=yke[k];
    }
    for (I=0;I<N;I++) if(fabs(y[0][I]-ye[0][2*I])>eps){k=1;printf("|%e-%e|=%e\n",y[0][I],ye[0][2*I],fabs(y[0][I]-ye[0][2*I]));}
}
