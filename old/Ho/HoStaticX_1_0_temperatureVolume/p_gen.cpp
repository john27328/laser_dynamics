#include "balansrk.h"

double BalansRK::P_gen(double P0, bool &error, double * n2, double *P_pump)
{
    const int N_kr = Nrasb;
    if (n2 == 0) {
        n2 = new double [N_kr];
    }
    if (P_pump == 0) {
        P_pump = new double [N_kr];
    }
    for (int i=0;i<N_kr;i++)
        n2[i] = 0;

    double q_pump;

//    double n20[N_kr];
//    for (int i=0;i<N_kr; i++)
//        n20[i]=n2[i];
    double n2p[N_kr];

    double q ;
    bool k=1;

    double epsilonN = 1e-8;
    int t=0;
    while(k)
    {
        t++;
        for (int i=0;i<N_kr; i++)
            n2p[i]=n2[i];
        q = q_gen_k(n2,P0, N_kr,q_pump, P_pump);
        k=0;
//        for(int i=0;i<N_kr; i++)
//            qDebug()<<"delta n2"<<i<<n2[i]-n2p[i]<<"t"<<t;
        for(int i=0;i<N_kr; i++)
            if(fabs(n2[i]-n2p[i])>epsilonN) k=1;
        if (t>100) {
            k=0;
            error = 1;
         }
    }



//    for(int i=0;i<N_kr; i++)
//        qDebug()<<"delta n2"<<i<<n2[i]-n2p[i]<<"t"<<t;
    double PGen = q_pump * q * ((-log(1 - T2) * c) / (2 * L)) * h * c / lambda_gen;

    double n2sred = 0;

    for (int i = 0; i<N_kr;i++)
        n2sred+=n2[i];
    n2sred = n2sred / N_kr;

    n1graf->addPoint(P0, 1-n2sred);
    n2graf->addPoint(P0,n2sred);
    //Kgraf->addPoint(P0,q*T2/2);
    Kgraf->addPoint(P0,q*(-log(1 - T2))/2);
    //q2graf->addPoint(P0,q*(-log(1 - T2))/2);


    return PGen;
}

double BalansRK::q_gen_k(double *n2, double P0, const int N_kr, double &q_pump, double *P_pump)
{
    double P = P0;
    double P_temp;

    if (P_pump == 0) {
        P_pump = new double [N_kr];
    }
    double R0[N_kr];
    double R0n[N_kr];

    double K_sigma = sigma_pump_pr * sigma_pump(lambda_pump) * N0;
    //qDebug()<<K_sigma;

    for (int i=0; i<N_kr; i++)
    {
        P_temp = P * exp(-K_sigma * (1-n2[i]) * l);
        P_pump[i] = P - P_temp;
        P = P_temp;
    }

    //qDebug()<<P0<<"T"<<P/P0;

    P=P * T_doj;

    for (int i = N_kr - 1; i>=0; i--)
    {
        P_temp = P * exp(-K_sigma * (1-n2[i]) * l);
        P_pump[i] += P - P_temp;
        P = P_temp;
    }

    //for (int i=0; i<N_kr; i++) qDebug()<<i<<P_pump[i];

    for (int i=0; i<N_kr; i++)
        R0[i] = P_pump[i] / (S * l * h * c / lambda_pump);

    double P_pumpFull = 0;
    for (int i=0; i<N_kr; i++)
    {
        P_pumpFull+=P_pump[i];
    }

    //qDebug()<<P_pumpFull;

    for(int i=0;i<N_kr;i++)
    {
        R0n[i] = R0[i]/N0;
    }

    //qDebug()<<"Pump_full"<<P_pumpFull;

    q_pump = P_pumpFull * L * lambda_pump / h / pow(c,2);

    //решение уравнения, подбираю q, чтобы deltaN == rho

    double ql = 0.;
    double qr = 1/((-log(1 - T2))/2);
    double qh = 1e-8;
    //метод золотого сечения
    double phi = (1.+sqrt(5.)) / 2.;
    double resphi = 2-phi;
    double q1 = ql + resphi * (qr - ql);
    double q2 = qr - resphi * (qr - ql);
    double f1 = dq(q1,N_kr, R0n, q_pump,n2);
    double f2 = dq(q2,N_kr, R0n, q_pump,n2);
    double tgold=0;
    while (fabs(qr-ql) > qh)
    {
        tgold++;
        if (f1 < f2)
        {
            qr = q2;
            q2 = q1;
            f2 = f1;
            q1 = ql + resphi * (qr - ql);
            f1 = dq(q1,N_kr, R0n, q_pump,n2);
        }
        else
        {
            ql = q1;
            q1 = q2;
            f1 = f2;
            q2 = qr - resphi * (qr - ql);
            f2 = dq(q2,N_kr, R0n, q_pump,n2);
        }
    }
    double q = (q1 + q2) / 2;

    if (dq(q,N_kr, R0n, q_pump,n2) > 1e-8) dq(q=0, N_kr, R0n, q_pump, n2);


    return q;
}
