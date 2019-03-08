#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "y.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void euler(Y y);
};

#endif // WIDGET_H
