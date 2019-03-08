#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
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

public:
    void euler(Y y);
    void rk4(Y y);
public slots:
    void startSlot();
};

#endif // WIDGET_H
