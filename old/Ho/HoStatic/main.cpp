#include <QCoreApplication>
#include <math.h>
#include <QDebug>


#define N_kr 100
#define pi 3.14


double N0 = 1.38e26;
double K_ap = 1.28e-24;
double A21 = 133;
double sigma_g = 1.32e-24;
double sigma_a = 0.25e-24;
double K_sigma = 0.96e2;
double d = 0.55e-3;
double rho_bad = 0.035;
double L = 12e-2;
double T1 = 0.3e-2;
double T2 = 35e-2;
double T_doj = 100e-2;
double l_kr = 40e-3;
double l=l_kr/N_kr;
double P = 4.4;
double S = pi * pow(d,2) / 4;
double lambda_pump = 1908e-9;
double lambda_gen = 2090e-9;
double c = 3e8;
double h = 6.62e-34;
double P_pump[N_kr];
double P_pumpFull;
double R0[N_kr];
double V = S * L;
double V_a = S * l;
double B_g = sigma_g * c / V;
double B_a = sigma_a * c / V;
double rho = rho_bad - c / L * (log(1 - T1) + log(1 - T2)) / 2;

//нормировка

double R0n[N_kr];
double K_ap_n = K_ap * N0;
double q_pump;

double dq(double q)
{
    double n2[N_kr];
    double D;

    //"1/2*((q^2*(Ba+Bg)^2*qpump^2+2*((Ba+Bg)*R0n+(A21+2*Kap)*Ba+Bg*A21)*q*qpump+R0n^2+(2*A21+4*Kap)*R0n+A21^2)^(1/2)+(-Ba-Bg)*q*qpump-A21-R0n)/Kap"
    for (int i=0; i<N_kr; i++)
    {

        D = pow(q,2) * pow((B_a+B_g),2) * pow(q_pump,2) +
                2 * ((B_a + B_g) * R0n[i] + (A21 + 2 * K_ap_n)*B_a + B_g * A21) * q * q_pump
                + pow(R0n[i],2) + (2 * A21 + 4 * K_ap_n) * R0n[i] + pow(A21,2);
        n2[i]= 0.5 * (sqrt(D) + (-B_a - B_g) * q * q_pump - A21 - R0n[i]) / K_ap_n;

    }



    double deltaNi[N_kr];
    for (int i=0; i<N_kr; i++)
        deltaNi[i] = B_g * n2[i] - B_a * (1 - n2[i]);

    double deltaNSum = 0;
    for (int i=0; i<N_kr; i++)
        deltaNSum += deltaNi[i];
    //qDebug()<<"n2"<<n2[0]<<"q"<<q<<"N0 * V_a *deltaNSum"<<N0 * V_a *deltaNSum<<"rho"<<rho;

    double dq = N0 * V_a * deltaNSum - rho;
    return fabs(dq);

}

int main()
{
    double P_temp;

    for (int i=0; i<N_kr; i++)
    {
        P_temp = P * exp(-K_sigma * l);
        P_pump[i] = P - P_temp;
        P = P_temp;
    }

    P=P * T_doj;

    for (int i = N_kr - 1; i>=0; i--)
    {
        P_temp = P * exp(-K_sigma * l);
        P_pump[i] += P - P_temp;
        P = P_temp;
    }

    for (int i=0; i<N_kr; i++)
        R0[i] = P_pump[i] / (S * l * h * c / lambda_pump);

    P_pumpFull = 0;
    for (int i=0; i<N_kr; i++)
    {
        P_pumpFull+=P_pump[i];
    }

    for(int i=0;i<N_kr;i++)
    {
        R0n[i] = R0[i]/N0;
    }

    //qDebug()<<"Pump_full"<<P_pumpFull;

    q_pump = P_pumpFull * L * lambda_pump / h / pow(c,2);

    //решение уравнения, подбираю q, чтобы deltaN == rho

    double q1 = 0.;
    double q2 = 10;
    double qh = 0.0001;
    double dq_min = dq(q1);
    double q_temp, dq_temp;

    for (double q = q1; q<=q2; q+=qh)
    {

        dq_temp=dq(q);
        if(dq_temp <= dq_min)
        {
            dq_min = dq_temp;
            q_temp = q;
        }
        qDebug()<<"dq_min"<<dq_min<<"dq"<<dq_temp<<"q"<<q;
    }

    double q = q_temp;


    qDebug()<<q<<dq(q)<<R0n[0]<<R0[0]<<P_pumpFull;

     if (q<q1 + qh || q>q2-qh) {q = 0; qDebug()<<"!!!";}

    //qDebug()<<dq(2.61822);

    double PGen = q_pump * q * ((-log(1 - T2) * c) / (2 * L)) * h * c / lambda_gen;
    qDebug()<<PGen;
    //double n = R0n * n1 - A21 * n2 - K_ap_n * pow(n2,2) - q * q_pump * (B_g * n2 - B_a * n1);

    return 0;
}


