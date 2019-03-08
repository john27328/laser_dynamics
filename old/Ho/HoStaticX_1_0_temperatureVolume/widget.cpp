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
    connect(ui->pushButtonLoad,SIGNAL(clicked(bool)),this,SLOT(loadTemperature()));
    connect(ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(closeTemperature()));
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

void Widget::loadTemperature()
{
    QFileDialog *FD=new QFileDialog;
    QString str =FD->getOpenFileName(0,"сохранение файла","./Tm:YAP","Text (*.csv *.txt);;All files (*.*)");
    qDebug()<<"loadTmYAP()";
    QFile file(str);
    if (!str.isEmpty())
    {
        if ( !file.open(QFile::ReadOnly | QFile::Text) )
        {
            qDebug() << "File not exists";
        }
        else
        {
            y->T_load_key = 1;
            QTextStream in(&file);
            QVector<QStringList> vector;
            // Считываем данные до конца файла
            while (!in.atEnd())
            {
                // ... построчно
                QString line = in.readLine();

                vector << line.split("\t");
            }
            int N_SL = (vector[0][3]).toDouble();
            int N_CRCL = (vector[0][5]).toDouble();
            double D_CR = (vector[1][1]).toDouble();
            double L_CR = (vector[1][3]).toDouble();
            double D_Pump = (vector[1][5]).toDouble();
            double P_Pump = (vector[1][7]).toDouble();
            double T_CR_PR = (vector[1][9]).toDouble();
            db(vr(N_SL) vr(N_CRCL)
               vr(D_CR) vr(L_CR)
               vr(D_Pump) vr(P_Pump));

            if (y->T_cr_dl) delete y->T_cr_dl;
            y->T_cr_dl = new double[N_SL];

            for (int k = 3, i = 0; k < vector.length() && i<N_SL; i++, k++)
            {
                double TS = 0; //усреднение
                double dD = D_CR/N_CRCL;
                for (int j = 0; j<vector[k].length(); j++)
                {
                    if (dD*(j+1)<D_Pump) TS += (vector[k][j]).toDouble();;
                }
                TS = TS / N_SL;
                db(vr(TS));

                y->T_cr_dl[i] = TS;
            }

            mapDSB["Nrasb"]->setValue(N_SL);
            mapDSB["P"]->setValue(P_Pump);
            mapDSB["l_kr"]->setValue(L_CR);
            db(vr(N_SL) vr(P_Pump) vr(L_CR));
            mapDSB["Nrasb"]->setEnabled(0);
            mapDSB["P"]->setEnabled(0);
            mapDSB["l_kr"]->setEnabled(0);
            mapDSB["TRoom"]->setEnabled(0);

            getValue();

            file.close();
        }
    }
}

void Widget::closeTemperature()
{
    y->T_load_key = 0;
    mapDSB["Nrasb"]->setEnabled(1);
    mapDSB["P"]->setEnabled(1);
    mapDSB["l_kr"]->setEnabled(1);
    mapDSB["TRoom"]->setEnabled(1);
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
