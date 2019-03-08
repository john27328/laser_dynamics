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

#define NY 4  //число уравнений

class BalansRK : public QObject
{
    Q_OBJECT

QTimer *timer;
public:
    BalansRK();
    void set_y0(double Y0, double Y1, double Y2, double Y3);
    void start_fyk(double ht);
    void stop_fyk();

    grafSource* y1graf;
    grafSource* y2graf;
    grafSource* y3graf;
    grafSource* Pgraf;
    QMap<QString,Constants *> cMap;

private:

    void constants();
    void variables();
    double dyk(int ny, double t, double *yfk, double dt, double *kf);
    double dykSet(int ny, double t, double y0, double y1, double y2, double y3);
    double tk;
    int k;
    double yk[NY];
    double y0[NY];
    double cc(QString name);
    double setMasC(QString name, int i);
    double (BalansRK::*cPtr)(QString name, int i);

    double masC[100][NY];
    int jC;
    int kC;
    double setC(QString name, double value);
    double kk1[NY];
    double kk2[NY];
    double kk3[NY];
    double kk4[NY];
    void f_kk(double t, double ht);
    double t_end;
    double k_max;
    bool startK=0;
    double P_gen();
    double P_gen(double q);
    int nDots;
    double t;
#define ADD_CONST(name, value, dimConst, dimDisp, ratioDisplayToConst,visible) \
    double name = value/ratioDisplayToConst;
//expression
#define ADD_VAR(name,expression, dimConst)  \
    double name = 0;
#include"const.cpp"

signals:
    void grafUpdate();

};

#endif // BALANSRK_H
