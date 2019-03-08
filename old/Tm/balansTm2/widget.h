#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QDebug>
#include <QProcess>
#include <iostream>
#include "y.h"


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
    Y *y;

public slots:
    void startSlot();
    void stopSlot();
    void plotGraf();
    void lcdHSlot();


};

#endif // WIDGET_H
