#ifndef GRAFSource_H
#define GRAFSource_H
#include <QPointF>
#include <QList>
#include <QColor>
#include <QPen>
#include <QString>

class grafSource
{
public:
    grafSource(QString name, int nList = 0);
    grafSource(int nList);
    grafSource();
    //QColor color;
    QPen pen;

    int n;//при n=0 бесконечность
    int iList; // число добавленных точек
    //int lineWidth;
    QList <QPointF> xyList;
    void addPoint(QPointF point);
    void addPoint(double x,double y);
    void addPointLists(QList<double> xList, QList<double> yList);
    void clear();
    QString nameSource;
};

#endif // GRAFSource_H
