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
    //qDebug()<<"value"<<*constValue<<constName<<constDimension;
    return *constValue*ratioDisplayToConst;
}

void Constants::setValue(double value)
{
    //qDebug()<<"setValue"<<value<<constName<<constValue;
    *constValue = value/ratioDisplayToConst;
}

QString Constants::name()
{
    return constName;
}

QString Constants::dimention()
{
    return displayDimension;
}

bool Constants::visible()
{
    return constVisible;
}

double Constants::display()
{

}


