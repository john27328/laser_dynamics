#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //qDebug()<<"Widget";
    ui->setupUi(this);
    y = new BalansRK;
    ui->graf->addListPoint(y->Pgraf);
    ui->graf->nameGraf = "мощность генерации Tm-лазера";
    ui->graf->nameY = "мощность, Вт";
    ui->graf->nameX = "время, сек";

    ui->grafN->addListPoint(y->y1graf);
    ui->grafN->addListPoint(y->y2graf);
    ui->grafN->addListPoint(y->y3graf);

    ui->grafN->nameGraf = "нормированные населенности";
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
            tempSB->setRange(0,9999);
            connect(tempSB,SIGNAL(valueChanged(double)),this,SLOT(getValue()));
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
    delete ui;
}

void Widget::setValue()
{
     QMapIterator<QString,Constants *> iMap(y->cMap);
     QMapIterator<QString,QDoubleSpinBox *> iMapDSB(mapDSB);
     while (iMap.hasNext())
     {
         //static int i=0;
         iMap.next();
         if(iMap.value()->visible()) {

             iMapDSB.next();
             iMapDSB.value()->setValue(iMap.value()->value());
         }

     }
}

void Widget::getValue()
{
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
}

void Widget::startSlot()
{
    //qDebug()<<"start Slot";
    //ui->PushButtonStart->setEnabled(0);
    //qDebug()<<"start";
    textStr+="===========================\n";
    //задаю начальные условия
    y->set_y0(0,1,0,0);
    //textStr+="\t y1 = " + QString::number()
    //y->set_y0(0,0.878443, 0.116576, 0.00498121);
    //y->setParametri(200,20,3,0.16);
    //задаю параметры кристалла


    //ui->textBrowser->setText(textStr);
    //qDebug()<<"y->start_fyk(1e-9);";
    y->start_fyk(1e-9);

    //qDebug()<<"end";
 //   ui->PushButtonStart->setEnabled(1);
    //t=0:0.000001:0.16;
}

void Widget::stopSlot()
{
    qDebug()<<"stopSlot";
    y->stop_fyk();
}

void Widget::plotGraf()
{
    //qDebug()<<"plotGraf";
 ui->graf->update();
 ui->grafN->update();
 //ui->lcdNumberP_gen->display(y->P_gen());
 //ui->lcdNumber_tk->display(y->getT());
 //qDebug()<<y->getT();

 //for(int i=0;i<y->Pgraf->iList;i++)
 //qDebug()<<y->Pgraf->xyList[i].x()<<y->Pgraf->xyList[i].y();
}
