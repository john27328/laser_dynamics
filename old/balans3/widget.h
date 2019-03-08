#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <balansrk.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDoubleSpinBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    BalansRK *y;
    QString textStr;
    QWidget *parametiWidget;
    //QVBoxLayout *parametriVBox;
    QGridLayout * parametriGrid;
    QMap<QString, QDoubleSpinBox *> mapDSB;



public slots:
    void startSlot();
    void stopSlot();
    void plotGraf();
    void setValue();
    void getValue();

};

#endif // WIDGET_H
