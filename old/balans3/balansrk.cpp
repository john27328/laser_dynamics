#include "balansrk.h"


BalansRK::BalansRK()
{
    //qDebug()<<"BalansRK";
    constants();
    variables();
    nDots = 10000;
    t_end = 2.1;
    Pgraf = new grafSource(0);
    Pgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y1graf = new grafSource(0);
    y1graf->pen=QPen(Qt::red,2,Qt::SolidLine);
    y2graf = new grafSource(0);
    y2graf->pen=QPen(Qt::green,2,Qt::SolidLine);
    y3graf = new grafSource(0);
    y3graf->pen=QPen(Qt::blue,2,Qt::SolidLine);
    timer = new QTimer();
    //qDebug()<<"BalansRK end";
}

void BalansRK::set_y0(double Y0, double Y1, double Y2, double Y3)
{
    //qDebug()<<"sey_y0";
    y0[0]=Y0; y0[1]=Y1; y0[2]=Y2; y0[3]=Y3;
    //qDebug()<<"set_y0";
}


void BalansRK::constants()
{
    //qDebug()<<"constants";
#define ADD_CONST(name, value, dimConst, dimDisp, ratioDisplayToConst,visible) \
    cMap[(#name)] = new Constants((&name),(#name),(dimConst),(dimDisp),(ratioDisplayToConst),(visible));
//expression ADD_VAR(name,expression, dimConst, dimDisp, ratioDisplayToConst)
#define ADD_VAR(name,expression, dimConst) \
    cMap[(#name)] = new Constants((&name),(#name),(dimConst),(dimConst),(1),(0));
#include"const.cpp"
    //qDebug()<<"constants end";

}

void BalansRK::variables()
{
#define ADD_CONST(name, value, dimConst, dimDisp, ratioConstToDim,visible)

#define ADD_VAR(name,expression, dimConst) \
    name = expression;
#include"const.cpp"
}


double BalansRK::cc(QString name)
{
    //qDebug()<<"c";
    if (cMap[name]==NULL)
        qDebug()<<name<<" = NULL";
    else
        return cMap[name]->value();
}

double BalansRK::setMasC(QString name, int i)
{
    qDebug()<<"setMasC"<<i<<jC<<name<<"="<<cc(name);
    return masC[i][jC++] = cc(name);
}

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
        kk1[i]=dyk(i,t,            yk, 0,      kk1);
    for (int i=0; i<NY; i++)
        kk2[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk1);
    for (int i=0; i<NY; i++)
        kk3[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk2);
    for (int i=0; i<NY; i++)
        kk4[i]=dyk(i,t + ht,       yk, ht,     kk3);
}

double BalansRK::P_gen()
{
    return P_gen(yk[0]);
}

double BalansRK::P_gen(double q)
{
    //qDebug()<<"P_gen";
    return 0;//cc("gamma_2") * cc("c") / 2. / cc("Le") * cc("h") * cc("c") / cc("lambda_g") * q;
}


void BalansRK::start_fyk(double ht)
{
    variables();
    //qDebug()<<"start_fyk";
    startK=1;
    //timer->start(1000);
    QTime time; time=QTime::currentTime();
    int t1=0;
    t1=time.elapsed();
     k_max=t_end/ht;
     qDebug()<<"t_end "<<t_end <<"ht"<<ht;
    double dk = k_max/((double)nDots);

    for (int i=0; i<NY; i++)
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
        f_kk(t,ht);
        t+=ht;
        for(int i=0;i<NY;i++)
        {
            yk[i]+=ht / 6. * (kk1[i] + 2 * kk2[i] + 2 * kk3[i] + kk4[i]);
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

void BalansRK::stop_fyk()
{
    startK = 0;
    qDebug()<<"stop_fyk";
}


#include "fyk.cpp"
