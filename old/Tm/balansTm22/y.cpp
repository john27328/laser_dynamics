#include "y.h"
#include <QApplication>

Y::Y()
{
    P=200; N_pr=3; T_out=0.2; t_end=0.16;
    nDots = 2000;
    Pgraf = new grafSource(0);
    Pgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y1graf = new grafSource(0);
    y1graf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y2graf = new grafSource(0);
    y2graf->pen=QPen(Qt::green,2,Qt::SolidLine);
    y3graf = new grafSource(0);
    y3graf->pen=QPen(Qt::blue,2,Qt::SolidLine);
    timer = new QTimer();
}



double Y::dyk(int ny, double t, double yk0, double yk1, double yk2, double yk3)
{
    switch (ny) {
    case 3:
        return K*yk1-beta*yk1*yk3-(W31+W32)*yk3;
        break;
    case 2:
        return 2*beta*yk1*yk3+W32*yk3-W21*yk2-yk0*(B_e*yk2-B_a*yk1);
        break;
    case 1:
        return -beta*yk1*yk3-K*yk1+W31*yk3+W21*yk2+yk0*(B_e*yk2-B_a*yk1);
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

void Y::setSigma_a_gen(double value)
{
    sigma_a_gen = value;
}

double Y::qk()
{
    return yk[0];
}

double Y::y1k()
{
    return yk[1];
}

double Y::y2k()
{
    return yk[2];
}

double Y::y3k()
{
    return yk[3];
}

void Y::setSigma_e_gen(double value)
{
    sigma_e_gen = value;
}

void Y::setW21(double value)
{
    W21 = value;
}

void Y::setW32(double value)
{
    W32 = value;
}

void Y::setW31(double value)
{
    W31 = value;
}

double Y::getBeta_j() const
{
    return beta_j;
}

double Y::getAlpha_i() const
{
    return alpha_i;
}

double Y::getLambda_g() const
{
    return lambda_g;
}

double Y::getLambda_p() const
{
    return lambda_p;
}

double Y::getSigma_a_gen() const
{
    return sigma_a_gen;
}

double Y::getSigma_e_gen() const
{
    return sigma_e_gen;
}

double Y::getLres() const
{
    return Lres;
}

void Y::setLres(double value)
{
    Lres = value;
}

double Y::getN() const
{
    return N;
}

double Y::getW21() const
{
    return W21;
}

double Y::getW32() const
{
    return W32;
}

double Y::getW31() const
{
    return W31;
}

double Y::getBeta() const
{
    return beta;
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
    y1graf->clear();
    y2graf->clear();
    y3graf->clear();



    for (int k=1; (k<k_max+1)&&(startK); k++)
    {
        //qDebug()<<k<<fabs(k/dk -(int)(k/dk))<<k/dk<<(int)(k/dk);

        //сохранение
        if (fabs(k/dk -(int)(k/dk))==0)
        {
            qDebug()<<(int)(k/dk)<<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]<<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()<<"k"<<k;//<<"timer"<<timer->timerId();
            Pgraf->addPoint(t,P_gen(yk[0]));
            y1graf->addPoint(t,yk[1]);
            y2graf->addPoint(t,yk[2]);
            y3graf->addPoint(t,yk[3]);

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
            QMessageBox::warning(0,"Warning", "уравнение расходится, увеличте шаг");
        } //проверка на NaN
    }
}

void Y::Tm_parameters()
{
    beta = beta0/3*N_pr;
    N=N0/3*N_pr; //3%

    Le=Lres+(n-1)*l;
    Va=3.14*d*d/4*l;
    V=Le/l*Va;
    gamma_1=-log(1-0.005);
    gamma_2=-log(1-T_out/100.);
    gamma_i=0;//-log(1-exp(-1/(sigma_a_gen*N*l)));
    rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
    B_e=alpha_i*c/V*sigma_e_gen;
    B_a=beta_j*c/V*sigma_a_gen;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    K=(lambda_p*P*4)/(N*l*h*c*3.14*d*d);
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

void Y::tm_YLF_3pr_default()
{
    qDebug()<<"Tm:YLF";
    beta0 = 3500;
    W31 = 360;
    W32 = 40;
    W21 = 67;
    h = 6.62e-34;
    c=3e8;
    sigma_a_pump = 1e-24;
    N0=4.1e26;
    n=1.7;
    lambda_p=793e-9;
    lambda_g=1.908e-6;
    sigma_e_gen=0.25e-24;
    sigma_a_gen= 0.04e-24;
    //P=200;
    //d=3e-3;
    alpha_i = 1;
    beta_j=0.2;
    Lres=0.15;
    //l=0.042;
    Tm_parameters();

}

void Y::tm_YAP_3pr_default()
{
    qDebug()<<"Tm:YAP";
    beta0 = 3500/4.1*7.8/4.*3.;
    W31 = 964;
    W32 = 126;
    W21 = 207;
    h = 6.62e-34;
    c=3e8;
    //sigma_a_pump = 1e-24;
    N0 = 7.8e26/4*3;
    n=1.93;
    lambda_p=793e-9;
    lambda_g=1.930e-6;
    sigma_e_gen=0.5e-24;  //?
    sigma_a_gen= 0.03e-24; //?
    //P=200;
    //d=3e-3;
    alpha_i = 1;
    beta_j=1;
    Lres=0.15;
    //l=0.042;
    Tm_parameters();
}

void Y::tm_YLF_3pr()
{
    QSettings *settings = new QSettings("./settings.conf",QSettings::IniFormat);

    tm_YLF_3pr_default();

    beta0 = settings->value("TmYLF/beta0",beta0).toDouble();
    W31 = settings->value("TmYLF/W31",W31).toDouble();
    W32 = settings->value("TmYLF/W32",W32).toDouble();
    W21 = settings->value("TmYLF/W21",W21).toDouble();
    h = settings->value("TmYLF/h",h).toDouble();
    c = settings->value("TmYLF/c",c).toDouble();
    sigma_a_pump = settings->value("TmYLF/sigma_a_pump",sigma_a_pump).toDouble();
    N0  = settings->value("TmYLF/N0",N0).toDouble();
    n = settings->value("TmYLF/n",n).toDouble();
    lambda_p = settings->value("TmYLF/lambda_p",lambda_p).toDouble();
    lambda_g = settings->value("TmYLF/lambda_g",lambda_g).toDouble();
    sigma_e_gen = settings->value("TmYLF/sigma_e_gen",sigma_e_gen).toDouble();
    sigma_a_gen = settings->value("TmYLF/sigma_a_gen",sigma_a_gen).toDouble();
    alpha_i = settings->value("TmYLF/alpha_i",alpha_i).toDouble();
    beta_j = settings->value("TmYLF/beta_j", beta_j).toDouble();\
    Lres = settings->value("TmYLF/L", Lres).toDouble();\

    //Tm_parameters();
}

void Y::tm_YAP_3pr()
{
    QSettings *settings = new QSettings("./settings.conf",QSettings::IniFormat);

    tm_YAP_3pr_default();

    beta0 = settings->value("TmYAP/beta0",beta0).toDouble();
    W31 = settings->value("TmYAP/W31",W31).toDouble();
    W32 = settings->value("TmYAP/W32",W32).toDouble();
    W21 = settings->value("TmYAP/W21",W21).toDouble();
    h = settings->value("TmYAP/h",h).toDouble();
    c = settings->value("TmYAP/c",c).toDouble();
    sigma_a_pump = settings->value("TmYAP/sigma_a_pump",sigma_a_pump).toDouble();
    N0  = settings->value("TmYAP/N0",N0).toDouble();
    n = settings->value("TmYAP/n",n).toDouble();
    lambda_p = settings->value("TmYAP/lambda_p",lambda_p).toDouble();
    lambda_g = settings->value("TmYAP/lambda_g",lambda_g).toDouble();
    sigma_e_gen = settings->value("TmYAP/sigma_e_gen",sigma_e_gen).toDouble();
    sigma_a_gen = settings->value("TmYAP/sigma_a_gen",sigma_a_gen).toDouble();
    alpha_i = settings->value("TmYAP/alpha_i",alpha_i).toDouble();
    beta_j = settings->value("TmYAP/beta_j", beta_j).toDouble();\
    Lres = settings->value("TmYAP/L", Lres).toDouble();\

    //Tm_parameters();
}

double Y::P_gen(double q)
{
    //Pgen = gamma_2*c/2/Le*h*c/lambda_g*y(4,:);
    return gamma_2*c/2./Le*h*c/lambda_g*q;
}

void Y::saveSettings()
{
    bool bOk;
    QString str = QInputDialog::getText(0,
                                        "input",
                                        "Чтобы пересохранить файл введите \"yes\" или \"да\"",
                                        QLineEdit::Normal,
                                        "",
                                        &bOk
                                        );
    if (str == "yes" || str == "да")
    {
        qDebug()<<"save";
        QSettings *settings = new QSettings("./settings.conf",QSettings::IniFormat);;

        tm_YLF_3pr_default();

        settings->setValue("TmYLF/beta0",beta0);
        settings->setValue("TmYLF/W31",W31);
        settings->setValue("TmYLF/W32",W32);
        settings->setValue("TmYLF/W21",W21);
        settings->setValue("TmYLF/h",h);
        settings->setValue("TmYLF/c",c);
        settings->setValue("TmYLF/sigma_a_pump",sigma_a_pump);
        settings->setValue("TmYLF/N0",N0);
        settings->setValue("TmYLF/n",n);
        settings->setValue("TmYLF/lambda_p",lambda_p);
        settings->setValue("TmYLF/lambda_g",lambda_g);
        settings->setValue("TmYLF/sigma_e_gen",sigma_e_gen);
        settings->setValue("TmYLF/sigma_a_gen",sigma_a_gen);
        settings->setValue("TmYLF/alpha_i",alpha_i);
        settings->setValue("TmYLF/beta_j", beta_j);\
        settings->setValue("TmYLF/L", Lres);\

        tm_YAP_3pr_default();

        settings->setValue("TmYAP/beta0",beta0);
        settings->setValue("TmYAP/W31",W31);
        settings->setValue("TmYAP/W32",W32);
        settings->setValue("TmYAP/W21",W21);
        settings->setValue("TmYAP/h",h);
        settings->setValue("TmYAP/c",c);
        settings->setValue("TmYAP/sigma_a_pump",sigma_a_pump);
        settings->setValue("TmYAP/N0",N0);
        settings->setValue("TmYAP/n",n);
        settings->setValue("TmYAP/lambda_p",lambda_p);
        settings->setValue("TmYAP/lambda_g",lambda_g);
        settings->setValue("TmYAP/sigma_e_gen",sigma_e_gen);
        settings->setValue("TmYAP/sigma_a_gen",sigma_a_gen);
        settings->setValue("TmYAP/alpha_i",alpha_i);
        settings->setValue("TmYAP/beta_j", beta_j);\
        settings->setValue("TmYAP/L", Lres);\
        settings->sync();
        endSaveSignal();
        QMessageBox::information(0,"information", "сохранение завершено");
    }

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
