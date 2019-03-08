#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QDebug>

class Constants
{
public:
    Constants();
    Constants(double *value, QString name, QString constDim, QString dispDim, double ratioDtoC, bool visible);
    double value();
    void setValue(double value);
    QString name();
    QString dimention();
    bool visible();
private:
    QString constName;
    double *constValue;
    QString constDimension;
    QString displayDimension;
    double ratioDisplayToConst;
    bool constVisible;
    double display();
};

#endif // CONSTANTS_H
