#include "widget.h"
#include <qdebug.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    Y y;
    //строю ВАХ
    double P0=100;
    double dP=100;
    double P1=800;
    for (double P=P0;P<P1;P+=dP)
    {
        qDebug()<<"P"<<P;
        y.P=P;
        y.tm_YLF_3pr();
        euler(y);
    }
 //close();
}

void Widget::euler(Y y)
{
    //t=0:0.000001:0.16;
    const double dt = 0.000000001;
    const double t1 = 0.16;
    const int i1 =(int)(t1/dt);
    int nDots = 100;
    double di = i1/((double)nDots);
    //double dy[5][i1];
    y.y0=0; //q
    y.y1=1;
    y.y2=0;
    y.y3=0;
    //y4<<0;
    double t;
    t=0;
    qDebug()<<"t="<<dt*i1;
    for (int i=0;i<i1;i++)
    {
        y.f();
        y.y1=y.y1+y.dy1*dt;
        y.y2=y.y2+y.dy2*dt;
        y.y3=y.y3+y.dy3*dt;
        y.y0=y.y0+y.dy0*dt;
        //y4<<1-y1[i]-y2[i]-y3[i];
        t=t+dt;
        //if ((i/di -(int)(i/di))==0)qDebug()<<"P_gen"<<y.P_gen()<<"q"<<y.y0<<"n1"<<y.y1<<"n2"<<y.y2<<"n3"<<y.y3<<"t"<<
        //                                     t<<"dq"<<y.dy0<<"dn1"<<y.dy1<<"dn2"<<y.dy2<<"dn3"<<y.dy3;

    }
    qDebug()<<"P"<<y.P<<"P_gen"<<y.P_gen()<<"q"<<y.y0<<"n1"<<y.y1<<"n2"<<y.y2<<"n3"<<y.y3<<"t"<<
                                                 t<<"dq"<<y.dy0<<"dn1"<<y.dy1<<"dn2"<<y.dy2<<"dn3"<<y.dy3;
}

Widget::~Widget()
{

}


