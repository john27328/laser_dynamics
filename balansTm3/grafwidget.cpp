#include "grafwidget.h"

grafWidget::grafWidget(QWidget *parent) :
    QWidget(parent)
{
    //painter=0;
    X_min=13.5; Y_min=27; X_max=20; Y_max=100;
    x_0=140, y_0=60; //нижний левый угол графика относительно нижнего левого края поля
    x_1=x_1_def=20,  y_1=50;//верхний правый угол графика
    x_2=5, y_2=5;//выступание сетки за края графика
    y_3=5; //отступ названия графика от верхней границы окна
    y_4=5; //отсуп названия графика от верхней границы графика
    y_5=5; //отступ название оси у от левого края окна
    y_6=100; //отступ название оси у от левого края графика
    y_7=5;//отступ название оси x от нижнего края окна
    y_8=20;//отступ название оси x от нижнего края графика
    x_legend = 200; //толшина легенды

    zoom_kl = 0;//0; // масштабирование, ключ
    mouseZoomEvent_kl = 0;
    mouseDrag_kl=0;
    mouseMoveEvent_kl=0;
    disp_kl=0;
    legend_kl=0;

    DX=X_max-X_min; DY=Y_max - Y_min;
    N_Graf=0;
    setMinimumSize(300,300);
    nameX = "ось x";
    nameY = "ось y";
    nameGraf = "график";

painter = new QPainter;
busy=0;
    //qDebug()<<x(X(0))<<y(Y(0));
}

grafWidget::~grafWidget()
{
    delete painter;
}

void grafWidget::paintEvent(QPaintEvent *)
{
    busy=1;
    if(legend_kl) x_1 = x_1_def + x_legend;
    else x_1 = x_1_def;
    //painter = new QPainter(this);
    painter->begin(this);
    w=width(), h=height();
    QBrush brush(Qt::white);
    painter->fillRect(0,0,w,h,brush);
    //qDebug()<<"0";
    if (N_Graf && !zoom_kl) extremes();
    step();
    if (N_Graf) plotGraf();
    plotNames();
    plotingArea();
    if (disp_kl) disp();
    if (mouseMoveEvent_kl!=0) plotSelection();
    if (legend_kl) plotLegend();
    //qDebug()<<"mouseMoveEvent_kl ="<<mouseMoveEvent_kl;
    //qDebug()<<"4";
    //delete painter;
    //qDebug()<<"5";
    mouseMoveEvent_kl=0;
    painter->end();
    busy=0;
}

double grafWidget::x(double X)
{
    return (X-X_min)/(X_max-X_min)*(w-x_0-x_1)+x_0;
}

double grafWidget::y(double Y)
{
    return h-(Y-Y_min)/(Y_max-Y_min)*(h-y_0-y_1)-y_0;
}

double grafWidget::X(double x)
{
    return (x-x_0)*(X_max - X_min)/(w-x_0-x_1) + X_min;
}

double grafWidget::Y(double y)
{
    return -(y-h+y_0)*(Y_max - Y_min)/(h-y_0-y_1) + Y_min;
}

QPointF grafWidget::XY_to_xy(QPointF point_XY)
{
    QPointF point;
    point.setX(x(point_XY.x()));
    point.setY(y(point_XY.y()));
    return point;
}

void grafWidget::plotingArea() //рисую сетку
{
    //ниже магия, с помощью которой подбираю красивый шаг для сетки
    if (DX==0) DX=1;
    if (DY==0) DY=1;
    double Dx_d=DX, Dy_d=DY;
    while (fabs(Dx_d)<10) {
        Dx_d=Dx_d*10;
    }
    while (fabs(Dy_d)<10){
        Dy_d=Dy_d*10;
    }
    while (fabs(Dx_d)>100) {
        Dx_d=Dx_d/10;
    }
    while (fabs(Dy_d)>100){
        Dy_d=Dy_d/10;
    }

    double dx_d,dy_d;
    if(Dx_d<20) dx_d=2;
    else
        if (Dx_d<50) dx_d=5;
        else dx_d=10;

    if(Dy_d<20) dy_d=2;
    else
        if (Dy_d<50) dy_d=5;
        else dy_d=10;
    double dX,dY;
    //вот такой шаг будет
    dX=DX/Dx_d*dx_d;
    dY=DY/Dy_d*dy_d;

    //определяю начальное значение сетки
    double nX,X0;
    nX=(int)(X_min/dX);
    if(X_min<0) nX--;
    X0=(nX+1)*dX;
    double X_sd=X0-X_min;
    if (X_sd==dX) X_sd=0;
    //qDebug()<<"X_sd"<<X_sd;

    double nY,Y0;
    nY=(int)(Y_min/dY);
    if(Y_min<0) nY--;
    Y0=(nY+1)*dY;
    double Y_sd=Y0-Y_min;
    if (Y_sd==dY) Y_sd=0;

    //далее начинаю рисовать


    //рисую оси
    painter->setPen(QPen(Qt::black,3));
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawLine(QPoint(x(X_min)-x_2,y(Y_min)),QPoint(x(X_max)+x_2,y(Y_min)));
    painter->drawLine(QPoint(x(X_min),y(Y_min)+y_2),QPoint(x(X_min),y(Y_max)-y_2));

    //qDebug()<<"1";
//    if(X_min*X_max <= 0){
//        painter->drawLine(QPoint(x(X_min)-5,y(0)),QPoint(x(X_max),y(0)));
//    }
//    else{
//        painter->drawLine(QPoint(x(X_min)-5,y(Y_min)),QPoint(x(X_max),y(Y_min)));
//    }
//    if(Y_min*Y_max <= 0){
//        painter->drawLine(QPoint(x(0),y(Y_min)+5),QPoint(x(0),y(Y_max)));
//    }
//    else{
//        painter->drawLine(QPoint(x(X_min),y(Y_min)+5),QPoint(x(X_min),y(Y_max)));
//    }
    //qDebug()<<"2";
    //painter->drawLine(QPoint(x_0,h-y_0+5),QPoint(x_0,0));
    
    
    //рисую сетку
    painter->setPen(QPen(Qt::black,1));
    painter->setFont(QFont("Times", 14, QFont::Normal));
    for (double X=X_min+X_sd;X<=X_max;X+=dX)
    {
        painter->drawLine(QPoint(x(X),y(Y_min)+y_2),QPoint(x(X),y(Y_max)-y_2));
        painter->drawText(QRect(x(X)-5,y(Y_min)+5,95,25),QString::number(X));
    }

    painter->setPen(QPen(Qt::black,1));
    for (double Y=Y_min+Y_sd;Y<=Y_max;Y+=dY)
    {
        painter->drawLine(QPoint(x(X_min)-x_2,y(Y)),QPoint(x(X_max)+x_2,y(Y)));
        painter->drawText(QRect(x(X_min)-98,y(Y)-10,90,25),Qt::AlignRight,QString::number(Y));

    }

    double X;
    for (X = X_min + X_sd; X>X_min; X-=dX/5.);
    painter->setPen(QPen(Qt::gray,1,Qt::DashLine));
    for (X+=dX/5.;X<=X_max;X+=dX/5.)
    {
        painter->drawLine(QPoint(x(X),y(Y_min)+y_2),QPoint(x(X),y(Y_max)-y_2));
    }

    double Y;
    for (Y = Y_min + Y_sd; Y>Y_min; Y-=dY/5.);
    painter->setPen(QPen(Qt::gray,1,Qt::DashLine));
    for (Y+=dY/5.;Y<=Y_max;Y+=dY/5.)
    {
        painter->drawLine(QPoint(x(X_min)-x_2,y(Y)),QPoint(x(X_max)+x_2,y(Y)));
    }

}

void grafWidget::extremes()
{
    //qDebug()<<"extrems start";
    X_max=X_min=Y_max=Y_min=0;

//инициализация
    for(int i=0, k=1; i<grafSourceList.size() && k;i++)
    {
        //qDebug()<<grafSourceList.size()<<i;
        if(!grafSourceList[i]->xyList.isEmpty())
        {

            X_min=grafSourceList[i]->xyList[0].x();
            X_max=grafSourceList[i]->xyList[0].x();
            Y_min=grafSourceList[i]->xyList[0].y();
            Y_max=grafSourceList[i]->xyList[0].y();
            k=0;
        }
    }
    //qDebug()<<"extrems 5";
    for(int i=0; i<grafSourceList.size();i++)
        if(!grafSourceList[i]->xyList.isEmpty())
        {
            //qDebug()<<i<<grafSourceList[i];
            nPoint= (grafSourceList[i]->iList < grafSourceList[i]->n || grafSourceList[i]->n == 0)
                    ? grafSourceList[i]->iList
                    : grafSourceList[i]->n;
            for(int j=0;j<nPoint;j++)
            {
                if (1/grafSourceList[i]->xyList[j].x() != 0)
                {
                    if (grafSourceList[i]->xyList[j].x()<X_min) X_min=grafSourceList[i]->xyList[j].x();
                    if (grafSourceList[i]->xyList[j].x()>X_max) X_max=grafSourceList[i]->xyList[j].x();
                }
                if(1/grafSourceList[i]->xyList[j].y() != 0)
                {
                    if (grafSourceList[i]->xyList[j].y()<Y_min) Y_min=grafSourceList[i]->xyList[j].y();
                    if (grafSourceList[i]->xyList[j].y()>Y_max) Y_max=grafSourceList[i]->xyList[j].y();
                }
            }
        }
    // защита от бесконечностей
    //qDebug()<<"extrems 10";
    if(1/X_max == 0 && 1/X_min == 0)
    {
        X_max = 1; X_min = -1;
    }
    else
    {
        if (1/X_max == 0) X_max = X_min+1;
        if (1/X_min == 0) X_min = X_max-1;
    }

    if(1/Y_max == 0 && 1/Y_min == 0)
    {
        Y_max = 1; Y_min = -1;
    }
    else
    {
        if (1/Y_max == 0) Y_max = Y_min+1;
        if (1/Y_min == 0) Y_min = Y_max-1;
    }
    //защита от NaN

    if(X_max!=X_max && X_min!=X_min)
    {
        X_max = 1; X_min = -1;
    }
    else
    {
        if(X_max!=X_max) X_max = X_min+1;
        if(X_min!=X_min) X_min = X_max-1;
    }

    DX=X_max-X_min, DY=Y_max-Y_min;

    //qDebug()<<X_max<<X_min<<Y_max<<Y_min;
    if (DX<1e-8 && X_max==0) X_max=1;
    if (DX<1e-8 && X_max>0) X_min=0;
    if (DX<1e-8  && X_max<0) X_max=0;
    if (DY<1e-8  && Y_max==0) Y_max=1;
    if (DY<1e-8  && Y_max>0) Y_min=0;
    if (DY<1e-8  && Y_max<0) Y_max=0;
    DX=X_max-X_min, DY=Y_max-Y_min;

    X_max+=DX*0.05;
    Y_max+=DY*0.05;
    if (X_min!=0) X_min-=DX*0.05;
    if (Y_min!=0) Y_min-=DY*0.05;
    //qDebug()<<"extrems end";
}

void grafWidget::step()
{
DX=X_max-X_min, DY=Y_max-Y_min;
}

void grafWidget::plotGraf()
{
    //рисую график
    //qDebug()<<"plotGraf()";
    QPolygonF polygon;
    for(int i=0; i<grafSourceList.size();i++)
        if(!grafSourceList[i]->xyList.isEmpty())
        {
            //qDebug()<<i<<grafSourceList[i];
            nPoint= (grafSourceList[i]->iList < grafSourceList[i]->n || grafSourceList[i]->n == 0)
                    ? grafSourceList[i]->iList
                    : grafSourceList[i]->n;
            //qDebug()<<nPoint;
            painter->setPen(grafSourceList[i]->pen);
            polygon.clear();
            for(int j=0;j<nPoint;j++)
            {
                        polygon<<XY_to_xy(grafSourceList[i]->xyList[j]);
            }

            painter->drawPolyline(polygon);
        }
}

void grafWidget::plotNames()
{
    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);
    painter->drawRect(QRect(QPoint(0,0),QPoint(w,y_1-y_2)));
    painter->drawRect(QRect(QPoint(0,0),QPoint(x_0-x_2,h)));
    painter->drawRect(QRect(QPoint(0,h-y_0+y_2),QPoint(w,h)));
    painter->drawRect(QRect(QPoint(w-x_1+x_2,0),QPoint(w,h)));
    painter->setPen(QPen(Qt::black,1));//x_0,y_1-y_4,x_1
    painter->setFont(QFont("Times", 25, QFont::Normal));
    painter->drawText(QRect(QPoint(x_0,y_4),QPoint(w-x_1,y_1-y_4)),Qt::AlignCenter,nameGraf);
    painter->save();
    painter->translate(0,h);painter->rotate(-90);
    painter->setFont(QFont("Times", 20, QFont::Normal));
    painter->drawText(QRect(QPoint(y_0,y_5),QPoint(h-y_1,x_0-y_6)),Qt::AlignHCenter|Qt::AlignBottom,nameY);
    painter->restore();
    painter->setFont(QFont("Times", 20, QFont::Normal));
    painter->drawText(QRect(QPoint(x_0,h-y_0+y_8),QPoint(w-x_1,h-y_7)),Qt::AlignHCenter|Qt::AlignTop,nameX);
    painter->setFont(QFont("Times", 15, QFont::Normal));
    painter->drawRect(QRect(QPoint(0+5,y_4),QPoint(x_0-80,y_1-y_4)));
    painter->drawRect(QRect(QPoint(0+5,y_4+y_1),QPoint(x_0-80,y_1-y_4+y_1)));
    painter->drawText(QRect(QPoint(0+5,y_4),QPoint(x_0-80,y_1-y_4)),Qt::AlignCenter,"reset");
    painter->drawText(QRect(QPoint(0+5,y_4+y_1),QPoint(x_0-80,y_1-y_4+y_1)),Qt::AlignCenter,"save");

}

void grafWidget::plotSelection()
{
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);
    painter->drawRect(QRect(QPoint(x(X_min1),y(Y_min1)),QPoint(x(X_max1),y(Y_max1))));
    //qDebug()<<x(X_min1)<<y(Y_min1)<<pe->x()<<pe->y();

}

void grafWidget::save()
{
    QFileDialog *FD=new QFileDialog;
    QString str =FD->getSaveFileName(0,"сохранение файла","./saveFile","*.txt");//qDebug()<<str;
    if (!str.isEmpty()){
        if(!str.endsWith(".txt")) str+=".txt";
        QFile file(str);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream strm(&file);
            for(int i=0; i<grafSourceList.size();i++)
                if(!grafSourceList[i]->xyList.isEmpty())
                {
                    //qDebug()<<i<<grafSourceList[i];
                    nPoint= (grafSourceList[i]->iList < grafSourceList[i]->n || grafSourceList[i]->n == 0)
                            ? grafSourceList[i]->iList
                            : grafSourceList[i]->n;
                    strm<<"graph "<<i<<":\n"<<"x\t y\n";
                    for(int j=0;j<nPoint;j++)
                    {
                        strm<<grafSourceList[i]->xyList[j].x()<<"\t"<<grafSourceList[i]->xyList[j].y()<<"\n";
                    }

                }

            file.close();
        }
    }
}

void grafWidget::disp()
{
    painter->setPen(QPen(Qt::black,1));
    painter->drawText(0+5,h-5,"x="+QString::number(X(x_disp))+"; y="+QString::number(Y(y_disp)));
}

void grafWidget::plotLegend()
{
    //qDebug()<<"test";
    double h_line = 30;
    painter->setPen(QPen(Qt::red,1));
    painter->drawText(w-x_legend,50, x_legend - 20 , 60, Qt::TextWordWrap, "test test test test test test test test test ");
    //painter->drawText();
}

void grafWidget::addListPoint(grafSource *sourcePTR)
{
    grafSourceList<<sourcePTR;
    N_Graf++;
}

void grafWidget::legendVisible(bool visible)
{
    legend_kl = visible;
}


void grafWidget::mousePressEvent(QMouseEvent * pe)
{
    double Xm = X(pe->x()), Ym=Y(pe->y());
    if (Xm>X_min && Xm<X_max && Ym>Y_min && Ym<Y_max)
    {
        if (pe->buttons() & Qt::RightButton)
        {
            mouseZoomEvent_kl = 1;
            X_min1=Xm;
            Y_min1=Ym;
            //qDebug() <<"mousePressEvent"<<Xm<<Ym;
        }

        if (pe->buttons() & Qt::LeftButton)
        {
            mouseDrag_kl = 1;
            X_min1=Xm;
            Y_min1=Ym;
            zoom_kl = 1;
            //qDebug() <<"mousePressEvent"<<Xm<<Ym;
        }

        if (pe->buttons() & Qt::MidButton)
        {
            disp_kl=1;
            x_disp=pe->x();
            y_disp=pe->y();
            update();
        }
    }

}

void grafWidget::mouseReleaseEvent(QMouseEvent * pe)
{
    //painter->drawText(QRect(QPoint(0+5,y_4),QPoint(x_0-10,y_1-y_4)),Qt::AlignCenter,"reset");
    //painter->drawRect(QRect(QPoint(0+5,y_4),QPoint(x_0-80,y_1-y_4)));
    //painter->drawRect(QRect(QPoint(0+5,y_4+y_1),QPoint(x_0-80,y_1-y_4+y_1)));

    double xm = pe->x(), ym=pe->y();
    //qDebug()<<mousePressEvent_kl;
    if (!mouseZoomEvent_kl && !mouseDrag_kl)
    {
        if (xm>5 && xm<(x_0-10) && ym>y_4 && ym<y_1-y_4)
        {
            //qDebug()<<"reset"<<y_4<<y_1-y_4;
            zoom_kl=0;
            update();
        }
        if (xm>5 && xm<(x_0-10) && ym>y_4+y_1 && ym<y_1-y_4+y_1)
        {
            //qDebug()<<"save"<<y_4+20<<y_1-y_4+20;
            save();

        }
    }
    if(mouseZoomEvent_kl)
    {

        double Xm = X(pe->x()), Ym=Y(pe->y());
        //if (!(Xm>X_min && Xm<X_max && Ym>Y_min && Ym<Y_max && X_min0!=Xm && Y_min0!=Ym))
        if (Xm<X_min)
        {
            Xm=X_min;
        }
        if (Xm>X_max)
        {
            Xm=X_max;
        }
        if (Ym<Y_min)
        {
            Ym=Y_min;
        }
        if (Ym>Y_max)
        {
            Ym=Y_max;
        }

        if (Xm<X_min1)
        {
            X_max=X_min1;
            X_min=Xm;
        }
        else
        {
            X_max=Xm;
            X_min=X_min1;
        }
        if (Ym<Y_min1)
        {
            Y_max=Y_min1;
            Y_min=Ym;
        }
        else
        {
            Y_max=Ym;
            Y_min=Y_min1;
        }
        zoom_kl=1;
        if ((X_max-X_min)/(X_max+X_min)<1e-7 && (Y_max-Y_min)/(Y_max+Y_min)<1e-7) zoom_kl=0;
        //qDebug() <<"mouseReleaseEvent"<<Xm<<Ym;


    }

    mouseZoomEvent_kl = 0;
    mouseDrag_kl=0;
    disp_kl=0;
    //qDebug()<<"end"<<mousePressEvent_kl;
    //mouseMoveEvent_kl = 0;

    update();
}

void grafWidget::mouseMoveEvent(QMouseEvent * pe)
{
    if(mouseZoomEvent_kl)
    {
        mouseMoveEvent_kl=1;
        X_max1=X(pe->x());
        Y_max1=Y(pe->y());
        update();
        //mouseMoveEvent_kl=0;
    }
    if(mouseDrag_kl)
    {
        X_max1=X(pe->x());
        Y_max1=Y(pe->y());
        X_max+=X_min1-X_max1;
        Y_max+=Y_min1-Y_max1;
        X_min+=X_min1-X_max1;
        Y_min+=Y_min1-Y_max1;
        update();
    }
    if(disp_kl)
    {
        x_disp=pe->x();
        y_disp=pe->y();
        update();
    }
}
