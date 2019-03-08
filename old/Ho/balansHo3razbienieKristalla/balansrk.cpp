#include "balansrk.h"

#define PRINT(name) qDebug()<<#name<<name;

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

    for(int i = 0; i < NY; i++)
        for(int j = 0; j < NKr; j++)
            kk0[i][j] = 0;
}

void BalansRK::set_y0(double Y0, double Y1, double Y2)
{
    //qDebug()<<"sey_y0";
    for (int j = 0; j < NKr; j++)
    {
        y0[0][j]=Y0; y0[1][j]=Y1; y0[2][j]=Y2;
    }
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

    //разбиваю мощность

    double PTemp = P;

    PRINT(PTemp)
    //прямой проход

    for(int i = 0; i < NKr; i++)
    {
        P_pump[i] = PTemp*(1-exp( - K_sigma * l));
        PTemp -= P_pump[i];
        //PRINT(PTemp)
    }
PRINT(PTemp)
        PRINT(T_doj)
    PTemp = PTemp * T_doj;

    PRINT(PTemp)

    for(int i = NKr - 1; i >= 0; i--)
    {
        P_pump[i] = PTemp*(1-exp( - K_sigma * l));
        PTemp -= P_pump[i];
        //PRINT(PTemp)
    }
PRINT(PTemp)


    for(int i = 0; i < NKr; i++)
    {
        R0n[i] = P_pump[i] / (S * l * h * c / lambda_pump);
        R0n[i] = R0n[i]/N0;
    }

    //отладка




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
    for (int j=0; j<NKr; j++)
    {
        for (int i=0; i<NY; i++)
            kk1[i][j]=dyk(i, j, t,            yk, 0,      kk0);
        for (int i=0; i<NY; i++)
            kk2[i][j]=dyk(i, j, t + 0.5 * ht, yk, 0.5*ht, kk1);
        for (int i=0; i<NY; i++)
            kk3[i][j]=dyk(i, j, t + 0.5 * ht, yk, 0.5*ht, kk2);
        for (int i=0; i<NY; i++)
            kk4[i][j]=dyk(i, j, t + ht,       yk, ht,     kk3);
    }
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
    return P_gen(yk[0][0]);
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
    for (int j=0; j< NKr; j++)
        for (int i=0; i<NY; i++)
            yk[i][j]=y0[i][j];


    t=0;
    Pgraf->clear();
    PPumpGraf->clear();
    y1graf->clear();
    y2graf->clear();
    y3graf->clear();
    y4graf->clear();


    EGen=EPump=0;
    double PPump = P;
    double PGen = P_gen(yk[0][0]);
    double dkTemp = 0;
    PPumpGraf->addPoint(0,PPump);
    PPumpGraf->addPoint(t_end,PPump);

    y1graf->addPoint(0,y0[1][0]);

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
        PGen = P_gen(yk[0][0]);
//        if (PGen>1) qDebug()<<dyk(0,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(1,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(2,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(3,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]
//                <<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()
//                             <<"k"<<k;//<<"timer"<<timer->timerId();
        // оптимизирую шаг, делаю меньше точек, пока мощности нет
        if (PGen<1e-3 && nDots>1000)
            dkTemp=dk*(nDots/1000. < 1000 ? nDots/1000. : 1000 );
        else
            dkTemp=dk;
        if (kTemp++>dkTemp-1)
        {
            kTemp = 0;
            //qDebug()<<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]<<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()<<"k"<<k;//<<"timer"<<timer->timerId();

            Pgraf->addPoint(t,PGen);

            y1graf->addPoint(t,yk[1][0]);
            y2graf->addPoint(t,yk[2][0]);
            y3graf->addPoint(t,(B_g * yk[2][0] - B_a * yk[1][0]) / B_a);
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
        PGen = P_gen(yk[0][0]);
        double dy, dq=0;
        for(int j=0; j<NKr; j++)
        {
            for(int i=1; i<NY; i++)
            {
                dy = ht / 6. * (kk1[i][j] + 2 * kk2[i][j] + 2 * kk3[i][j] + kk4[i][j]);
                //yk[i]+= fabs(dy) < 1e-8 ? 0 : dy;
                yk[i][j]+=dy;
            }
            dq += ht / 6. * (kk1[0][j] + 2 * kk2[0][j] + 2 * kk3[0][j] + kk4[0][j]);
        }

        for(int j=0; j<NKr; j++)
        {
            yk[0][j] += dq;
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
