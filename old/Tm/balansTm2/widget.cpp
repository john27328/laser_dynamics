#include "widget.h"
#include "ui_widget.h"
#include <QTextStream>
#include <QTimer>


using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    y= new Y;

    ui->graf->addListPoint(y->Pgraf);
    ui->graf->nameGraf = "мощность генерации Tm-лазера";
    ui->graf->nameY = "мощность, Вт";
    ui->graf->nameX = "время, сек";

//    ui->graf_n->addListPoint(y->y1graf);
//    ui->graf_n->addListPoint(y->y2graf);
//    ui->graf_n->addListPoint(y->y3graf);

//    ui->graf_n->nameGraf = "нормированные населенности";
//    ui->graf_n->nameY = "концентрация";
//    ui->graf_n->nameX = "время, сек";

    ui->comboBoxTm->addItem("Tm:YLF");
    ui->comboBoxTm->addItem("Tm:YAP");
    connect(ui->PushButtonStart,SIGNAL(clicked(bool)),this,SLOT(startSlot()));
    ui->graf->update();
    connect(y,SIGNAL(grafUpdate()),this,SLOT(plotGraf()));
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(stopSlot()));
    connect(ui->doubleSpinBoxHt,SIGNAL(valueChanged(double)),this,SLOT(lcdHSlot()));
    connect(ui->spinBoxHN,SIGNAL(valueChanged(int)),this,SLOT(lcdHSlot()));
    lcdHSlot();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::startSlot()
{
    qDebug()<<"start";
    //задаю начальные условия
    y->set_y0(0,1,0,0);
    //y->set_y0(0,0.878443, 0.116576, 0.00498121);
    //y->setParametri(200,20,3,0.16);
    //задаю параметры кристалла
    y->setP(ui->doubleSpinBoxP->value());
    y->setT_out(ui->doubleSpinBoxTout->value());
    y->setN_pr(ui->doubleSpinBoxN->value());
    y->setT_end(ui->doubleSpinBoxTend->value());
    y->setNDots(ui->spinBoxNDots->value());
    y->setL(ui->doubleSpinBox_l->value()/1000.);
    y->setD(ui->doubleSpinBoxD->value()/1000.);
    int kristall_item = ui->comboBoxTm->currentIndex();

    //рассчитываю параметры выбранного кристалл
    switch (kristall_item) {
    case 0:
        y->tm_YLF_3pr();
        break;
    case 1:
        y->tm_YAP_4pr();
        break;
    default:
        break;
    }

    double H=ui->doubleSpinBoxHt->value() / pow(10,ui->spinBoxHN->value());
    qDebug()<<H;

    y->fyk(H);
    //t=0:0.000001:0.16;
}

void Widget::stopSlot()
{
    y->stop_fyk();
}

void Widget::plotGraf()
{
 ui->graf->update();
 ui->lcdNumberP_gen->display(y->P_gen());
 ui->lcdNumber_tk->display(y->getT());
 //qDebug()<<y->getT();

 //for(int i=0;i<y->Pgraf->iList;i++)
 //qDebug()<<y->Pgraf->xyList[i].x()<<y->Pgraf->xyList[i].y();
}

void Widget::lcdHSlot()
{
    double H = ui->doubleSpinBoxHt->value() / pow(10,ui->spinBoxHN->value());
    ui->lcdNumberH->display(H);
}

