#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    Y y;
//    double P=1000;
//    y.P=P;
//    y.tm_YLF_3pr();
//    euler(y);
    connect(ui->pushButton_start,SIGNAL(clicked(bool)),this,SLOT(startSlot()));


}

void Widget::euler(Y y)
{
    //t=0:0.000001:0.16;
    const double dt = 0.000000001;
    const double t1 = 0.16;
    const int i1 =(int)(t1/dt);
    int nDots = 50;
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
    QString display;

    if(ui->checkBoxYLF_YAP->checkState())
    {
        display+="Tm:YAP 4%\n";
    }
    else
    {
        display+="Tm:YLF 3%\n";
    }
    display+="P = " + QString::number(y.P) + "\n";
    display+="============================================================\n";
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
        if ((i/di -(int)(i/di))==0)
        {
            display+=" || P_gen = " +QString::number(y.P_gen(),4,2)
                  //  +"\t|| q = "+QString::number(y.y0)
                    +"\t|| n1 = "+QString::number(y.y1,4,4)
                    +"\t|| n2  = "+QString::number(y.y2,4,4)
                    +"\t|| n3 = "+QString::number(y.y3,4,4)
                    +"\t|| t = "+QString::number(t,4,4)
                    +"||";
            //<<"dq"<<y.dy0<<"dn1"<<y.dy1<<"dn2"<<y.dy2<<"dn3"<<y.dy3;
            display+="\n"; //<<"q"<<y.y0<<"n1"<<y.y1<<"n2"<<y.y2<<"n3"<<y.y3<<"t"<<t<<"dq"<<y.dy0<<"dn1"<<y.dy1<<"dn2"<<y.dy2<<"dn3"<<y.dy3;
            ui->textBrowser->setText(display);
            QApplication::processEvents();
        }
    }
    display+="============================================================\n";
    ui->textBrowser->setText(display);
    //ui->textBrowser->append("end");
    qDebug()<<"P"<<y.P<<"P_gen"<<y.P_gen()<<"q"<<y.y0<<"n1"<<y.y1<<"n2"<<y.y2<<"n3"<<y.y3<<"t"<<
              t<<"dq"<<y.dy0<<"dn1"<<y.dy1<<"dn2"<<y.dy2<<"dn3"<<y.dy3;
}

void Widget::rk4(Y y)
{
    int j;
    qDebug()<<"N="<<y.N;
    int nDots = 50;
      QString display;
    if(y.Nm<y.N) qDebug()<<"o_O Nm<N!?\n";
    else {
        y.yk[0]=0;y.yke[0]=0;
        y.yk[1]=1;y.yke[1]=1;
        y.yk[2]=0;y.yke[2]=0;
        y.yk[3]=0;y.yke[3]=0;
        y.fyk();
        qDebug()<<"fykEnd";
        if(y.k<1)
            for(j=0;j<y.N;j+=y.N/nDots)
            { qDebug()<<y.P_gen(y.y[0][j]);
                display+=" || P_gen = " +QString::number(y.P_gen(y.y[0][j]),4,2)
                      //  +"\t|| q = "+QString::number(y.y0)
                        +"\t|| n1 = "+QString::number(y.y[1][j],4,4)
                        +"\t|| n2  = "+QString::number(y.y[2][j],4,4)
                        +"\t|| n3 = "+QString::number(y.y[3][j],4,4)
                        +"\t|| t = "+QString::number(y.H*j,4,4)
                        +"||";
                ui->textBrowser->setText(display);
                QApplication::processEvents();
            }
                else qDebug()<<"reduce the step\n";
    }
qDebug()<<"rk4End";
}

void Widget::startSlot()
{
    qDebug()<<1;
    Y y;
    double P=ui->doubleSpinBox_P->value();
    y.P=P;
    qDebug()<<2;
    qDebug()<<"checkBoxYLF_YAP"<<ui->checkBoxYLF_YAP->checkState();
    if(ui->checkBoxYLF_YAP->checkState())
    {
        y.tm_YAP_4pr();
    }
    else
    {
        y.tm_YLF_3pr();
    }
    //y.tm_YLF_3pr();
    qDebug()<<"checkBox_metod"<<ui->checkBox_metod->checkState();
    if(ui->checkBox_metod->checkState())
        euler(y);\
    else
    {
        qDebug()<<"else";
        rk4(y);
    qDebug()<<"elseEnd";
    }
    qDebug()<<"startSlotEnd";
}

Widget::~Widget()
{
    delete ui;
}
