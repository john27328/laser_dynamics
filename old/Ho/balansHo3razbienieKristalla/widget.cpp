#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //qDebug()<<"Widget";
    ui->setupUi(this);
    ui->spinBoxNDots->setValue(20000);
    ui->doubleSpinBoxTEnd->setValue(10);
    y = new BalansRK;
    ui->graf->addListPoint(y->Pgraf);
    ui->graf->addListPoint(y->PPumpGraf);
    ui->graf->nameGraf = "мощность генерации Ho-лазера";
    ui->graf->nameY = "мощность, Вт";
    ui->graf->nameX = "время, сек";

    ui->grafN->addListPoint(y->y1graf);
    ui->grafN->addListPoint(y->y2graf);
    ui->grafN->addListPoint(y->y3graf);
    ui->grafN->addListPoint(y->y4graf);

    ui->grafN->nameGraf = "населенности";
    ui->grafN->nameY = "населенности";
    ui->grafN->nameX = "время, сек";
    connect(ui->pushButtonStart,SIGNAL(clicked(bool)),this,SLOT(startSlot()));
    ui->graf->update();
    connect(y,SIGNAL(grafUpdate()),this,SLOT(plotGraf()));
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(stopSlot()));
    qDebug()<<"Widget end";
    parametiWidget = new QWidget;
    //parametriVBox = new QVBoxLayout;
    parametriGrid = new QGridLayout;
    parametiWidget->setLayout(parametriGrid);
    //parametiWidget->show();
    ui->scrollArea->setWidget(parametiWidget);

    QMapIterator<QString,Constants *> iMap(y->cMap);
           QDoubleSpinBox *tempSB;

    while (iMap.hasNext())
    {
        static int i=0;
        iMap.next();
        if(iMap.value()->visible()) {
            parametriGrid->addWidget(new QLabel(iMap.value()->name()),i,0);
            tempSB = new QDoubleSpinBox;
            mapDSB[iMap.key()]= tempSB;
            tempSB->setRange(0,999999);
            //connect(tempSB,SIGNAL(valueChanged(double)),this,SLOT(getValue()));
            //tempSB->setValue(iMap.value()->value());
            parametriGrid->addWidget(tempSB,i,1);
            parametriGrid->addWidget(new QLabel(iMap.value()->dimention()),i,2);
            i++;
        }
    }
    setValue();

    //QMapIterator<QString,Constants *> iMap(y->cMap);
//    while (iMap.hasNext())
//    {
//        iMap.next();
//        qDebug()<<iMap.key()<<iMap.value()->value();
//    }
}

Widget::~Widget()
{
    //stopSlot();
    //qDebug()<40<"kill";
    delete y;
    delete ui;
}

void Widget::setValue()
{
    //qDebug()<<"setValue(); start";
     QMapIterator<QString,Constants *> iMap(y->cMap);
     QMapIterator<QString,QDoubleSpinBox *> iMapDSB(mapDSB);
     while (iMap.hasNext())
     {
         //static int i=0;
         iMap.next();
         if(iMap.value()->visible()) {

             iMapDSB.next();
             iMapDSB.value()->setValue(iMap.value()->value());
             qDebug()<<"setValue"<<iMapDSB.key()<<iMap.key()<<iMap.value()->value();
             connect(iMapDSB.value(),SIGNAL(valueChanged(double)),this,SLOT(getValue()));
         }

     }
     //qDebug()<<"setValue end";
}

void Widget::getValue()
{
    qDebug()<<"getValue() start";
    QMapIterator<QString,Constants *> iMap(y->cMap);
    QMapIterator<QString,QDoubleSpinBox *> iMapDSB(mapDSB);
    while (iMap.hasNext())
    {
        //static int i=0;
        iMap.next();

        if(iMap.value()->visible()) {
            iMapDSB.next();
            iMap.value()->setValue(iMapDSB.value()->value());
        }

    }
    qDebug()<<"getValue end";
}

void Widget::startSlot()
{
    //qDebug()<<"start Slot";
    //ui->PushButtonStart->setEnabled(0);
    //qDebug()<<"start";
    //задаю начальные условия
    y->set_y0(0,1,0);
    //y->set_y0(0,1,0);
    y->setNDots(ui->spinBoxNDots->value());
    y->setT_end(ui->doubleSpinBoxTEnd->value()/1000.);
    y->start_fyk(ui->doubleSpinBoxH->value()*pow(10,(-ui->spinBoxH->value())));
    //QTest::qSleep(1000);
    qDebug()<<"startSlot() end";
}

void Widget::stopSlot()
{
    qDebug()<<"stopSlot";
    y->stop_fyk();
}

void Widget::plotGraf()
{
    //qDebug()<<"plotGraf";
    //qDebug()<<"plotgraf start";
    ui->graf->update();
    ui->grafN->update();
    ui->lcdNumberPgen->display(y->P_gen());
    ui->lcdNumberT->display(y->getT());
    double EGen = y->getEGen()*1000;
    double EPump = y->getEPump()*1000;
    ui->lcdNumberEGen->display(EGen);
    ui->lcdNumberEPump->display(EPump);
    double kpd = EGen/EPump;
    ui->lcdNumberKPD->display(kpd);
    //qDebug()<<"plotgraf end";
    //qDebug()<<y->getT();

    //for(int i=0;i<y->Pgraf->iList;i++)
    //qDebug()<<y->Pgraf->xyList[i].x()<<y->Pgraf->xyList[i].y();
}
