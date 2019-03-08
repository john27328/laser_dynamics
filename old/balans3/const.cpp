#include "balansrk.h"
//    QMap<QString,QString> mapPhonebook;
//    mapPhonebook["Piggy"] = "+380 93 785 11 11";
//    mapPhonebook["Kermit"] = "+7 85 123 65 56";
//    mapPhonebook["Gonzo"] = "+49 73 631 32 21";

//    QMap<QString,QString>::iterator it = mapPhonebook.begin();
//    for(;it != mapPhonebook.end(); ++it)
//    {
//       qDebug() << "Name:" << it.key() << "Phone:" << it.value();
//    }

    /*********************************************************************************
     * Здесь надо объявить все переменные                                            *
     *          ADD_CONST(valueDisp, name, dimConst, dimDisp, ratioConstToDisp, visible)  *
     * например:                                                            *
     *          ADD_CONST(3e10,"c","см*с","мс",100,1)                         *
     * или переменные                                                       *
     *          ADD_VAR(name,expression, dimConst)           *
     ************************************************************************/
    //tm:YLF

    ADD_CONST(A,3,"W","W", 1,1);
    ADD_VAR(B,2*A,"");

