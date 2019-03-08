#include "balansrk.h"


BalansRK::BalansRK()
{
    //qDebug()<<"BalansRK";
    constants();
    variables();
    nDots = 100000;
    t_end = 2.1;
    Pgraf = new grafSource(0);
    Pgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    PPumpGraf = new grafSource(0);
    PPumpGraf->pen=QPen(Qt::blue,3,Qt::SolidLine);
    y1graf = new grafSource(0);
    y1graf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y2graf = new grafSource(0);
    y2graf->pen=QPen(Qt::green,2,Qt::SolidLine);
    y3graf = new grafSource(0);
    y3graf->pen=QPen(Qt::blue,2,Qt::SolidLine);
    y4graf = new grafSource(0);
    y4graf->pen=QPen(Qt::darkMagenta,2,Qt::SolidLine);
    timer = new QTimer();
    //qDebug()<<"BalansRK end";

    for(int i = 0; i < NY; i++) kk0[i] = 0;
}

void BalansRK::set_y0(double Y0, double Y1, double Y2)
{
    //qDebug()<<"sey_y0";
    y0[0]=Y0; y0[1]=Y1; y0[2]=Y2;
    //qDebug()<<"set_y0";
}


void BalansRK::constants()
{
    //qDebug()<<"constants";
#define ADD_CONST(name, value, dimDisp,dimConst,  ratioDisplayToConst,visible) \
    cMap[(#name)] = new Constants((&name),(#name),(dimConst),(dimDisp),(ratioDisplayToConst),(visible));
#define ADD_VAR(name,expression, dimConst) \
    cMap[(#name)] = new Constants((&name),(#name),(dimConst),(dimConst),(1),(0));
#include"const.cpp"
    //qDebug()<<"constants end";

}

void BalansRK::variables()
{
#define ADD_CONST(name, value, dimDisp, dimConst,  ratioConstToDim,visible)

#define ADD_VAR(name,expression, dimConst) \
    name = expression;
#include"const.cpp"
}


//double BalansRK::cc(QString name)
//{
//    //qDebug()<<"c";
//    if (cMap[name]==NULL)
//        qDebug()<<name<<" = NULL";
//    else
//        return cMap[name]->value();
//}

//double BalansRK::setMasC(QString name, int i)
//{
//    qDebug()<<"setMasC"<<i<<jC<<name<<"="<<cc(name);
//    return masC[i][jC++] = cc(name);
//}

double BalansRK::setC(QString name,double value)
{
    //qDebug()<<"setC";
    if (cMap[name]==NULL)
        qDebug()<<name<<" = NULL";
    else
        cMap[name]->setValue(value);
}

void BalansRK::f_kk(double t, double ht)
{
    //qDebug()<<"f_kk";
    for (int i=0; i<NY; i++)
        kk1[i]=dyk(i,t,            yk, 0,      kk0);
    for (int i=0; i<NY; i++)
        kk2[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk1);
    for (int i=0; i<NY; i++)
        kk3[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk2);
    for (int i=0; i<NY; i++)
        kk4[i]=dyk(i,t + ht,       yk, ht,     kk3);
}

double BalansRK::getEGen() const
{
    return EGen;
}

double BalansRK::getEPump() const
{
    return EPump;
}

void BalansRK::setT_end(double value)
{
    t_end = value;
}

double BalansRK::P_gen()
{
    return P_gen(yk[0]);
}

double BalansRK::getT()
{
    return t;
}

#include "p_gen.cpp"

void BalansRK::setNDots(int value)
{
    nDots = value;
}


void BalansRK::start_fyk(double ht)
{
    variables();
    //qDebug()<<"start_fyk";
    startK=1;
    QTime time; time=QTime::currentTime();
    int tTemp=0;
    tTemp=time.elapsed();
     k_max=t_end/ht;
    double dk = k_max/((double)nDots);
    double kTemp = 0;

    //задаю НУ
    for (int i=0; i<NY; i++)
        yk[i]=y0[i];


    t=0;
    Pgraf->clear();
    PPumpGraf->clear();
    y1graf->clear();
    y2graf->clear();
    y3graf->clear();
    y4graf->clear();


    EGen=EPump=0;
    double PPump = P;
    double PGen = P_gen(yk[0]);
    double dkTemp = 0;
    PPumpGraf->addPoint(0,PPump);
    PPumpGraf->addPoint(t_end,PPump);

    y1graf->addPoint(0,y0[1]);

    //график порога
    //(AA * (Alpha * y2 - Beta * y1) * (y0 + 1) - BB * gamma * y0 )
    double DeltaNPor = rho / V_a / N0 / B_a;
    qDebug()<<DeltaNPor;
    y4graf->addPoint(0,DeltaNPor);
    y4graf->addPoint(t_end,DeltaNPor);
    //qDebug()<<2;
    for (int k=1; (k<k_max+1)&&(startK); k++)
    {
        //сохранение
        PGen = P_gen(yk[0]);
//        if (PGen>1) qDebug()<<dyk(0,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(1,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(2,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(3,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]
//                <<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()
//                             <<"k"<<k;//<<"timer"<<timer->timerId();
        if (PGen<1e-3 && nDots>1000)
            dkTemp=dk*(nDots/1000. < 1000 ? nDots/1000. : 1000 );
        else
            dkTemp=dk;
        if (kTemp++>dkTemp-1)
        {
            kTemp = 0;
            qDebug()<<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]<<"t="<<t<<"time"<<time.elapsed()<<"k"<<k;//<<"timer"<<timer->timerId();

            Pgraf->addPoint(t,PGen);

            y1graf->addPoint(t,yk[1]);
            y2graf->addPoint(t,yk[2]);
            y3graf->addPoint(t,(B_g * yk[2] - B_a * yk[1]) / B_a);
            //qDebug()<<N*yk[1]<<N*yk[2]<<N*((sigma_e_gen*yk[2]-sigma_a_gen*yk[1]))/sigma_e_gen<<N<<yk[2]<<yk[1]<<sigma_e_gen*yk[2]<<sigma_a_gen*yk[1];

            if  ((time.elapsed()-tTemp)>100)
            { //qDebug()<<2<<t;
                QApplication::processEvents();
                emit grafUpdate();
                tTemp=time.elapsed();
            }
        }

        //метод рк4

        t+=ht;
        f_kk(t,ht);

        //PPump = Pump_Power*(t<TPump ? 1:0);
        PGen = P_gen(yk[0]);
        double dy;
        for(int i=0;i<NY;i++)
        {
            dy = ht / 6. * (kk1[i] + 2 * kk2[i] + 2 * kk3[i] + kk4[i]);
            //yk[i]+= fabs(dy) < 1e-8 ? 0 : dy;
            yk[i]+=dy;
        }

        //проверка на NaN
        if (yk[0]!=yk[0])
        {
            while ((time.elapsed()-tTemp)<200);
            tTemp=time.elapsed();
            startK=0;
            QApplication::processEvents();
            emit grafUpdate();
            tTemp=time.elapsed();
            while ((time.elapsed()-tTemp)<100);
            QMessageBox::warning(0,"Warning", "уравнение расходится, увеличте шаг");
            PGen = 0;
        }

        //рассчет энергии
        EGen+=ht*PGen;
        EPump+=ht*PPump;
    }
    while ((time.elapsed()-tTemp)<100);
    emit grafUpdate();
}

void BalansRK::stop_fyk()
{
    startK = 0;
    //qDebug()<<"stop_fyk";
}


#include "fyk.cpp"
