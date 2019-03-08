#ifndef BALANSRK_H
#define BALANSRK_H

#include <QMap>
#include "constants.h"
#include <QDebug>
#include <QTimer>
#include <QTime>
#include "grafsource.h"
#include <math.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QVector>
#include <QFile>

#define NY 3  //число уравнений

class BalansRK : public QObject
{
    Q_OBJECT

QTimer *timer;
public:
    BalansRK();
    void set_y0(double Y0, double Y1, double Y2);
    void start_fyk(double ht);
    void stop_fyk();

    grafSource* n1graf;
    grafSource* n2graf;

    grafSource* n1zgraf;
    grafSource* n2zgraf;
    grafSource* PPumpZgraf;

    grafSource* Kgraf;
    grafSource* q2graf;
    grafSource* VAHgraf;
    grafSource* PPumpGraf;
    QMap<QString,Constants *> cMap;

    double getT();
    void constants();
    void variables();
    void setNDots(int value);

    void setT_end(double value);

    double getEPump() const;

    double getEGen() const;
    void sigma_pump_set_dat();
    double sigma_pump(double lambda_pump_m);

private:
    QVector<double> sigma_pump_dat;
    QVector<double> sigma_pump_lambda;
    double P_gen(double P0, bool &error,double *n2 = 0,  double *P_pump=0);
    double q_gen_k(double *n2, double P0, const int N_kr, double &q_pump, double *P_pump=0);

    double dyk(int ny, double t, double *yfk, double dt, double *kf);
//    double dykSet(int ny, double t, double y0, double y1, double y2, double y3);
    double tk;
    int k;
    double yk[NY];
    double y0[NY];
//    double cc(QString name);
//    double setMasC(QString name, int i);
    double (BalansRK::*cPtr)(QString name, int i);

    double masC[100][NY];
    int jC;
    int kC;
    double setC(QString name, double value);
    double kk0[NY];
    double kk1[NY];
    double kk2[NY];
    double kk3[NY];
    double kk4[NY];

    void f_kk(double t, double ht);
    double t_end;
    double k_max;
    bool startK=0;

    double EPump;
    double EGen;

    //double P_gen(double q);
    int nDots;
    double t;
    double dq(double q, const int N_kr, double *R0n, double q_pump, double *n2);
#define ADD_CONST(name, value, dimConst, dimDisp, ratioDisplayToConst,visible) \
    double name = (double)value/(double)ratioDisplayToConst;
//expression
#define ADD_VAR(name,expression, dimConst)  \
    double name = 0;
#include"const.cpp"

signals:
    void grafUpdate();

};

#endif // BALANSRK_H
