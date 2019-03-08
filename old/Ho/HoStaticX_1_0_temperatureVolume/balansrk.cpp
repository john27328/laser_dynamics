#include "balansrk.h"


BalansRK::BalansRK()
{
    //qDebug()<<"BalansRK";
    static double sigma_pump_st[] = {0.244472044657664, 0.298236640277648, 0.175656800044329, 0.176301556180327, 0.244472044657664, 0.47768148557935, 0.99655259075857, 1.25464212257079, 1.09532211790852, 0.871403274982738, 0.575071048007515, 0.414884429897341, 0.430708270176857, 0.453322001983115, 0.604184437097327, 0.391368470905093, 0.174670892424313, 0.209420179265201, 0.316182875184098, 0.531763497215144, 0.535694278714599, 0.41255622586345, 0.24383406256076, 0.187162762081373, 0.201972327257757, 0.361106567271826, 0.388959339628766, 0.362018030124067, 0.55331989333834, 0.511095908136425, 0.317069969078088, 0.369408269774161, 0.650977031408109, 0.537056269978361, 0.233497855953659, 0.168922840988813, 0.23528151685765, 0.300555655904834, 0.207720094002767, 0.28001503061498, 0.382813983121185, 0.273464219009976, 0.160536764877069, 0.168922840988813, 0.127143499749048, 0.110107391965375, 0.19372880693761, 0.208331261941544, 0.119688725324442, 0.043893640871534, 0.036326512666831};
    sigma_pump_dat = sigma_pump_st;
    constants();
    variables();
    nDots = 100000;
    t_end = 2.1;
    VAHgraf = new grafSource(0);
    VAHgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    PPumpGraf = new grafSource(0);
    PPumpGraf->pen=QPen(Qt::blue,3,Qt::SolidLine);
    n1graf = new grafSource(0);
    n1graf->pen=QPen(Qt::red,2,Qt::SolidLine);
    n2graf = new grafSource(0);
    n2graf->pen=QPen(Qt::green,2,Qt::SolidLine);

    n1zgraf = new grafSource(0);
    n1zgraf->pen=QPen(Qt::red,2,Qt::SolidLine);
    n2zgraf = new grafSource(0);
    n2zgraf->pen=QPen(Qt::green,2,Qt::SolidLine);
    PPumpZgraf = new grafSource(0);
    PPumpZgraf->pen=QPen(Qt::blue,2,Qt::SolidLine);

    Kgraf = new grafSource(0);
    Kgraf->pen=QPen(Qt::blue,2,Qt::SolidLine);
    q2graf = new grafSource(0);
    q2graf->pen=QPen(Qt::darkMagenta,2,Qt::SolidLine);
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

create_sigma_g();



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
        kk1[i]=dyk(i,t,            yk, 0,      kk0);
    for (int i=0; i<NY; i++)
        kk2[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk1);
    for (int i=0; i<NY; i++)
        kk3[i]=dyk(i,t + 0.5 * ht, yk, 0.5*ht, kk2);
    for (int i=0; i<NY; i++)
        kk4[i]=dyk(i,t + ht,       yk, ht,     kk3);
}

double BalansRK::dq(double q, const int N_kr, double *R0n, double q_pump, double *n2)
{

    double D;

    //"1/2*((q^2*(Ba+Bg)^2*qpump^2+2*((Ba+Bg)*R0n+(A21+2*Kap)*Ba+Bg*A21)*q*qpump+R0n^2+(2*A21+4*Kap)*R0n+A21^2)^(1/2)+(-Ba-Bg)*q*qpump-A21-R0n)/Kap"
    for (int i=0; i<N_kr; i++)
    {

        D = pow(q,2) * pow((B_a+B_g[i]),2) * pow(q_pump,2) +
                2 * ((B_a + B_g[i]) * R0n[i] + (A21 + 2 * K_ap_n)*B_a + B_g[i] * A21) * q * q_pump
                + pow(R0n[i],2) + (2 * A21 + 4 * K_ap_n) * R0n[i] + pow(A21,2);
        n2[i]= 0.5 * (sqrt(D) + (-B_a - B_g[i]) * q * q_pump - A21 - R0n[i]) / K_ap_n;

    }



    double deltaNi[N_kr];
    for (int i=0; i<N_kr; i++)
        deltaNi[i] = B_g[i] * n2[i] - B_a * (1 - n2[i]);

    double deltaNSum = 0;
    for (int i=0; i<N_kr; i++)
        deltaNSum += deltaNi[i];
    //qDebug()<<"n2"<<n2[0]<<"q"<<q<<"N0 * V_a *deltaNSum"<<N0 * V_a *deltaNSum<<"rho"<<rho;

    double dq = (deltaNSum - rho / N0 / V_a)/B_g[0] ;
    return fabs(dq);
}

double BalansRK::getEGen() const
{
    return EGen;
}

double BalansRK::sigma_pump(double lambda_pump_m)
{
    double sigma = 0;
    double lambda_pump = lambda_pump_m * 1e9;
    if (lambda_pump > 1900 && lambda_pump < 1950)
    {
        double delta_i = (lambda_pump - 1900);
        int i1 = delta_i;
        double sigma_i1=sigma_pump_dat[i1];
        double sigma_i2=sigma_pump_dat[i1+1];
        sigma = sigma_i1 + (sigma_i2 - sigma_i1) * (delta_i - i1);
    }
    else
        sigma = 0;
    return sigma * 1e-24;
}

double BalansRK::sigma_g_T(double T)
{
    double E1k[] =
    {
        0,
        4,
        41,
        51,
        141,
        144,
        150,
        162,
        398,
        418,
        448,
        457,
        498,
        506,
        520,
        531,
        535
    };

    double E2k[] =
    {
        5229,
        5232,
        5243,
        5250,
        5303,
        5312,
        5320,
        5341,
        5352,
        5375,
        5395,
        5404,
        5418,
        5455,
        5490
    };

    const int N_E1 = sizeof(E1k)/sizeof(double);
    const int N_E2 = sizeof(E2k)/sizeof(double);

    double E1[N_E1], E2[N_E2];

    for (int i=0; i<N_E1; i++)
        E1[i]=E1k[i]*h*c*100;
    for (int i=0; i<N_E2; i++)
        E2[i]=E2k[i]*h*c*100;

    double Z1 = 0;
    for (int i = 0; i < N_E1; i++)
        Z1 += exp(- (E1[i] - E1[0]) / k /T);

    double Z2 = 0;
    for (int i = 0; i < N_E2; i++)
        Z2 += exp(- (E2[i] - E2[0]) / k /T);

    double z1_z2 = Z1/Z2;
    db(vr(z1_z2));
    double deltaE12 = E2[0] - E1[0];
    double sigma_g = sigma_a * z1_z2 * exp(+(deltaE12 - h * c  / lambda_gen) / K / T);
//    db(vr(sigma_g)
//       vr(sigma_g/sigma_a)
//       vr(exp(+(deltaE12 - h * c  / lambda_gen) / K / T))
//       vr(lambda_gen));
    return sigma_g;
}

void BalansRK::create_sigma_g()
{
    if (sigma_g) {
        db(vr(sigma_g));
        delete[] sigma_g;
    }
    const int N_kr = Nrasb;
    sigma_g = new double[N_kr];

    if (T_load_key)
    {
        for (int i = 0; i<N_kr; i++)
        {
            sigma_g[i] = sigma_g_T(T_cr_dl[i]);
            db(vr(sigma_g[i]) vr(sigma_a));
        }
    }
    else
    {
        for (int i = 0; i<N_kr; i++)
        {
            sigma_g[i] = sigma_g_T(TRoom);
            db(vr(sigma_g[i]) vr(sigma_a));
        }
    }

    //ADD_VAR(B_g, sigma_g * c / V, "c * m-1")
    if (B_g) delete (B_g);
    //const int N_kr = Nrasb;
    B_g = new double[N_kr];
    for (int i = 0; i<N_kr; i++)
    {
        B_g[i] = sigma_g[i] * c / V;
    }
}

double BalansRK::getEPump() const
{
    return EPump;
}

void BalansRK::setT_end(double value)
{
    t_end = value;
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
    n1graf->clear();
    n2graf->clear();

    n1zgraf->clear();
    n2zgraf->clear();
    PPumpZgraf->clear();

    Kgraf->clear();
    VAHgraf->clear();
    q2graf->clear();
    qDebug()<<"start_fyk(double ht)";
    variables();

    double P2 = P;
    double dP = P2 / nDots;
    const int N_kr = Nrasb;
    double n2[N_kr];
    double P_pump[N_kr];

    bool error = 0;
    for (P=0; P<=P2; P+=dP)
    {
        double Pg = P_gen(P, error, n2,  P_pump);
        //qDebug()<<"P"<<P<<Pg;
        VAHgraf->addPoint(P,Pg);
    }
     if (error) QMessageBox::warning(0,"Warning", "n2 - n1 > epsilonN!");

    // строю график n(z) и P(z)

    double z = 0;

    //PPumpZgraf->addPoint(z,P);
    for (int i = 0; i<N_kr; i++)
    {
        P=P_pump[i]/P_pump[0];
        n1zgraf->addPoint(z,1-n2[i]);
        n2zgraf->addPoint(z,n2[i]);
        PPumpZgraf->addPoint(z,P);
        z+=l*1000;

        n1zgraf->addPoint(z,1-n2[i]);
        n2zgraf->addPoint(z,n2[i]);
        PPumpZgraf->addPoint(z,P);
    }

    P=P2;
    emit grafUpdate();
}

void BalansRK::stop_fyk()
{
    startK = 0;
    //qDebug()<<"stop_fyk";
}


#include "fyk.cpp"
