#include "balansrk.h"


BalansRK::BalansRK()
{
    //qDebug()<<"BalansRK";
    constants();
    variables();
    nDots = 100000;
    t_end = 2.1;
    Pgraf = new grafSource(0);
    Pgraf->pen=QPen(Qt::red,3,Qt::SolidLine);
    PPumpGraf = new grafSource(0);
    PPumpGraf->pen=QPen(Qt::blue,3,Qt::SolidLine);
    y1graf = new grafSource(0);
    y1graf->pen=QPen(Qt::red,3,Qt::SolidLine);
    y2graf = new grafSource(0);
    y2graf->pen=QPen(Qt::green,3,Qt::SolidLine);
    y3graf = new grafSource(0);
    y3graf->pen=QPen(Qt::blue,3,Qt::SolidLine);
    y4graf = new grafSource(0);
    y4graf->pen=QPen(Qt::darkMagenta,3,Qt::SolidLine);
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
        kk[1][i]=dyk(i,t,yk[0],yk[1],yk[2],yk[3]);
    for (int i=0; i<NY; i++)
        kk[2][i]=dyk(i,t+0.5*ht,yk[0]+0.5*ht*kk[1][0],yk[1]+0.5*ht*kk[1][1],yk[2]+0.5*ht*kk[1][2],yk[3]+0.5*ht*kk[1][3]);
    for (int i=0; i<NY; i++)
        kk[3][i]=dyk(i,t+0.5*ht,yk[0]+0.5*ht*kk[2][0],yk[1]+0.5*ht*kk[2][1],yk[2]+0.5*ht*kk[2][2],yk[3]+0.5*ht*kk[2][3]);
    for (int i=0; i<NY; i++)
        kk[4][i]=dyk(i,t+ht,    yk[0]+ht*kk[3][0],    yk[1]+ht*kk[3][1],    yk[2]+ht*kk[3][2],    yk[3]+ht*kk[3][3]);
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

double BalansRK::P_gen(double q)
{
    //qDebug()<<"P_gen";
    return gamma_2 * c / 2. /Le * h *c / lambda_g * q;
}

void BalansRK::setNDots(int value)
{
    nDots = value;
}


void BalansRK::start_fyk(double ht)
{
    variables();
    //qDebug()<<"start_fyk";
    startK=1;
    //timer->start(1000);
    QTime time; time=QTime::currentTime();
    int tTemp=0;
    tTemp=time.elapsed();
     k_max=t_end/ht;
     //qDebug()<<1;
    double dk = k_max/((double)nDots);
    double kTemp = 0;
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
    double DeltaNPor = rho / Va / sigma_e_gen / c * V;
    qDebug()<<DeltaNPor;
    y4graf->addPoint(0,DeltaNPor);
    y4graf->addPoint(t_end,DeltaNPor);
    //qDebug()<<2;
    for (int k=1; (k<k_max+1)&&(startK); k++)
    {
        //qDebug()<<2.5;
        //qDebug()<<k<<fabs(k/dk -(int)(k/dk))<<k/dk<<(int)(k/dk);
        //сохранение
        //PPump = Pump_Power*(t<=TPump ? 1:0);
        PGen = P_gen(yk[0]);
//        if (PGen>1) qDebug()<<dyk(0,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(1,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(2,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<dyk(3,t,yk[0],yk[1],yk[2],yk[3], yk[4])
//                <<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]
//                <<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()
//                             <<"k"<<k;//<<"timer"<<timer->timerId();
        //снижение детализации, если нет мощности
        double nDots_simp=1000;
        if (PGen<1e-3 && nDots>nDots_simp)
            dkTemp=dk*(nDots/nDots_simp < nDots_simp ? nDots/nDots_simp : nDots_simp );
        else
            dkTemp=dk;
        bool kSave=0;
        if (kTemp++>dkTemp-1)
        {
            kTemp = 0;
            kSave = 1;
        }

        //пикдетектор
        static double PgenPr=0, PgenMax=0;
                //qDebug()<<PgenPr<<PgenMax<<PGen;
        if(PGen>1e-3 && PGen<PgenMax && PgenMax>= PgenPr) {kSave = 1; qDebug()<<"!!"<<PgenPr<<PgenMax<<PGen;}
        PgenPr=PgenMax; PgenMax=PGen;

        if(kSave)
        {
            PGen=PgenPr;
            kSave = 0;
            //qDebug()<<"P="<<P_gen(yk[0])<<yk[0]<<yk[1]<<yk[2]<<yk[3]<<yk[4]<<"t="<<t<<"time"<<time.elapsed()<<"k"<<k;//<<"timer"<<timer->timerId();

            Pgraf->addPoint(t,PGen);

            y1graf->addPoint(t,N*yk[1]);
            y2graf->addPoint(t,N*yk[2]);
            y3graf->addPoint(t,N*(alpha_i*sigma_e_gen*yk[2]-beta_j*sigma_a_gen*yk[1])/sigma_e_gen);
            //qDebug()<<N*yk[1]<<N*yk[2]<<N*((sigma_e_gen*yk[2]-sigma_a_gen*yk[1]))/sigma_e_gen<<N<<yk[2]<<yk[1]<<sigma_e_gen*yk[2]<<sigma_a_gen*yk[1];

            if  ((time.elapsed()-tTemp)>100)
            { //qDebug()<<2<<t;
                QApplication::processEvents();
                emit grafUpdate();
                tTemp=time.elapsed();
            }
        }

        //метод рк4
        f_kk(t,ht);
        t+=ht;
        //PPump = Pump_Power*(t<TPump ? 1:0);
        PGen = P_gen(yk[0]);
        for(int i=0;i<NY;i++)
        {
            yk[i]+=ht/6.*(kk[1][i]+2*kk[2][i]+2*kk[3][i]+kk[4][i]);
        }
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
        } //проверка на NaN
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
