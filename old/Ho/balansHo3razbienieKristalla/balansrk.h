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

#define NY 3  //число уравнений
#define NKr 10 // число разбиений кристалла

class BalansRK : public QObject
{
    Q_OBJECT

QTimer *timer;
public:
    BalansRK();
    void set_y0(double Y0, double Y1, double Y2);
    void start_fyk(double ht);
    void stop_fyk();

    grafSource* y1graf;
    grafSource* y2graf;
    grafSource* y3graf;
    grafSource* y4graf;
    grafSource* Pgraf;
    grafSource* PPumpGraf;
    QMap<QString,Constants *> cMap;
    double P_gen();
    double getT();
    void constants();
    void variables();
    void setNDots(int value);

    void setT_end(double value);

    double getEPump() const;

    double getEGen() const;

private:


    double dyk(int ny, int nKr, double t, double yfk[NY][NKr], double dt, double kf[NY][NKr]);
//    double dykSet(int ny, double t, double y0, double y1, double y2, double y3);
    double tk;
    int k;
    double yk[NY][NKr];
    double y0[NY][NKr];
//    double cc(QString name);
//    double setMasC(QString name, int i);
    double (BalansRK::*cPtr)(QString name, int i);

    double masC[100][NY];
    int jC;
    int kC;
    double setC(QString name, double value);
    double kk0[NY][NKr];
    double kk1[NY][NKr];
    double kk2[NY][NKr];
    double kk3[NY][NKr];
    double kk4[NY][NKr];

    void f_kk(double t, double ht);
    double t_end;
    double k_max;
    bool startK=0;

    double EPump;
    double EGen;

    double P_gen(double q);
    int nDots;
    double t;
#define ADD_CONST(name, value, dimConst, dimDisp, ratioDisplayToConst,visible) \
    double name = (double)value/(double)ratioDisplayToConst;
//expression
#define ADD_VAR(name,expression, dimConst)  \
    double name;
#include"const.cpp"

signals:
    void grafUpdate();

};

#endif // BALANSRK_H
