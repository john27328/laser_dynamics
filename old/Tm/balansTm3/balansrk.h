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


    double dyk(int ny, double t, double y0, double y1, double y2, double y3);
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
    double kk[4+1][NY];
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
    double name = 0;
#include"const.cpp"

signals:
    void grafUpdate();

};

#endif // BALANSRK_H
