#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //qDebug()<<"Widget";
    ui->setupUi(this);
    ui->spinBoxNDots->setValue(100);
    y = new BalansRK;
    ui->graf->addListPoint(y->VAHgraf);
    //ui->graf->addListPoint(y->PPumpGraf);
    ui->graf->nameGraf = "ВАХ Ho:YAG лазера";
    ui->graf->nameY = "мощность генерации, Вт";
    ui->graf->nameX = "мощность накачки, Вт";

    ui->grafN->addListPoint(y->n1graf);
    ui->grafN->addListPoint(y->n2graf);
    ui->grafN->addListPoint(y->Kgraf);
    ui->grafN->addListPoint(y->q2graf);

    ui->grafN->nameGraf = "населенности";
    ui->grafN->nameY = "населенности";
    ui->grafN->nameX = "мощность накачки, Вт";

    nzgraf = new grafWidget();
    nzgraf->addListPoint(y->n1zgraf);
    nzgraf->addListPoint(y->n2zgraf);
    nzgraf->addListPoint(y->PPumpZgraf);

    nzgraf->nameGraf = "населенности";
    nzgraf->nameY = "населенности";
    nzgraf->nameX = "z, мм";


    connect(ui->pushButtonStart,SIGNAL(clicked(bool)),this,SLOT(startSlot()));


    connect(y,SIGNAL(grafUpdate()),this,SLOT(plotGraf()));
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
    y->variables();
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
    y->start_fyk(0);
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
    ui->graf->update();
    ui->grafN->update();


    double lambda = y->cMap["lambda_pump"]->value();
    //qDebug()<<lambda;
    double sigma = y->sigma_pump(lambda *1e-9);
    double sigma_pr = y->cMap["sigma_pump_pr"]->value()/100;
    ui->lcdNumberSigmaPump->display(sigma_pr * sigma * 1e24);

    sigma = sigma * 1e4;
    double lKr = y->cMap["l_kr"]->value()*1e-1;
    double N0 = y->cMap["N0"]->value()*1e20;

    double THo = exp(-sigma * sigma_pr * N0 * lKr);
    qDebug()<<THo;
    ui->lcdNumberTHo->display(THo * 100);


    nzgraf->show();
    nzgraf->update();
}
