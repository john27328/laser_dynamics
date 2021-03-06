#ifndef Y_H
#define Y_H

#include <math.h>
#include <QDebug>
//#include <QVector>
#include <grafsource.h>
#include <grafwidget.h>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>

#define Ny 4//число уравнений

class Y : public QObject
{
    Q_OBJECT
public:
    Y();

    QTimer *timer;

    double getT_end() const;
    void setT_end(double value);

//    double getH() const;
//    void setH(double va   void set_y0(double Y0, double Y1, double Y2, double Y3);lue);

    double getP() const;
    void setP(double value);

    double getN_pr() const;
    void setN_pr(double value);
    void setParametri(double P_Nak, double T_Out, double N_Pr, double t_End);
    void fyk(double ht);
    void Tm_parameters();
    void tm_YLF_3pr_default();
    void tm_YAP_3pr_default();
    void tm_YLF_3pr();
    void tm_YAP_3pr();
    void set_y0(double Y0, double Y1, double Y2, double Y3);
    void stop_fyk();

   // grafSource* y0graf;
    grafSource* y1graf;
    grafSource* y2graf;
    grafSource* y3graf;
    grafSource* Pgraf;

    double getT_out() const;
    void setT_out(double value);
    double P_gen();

    double getT() const;
    void setT(double value);

    int getNDots() const;
    void setNDots(int value);

    double getL() const;
    void setL(double value);

    double getD() const;
    void setD(double value);

    double getBeta() const;

    double getW31() const;

    double getW32() const;

    double getW21() const;

    double getN() const;

    double getLres() const;
    void setLres(double value);

    double getSigma_e_gen() const;

    double getSigma_a_gen() const;

    double getLambda_p() const;

    double getLambda_g() const;

    double getAlpha_i() const;

    double getBeta_j() const;

    void setW31(double value);

    void setW32(double value);

    void setW21(double value);

    void setSigma_e_gen(double value);

    void setSigma_a_gen(double value);

    double qk();
    double y1k();
    double y2k();
    double y3k();

private:
    double dyk(int ny, double t, double yk0, double yk1, double yk2, double yk3);
    double yk[Ny];
    double y0[Ny];
    double eps;//относительная точность
    double t_end;
    //double H; //шаг
    double k_max;
    bool startK=0;

    double k1[Ny],k2[Ny],k3[Ny],k4[Ny];

    void kk(double t, double ht);

    double beta0;
    double beta;
    double W31;
    double W32;
    double W21;
    double h;
    double c;
    double sigma_a_pump;
    double N0;
    double N;
    double N_pr;
    double n;
    double lambda_p;
    double lambda_g;
    double sigma_e_gen;
    double sigma_a_gen;
    double P;
    double d;
    double alpha_i;
    double beta_j;
    double Lres;
    double l;
    double Le;
    double Va;
    double V;
    double gamma_1;
    double gamma_2;
    double gamma_i;
    double rho;
    double B_e;
    double B_a;
    //double K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*3.14*d*d);
    double K;
    double T_out;
    int nDots;
    double t;


    double P_gen(double q);
signals:
    void grafUpdate();
    void endSaveSignal();

public slots:
    void saveSettings();


};

#endif // Y_H
