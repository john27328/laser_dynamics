#include "grafsource.h"
#include <QDebug>
grafSource::grafSource(QString name, int nList)
{
    n=nList;
    iList=0;
    nameSource = name;
}

grafSource::grafSource(int nList)
{
    n=nList;
    iList=0;
}

grafSource::grafSource()
{
    n=0;
    iList=0;
}

void grafSource::addPoint(QPointF point)
{
    iList++;
    //if(1/point.x()==0)point.setX(1e300);
    if(n>0 && iList>n)
        xyList.removeFirst();
    xyList<<point;
}

void grafSource::addPoint(double x,double y)
{
    addPoint(QPointF(x,y));
}

void grafSource::addPointLists(QList<double> xList, QList<double> yList)
{
    int n = xList.size();
    int m = yList.size();
    if (m>n) n=m;
    for (int i=0;i<n;i++)
    {
        addPoint(xList[i],yList[i]);
    }
}

void grafSource::clear()
{
    iList = 0;
    xyList.clear();
}
