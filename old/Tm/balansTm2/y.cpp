#include "y.h"
#include <QApplication>

Y::Y()
{
    P=200; N_pr=3; T_out=0.2; t_end=0.16;
    nDots = 2000;
    Pgraf = new grafSource(0);
    Pgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y1graf = new grafSource(0);
    y2graf = new grafSource(0);
    y3graf = new grafSource(0);
    timer = new QTimer();
}



double Y::dyk(int ny, double t, double yk0, double yk1, double yk2, double yk3)
{
    switch (ny) {
    case 3:
        return K*yk1-beta0*yk1*yk3-(W31+W32)*yk3;
        break;
    case 2:
        return 2*beta0*yk1*yk3+W32*yk3-W21*yk2-yk0*(B_e*yk2-B_a*yk1);
        break;
    case 1:
        return -beta0*yk1*yk3-K*yk1+W31*yk3+W21*yk2+yk0*(B_e*yk2-B_a*yk1);
        break;
    case 0:
        return N*Va*(yk0+1)*(B_e*yk2-B_a*yk1)-(yk0)*rho;
        break;
    default:
        break;
    }

}

double Y::getT_end() const
{
    return t_end;
}

void Y::setT_end(double value)
{
    t_end = value;
}

void Y::kk(double t, double ht)
{
    for (int i=0; i<Ny; i++)
        k1[i]=dyk(i,t,yk[0],yk[1],yk[2],yk[3]);
    for (int i=0; i<Ny; i++)
        k2[i]=dyk(i,t+0.5*ht,yk[0]+0.5*ht*k1[0],yk[1]+0.5*ht*k1[1],yk[2]+0.5*ht*k1[2],yk[3]+0.5*ht*k1[3]);
    for (int i=0; i<Ny; i++)
        k3[i]=dyk(i,t+0.5*ht,yk[0]+0.5*ht*k2[0],yk[1]+0.5*ht*k2[1],yk[2]+0.5*ht*k2[2],yk[3]+0.5*ht*k2[3]);
    for (int i=0; i<Ny; i++)
        k4[i]=dyk(i,t+ht,    yk[0]+ht*k3[0],    yk[1]+ht*k3[1],    yk[2]+ht*k3[2],    yk[3]+ht*k3[3]);
}

double Y::getD() const
{
    return d;
}

void Y::setD(double value)
{
    d = value;
}

double Y::getL() const
{
    return l;
}

void Y::setL(double value)
{
    l = value;
}

int Y::getNDots() const
{
    return nDots;
}

void Y::setNDots(int value)
{
    nDots = value;
}

double Y::getT() const
{
    return t;
}

void Y::setT(double value)
{
    t = value;
}

double Y::getT_out() const
{
    return T_out;
}

void Y::setT_out(double value)
{
    T_out = value;
}

double Y::P_gen()
{
    //qDebug()<<yk[0];
    return P_gen(yk[0]);
}

void Y::fyk(double ht)
{
    startK=1;
    //timer->start(1000);
    QTime time; time=QTime::currentTime();
    int t1=0;
    t1=time.elapsed();
     k_max=t_end/ht;
     qDebug()<<"t_end "<<t_end;
    double dk = k_max/((double)nDots);

    for (int i=0; i<Ny; i++)
        yk[i]=y0[i];
    t=0;
    Pgraf->clear();

    for (int k=1; (k<k_max+1)&&(startK); k++)
    {
        //qDebug()<<k<<fabs(k/dk -(int)(k/dk))<<k/dk<<(int)(k/dk);

        //сохранение
        if (fabs(k/dk -(int)(k/dk))==0)
        {
            qDebug()<<(int)(k/dk)<<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]<<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()<<"k"<<k;//<<"timer"<<timer->timerId();
            Pgraf->addPoint(t,P_gen(yk[0]));
            if  ((time.elapsed()-t1)>100)
            { //qDebug()<<2<<t;
                QApplication::processEvents();
                emit grafUpdate();
                t1=time.elapsed();
            }
        }


        //метод рк4
        kk(t,ht);
        t+=ht;
        for(int i=0;i<Ny;i++)
        {
            yk[i]+=ht/6.*(k1[i]+2*k2[i]+2*k3[i]+k4[i]);
        }
        if (yk[0]!=yk[0])
        {
            startK=0;
            QApplication::processEvents();
            emit grafUpdate();
            t1=time.elapsed();
        } //проверка на NaN
    }
}

double Y::getN_pr() const
{
    return N_pr;
}

void Y::setN_pr(double value)
{
    N_pr = value;
}

double Y::getP() const
{
    return P;
}

void Y::setP(double value)
{
    P = value;
}

void Y::tm_YLF_3pr()
{
    qDebug()<<"Tm:YLF";
    beta0 = 3500/3*N_pr;
    W31 = 360;
    W32 = 40;
    W21 = 67;
    h = 6.62e-34;
    c=3e8;
    sigma_a_pump = 1e-24;
    N=4.1e26/3*N_pr; //3%
    n=1.7;
    lambda_p=793e-9;
    lambda_g=1.908e-6;
    sigma_e_gen=0.25e-24;
    sigma_a_gen= 0.04e-24;
    //P=200;
    //d=3e-3;
    alpha_i = 1;
    beta_j=0.2;
    L=0.15;
    //l=0.042;
    Le=L+(n-1)*l;
    Va=3.14*d*d/4*l;
    V=Le/l*Va;
    gamma_1=-log(1-0.005);
    gamma_2=-log(1-T_out/100.);
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);
}

void Y::tm_YAP_4pr()
{
    qDebug()<<"Tm:YAP";
    beta0 = 3500/4.1*7.8/4*N_pr;
    W31 = 964;
    W32 = 126;
    W21 = 207;
    h = 6.62e-34;
    c=3e8;
    //sigma_a_pump = 1e-24;
    N=7.8e26/4*N_pr;
    n=1.93;
    lambda_p=793e-9;
    lambda_g=1.930e-6;
    sigma_e_gen=0.5e-24;  //?
    sigma_a_gen= 0.03e-24; //?
    //P=200;
    //d=3e-3;
    alpha_i = 1;
    beta_j=0.2;
    L=0.15;
    //l=0.042;
    Le=L+(n-1)*l;
    Va=3.14*d*d/4*l;
    V=Le/l*Va;
    gamma_1=-log(1-0.005);
    gamma_2=-log(1-T_out/100);
    gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);
}

double Y::P_gen(double q)
{
    //Pgen = gamma_2*c/2/Le*h*c/lambda_g*y(4,:);
    return gamma_2*c/2./Le*h*c/lambda_g*q;
}

void Y::set_y0(double Y0, double Y1, double Y2, double Y3)
{
    y0[0] = Y0;
    y0[1] = Y1;
    y0[2] = Y2;
    y0[3] = Y3;
}

void Y::stop_fyk()
{
    startK = 0;
}


void Y::setParametri(double P_Nak, double T_Out, double N_Pr, double t_End)
{
    P=P_Nak;
    T_out=T_Out;
    N_pr=N_Pr;
    t_end=t_End;
}
