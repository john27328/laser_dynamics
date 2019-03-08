#include "widget.h"
#include "ui_widget.h"


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

    ui->grafN->addListPoint(y->y1graf);
    ui->grafN->addListPoint(y->y2graf);
    ui->grafN->addListPoint(y->y3graf);

    ui->grafN->nameGraf = "нормированные населенности";
    ui->grafN->nameY = "населенности";
    ui->grafN->nameX = "время, сек";

    ui->comboBoxTm->addItem("Tm:YLF");
    ui->comboBoxTm->addItem("Tm:YAP");
    connect(ui->PushButtonStart,SIGNAL(clicked(bool)),this,SLOT(startSlot()));
    ui->graf->update();
    connect(y,SIGNAL(grafUpdate()),this,SLOT(plotGraf()));
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(stopSlot()));
    connect(ui->doubleSpinBoxHt,SIGNAL(valueChanged(double)),this,SLOT(lcdHSlot()));
    connect(ui->spinBoxHN,SIGNAL(valueChanged(int)),this,SLOT(lcdHSlot()));
    lcdHSlot();
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),y,SLOT(saveSettings()));
    connect(ui->comboBoxTm,SIGNAL(currentIndexChanged(int)),this,SLOT(TmSlot()));
    TmSlot();
    //QLocale locale(QLocale::English);
    QDoubleValidator *validator = new QDoubleValidator;
    //validator->setLocale(locale);
    ui->lineEditSigma_a_gen->setValidator(validator);
    ui->lineEditSigma_e_gen->setValidator(validator);
    connect(y,SIGNAL(endSaveSignal()),this,SLOT(TmSlot()));
    connect(ui->pushButtoLoading,SIGNAL(clicked(bool)),this,SLOT(TmSlot()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::startSlot()
{
    ui->PushButtonStart->setEnabled(0);
    qDebug()<<"start";
    textStr+="===========================\n";
    textStr+=ui->comboBoxTm->currentText() + " " + QString::number(ui->doubleSpinBoxN->value())+"%\n";
    //задаю начальные условия
    y->set_y0(0,1,0,0);
    //textStr+="\t y1 = " + QString::number()
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
    y->setW21(ui->doubleSpinBoxW21->value());
    y->setW31(ui->doubleSpinBoxW31->value());
    y->setW32(ui->doubleSpinBoxW32->value());
    y->setSigma_a_gen(QLocale().toDouble(ui->lineEditSigma_a_gen->text()));
    y->setSigma_e_gen(QLocale().toDouble(ui->lineEditSigma_e_gen->text()));

    textStr += "P = " + QString::number(ui->doubleSpinBoxP->value()) +"Вт"
            + "\tT_out = " + QString::number(ui->doubleSpinBoxTout->value()) +"%"
             + "\tl = " + QString::number(ui->doubleSpinBox_l->value()) +"мм"
             + "\tD = " + QString::number(ui->doubleSpinBoxD->value()) +"мм\n" ;
             //+"\tt = " + QString::number((ui->doubleSpinBoxTout->value()) +"сек"

    y->Tm_parameters();

    double H=ui->doubleSpinBoxHt->value() / pow(10,ui->spinBoxHN->value());
    qDebug()<<H;


    textStr += "beta = " + QString::number(y->getBeta())
             + "\tW31 = " + QString::number(y->getW31())
             + "\tW32 = " + QString::number(y->getW32())
             + "\tW21 = " + QString::number(y->getW21())
             + "\tsigma_e_gen = "  +QString::number(y->getSigma_e_gen())
             + "\tsigma_a_gen = " + QString::number(y->getSigma_a_gen())
             + "\tlambda_p = " + QString::number(y->getLambda_p())
             + "\tlambda_g = " + QString::number(y->getLambda_g())
             + "\talpha_i = " + QString::number(y->getAlpha_i())
             + "\tbeta_j = " + QString::number(y->getBeta_j())
             + "\tL_res = " + QString::number(y->getLres())
             + "\n";
    ui->textBrowser->setText(textStr);

    y->fyk(H);

    textStr += "P_gen = " + QString::number(y->P_gen())+ "Вт"
             + "\tt_end = " + QString::number(y->getT()) + "сек"
             + "\tq = " + QString::number(y->qk())
             + "\ty1 = " + QString::number(y->y1k())
             + "\ty2 = " + QString::number(y->y2k())
             + "\ty3 = " + QString::number(y->y3k()) + "\n";

    ui->textBrowser->setText(textStr);
    qDebug()<<"end";
    ui->PushButtonStart->setEnabled(1);
    //t=0:0.000001:0.16;
}

void Widget::stopSlot()
{
    y->stop_fyk();
}

void Widget::plotGraf()
{
 ui->graf->update();
 ui->grafN->update();
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

void Widget::TmSlot()
{
    int kristall_item = ui->comboBoxTm->currentIndex();

    //рассчитываю параметры выбранного кристалл
    switch (kristall_item) {
    case 0:
        y->tm_YLF_3pr();
        break;
    case 1:
        y->tm_YAP_3pr();
        break;
    default:
        break;
    }
    ui->doubleSpinBoxW21->setValue(y->getW21());
    ui->doubleSpinBoxW31->setValue(y->getW31());
    ui->doubleSpinBoxW32->setValue(y->getW32());
    //QLocale::toString()
    ui->lineEditSigma_a_gen->setText(QLocale().toString(y->getSigma_a_gen(),'e',2));
    ui->lineEditSigma_e_gen->setText(QLocale().toString(y->getSigma_e_gen(),'e',2));
}
