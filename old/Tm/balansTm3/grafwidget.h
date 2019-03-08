#ifndef GRAFWIDGET_H
#define GRAFWIDGET_H

#include <QWidget>

#include <QPainter>
#include <QPointer>
#include "grafsource.h"
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QDebug>
#include <cmath>
#include <QPolygon>
#include <QPolygonF>
#include <QString>
#include <QLabel>
#include <QPainterPath>
#include <QtGui>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>

class grafWidget : public QWidget
{
    Q_OBJECT
public:
    explicit grafWidget(QWidget *parent = 0);
    ~grafWidget();
    virtual void paintEvent(QPaintEvent*);
    void addListPoint(grafSource *sourcePTR);

    QString nameX;
    QString nameY;
    QString nameGraf;

    //int getBusy() const;
    int busy;
    void legendVisible(bool visible);

private:

    double X_min, X_max, Y_min, Y_max, X_min1, X_max1, Y_min1, Y_max1;
    double x_disp;
    double y_disp;
    double w, h;
    double x_0;
    double y_0;
    double x_1;
    double x_1_def;
    double y_1;
    double x_2;
    double y_2;
    double y_3;
    double y_4;
    double y_5;
    double y_6;
    double y_7;
    double y_8;
    double DX, DY;
    int zoom_kl;
    int mouseZoomEvent_kl;
    int mouseDrag_kl;
    int mouseMoveEvent_kl;
    int disp_kl;
    int legend_kl;
    double x_legend;
    double x(double X);
    double y(double Y);
    double X(double x);
    double Y(double y);
    int N_Graf;
    QPointF XY_to_xy(QPointF point_XY);
    int nPoint;
    void extremes();
    void step();
    void plotingArea();
    void plotGraf();
    void plotNames();
    void plotSelection();
    void save();
    void disp();
    void plotLegend();
    QMouseEvent * ME_pe;

    //void recalculation_of_coordinates_x();
    //void recalculation_of_coordinates_x();
    //void range_XY();
    //void axis();
    //QPainter *painter;
    QPainter *painter;

    //grafSource source;

    QList<grafSource*> grafSourceList;
    QLabel *mouseLabel;
signals:

public slots:

protected:
    virtual void mousePressEvent  (QMouseEvent* pe);
    virtual void mouseReleaseEvent(QMouseEvent* pe);
    virtual void mouseMoveEvent   (QMouseEvent* pe);



};

#endif // GRAFWIDGET_H
