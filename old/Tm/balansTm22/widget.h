#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QDebug>
#include <QProcess>
#include <iostream>
#include "y.h"
#include <QTextStream>
#include <QTimer>
#include <QSettings>
#include <QString>
#include <QDoubleValidator>
#include <QLocale>

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
    QString textStr;



public slots:
    void startSlot();
    void stopSlot();
    void plotGraf();
    void lcdHSlot();
    void TmSlot();


};

#endif // WIDGET_H
