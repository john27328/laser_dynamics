#include "constants.h"

Constants::Constants()
{

}

Constants::Constants(double *value, QString name, QString constDim, QString dispDim, double ratioDtoC, bool visible)
{


constName = name;
constValue = value;
constDimension = constDim;
displayDimension = dispDim;
ratioDisplayToConst = ratioDtoC;
constVisible = visible;
qDebug()<<constName<<*value;

}

double Constants::value()
{
    //qDebug()<<"name = "<<*constValue;
    return *constValue;
}

void Constants::setValue(double value)
{
    *constValue = value;
}

QString Constants::name()
{
    return constName;
}

QString Constants::dimention()
{
    return constDimension;
}

bool Constants::visible()
{
    return constVisible;
}

double Constants::display()
{

}


