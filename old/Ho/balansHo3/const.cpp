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
     *         ADD_CONST(name, value,  dimDisp, dimConst, ratioDisplayToConst,visible)  *
     * например:                                                            *
     *          ADD_CONST(c, 3,"10^8 м/c","см/с",1e-10,1)                         *
     * или переменные                                                       *
     *         ADD_VAR(name,expression, dimConst)          *
     ************************************************************************/

    //ADD_CONST(A,3,"W","W", 1,1);
    //ADD_VAR(B,2*A,"");

//    ADD_CONST(h, 6626, "*10^-37 Дж*с", "Дж*с", 1e37, 0)
//      ADD_VAR(L_eff, L + (n - 1) * l, "cm")

    ADD_CONST(N0, 1.38, "*10^20 cm^-3", "m-3", 1e-26, 1)
    ADD_CONST(K_ap, 1.28, "*10^-18 cm^3*c-1", "m^3*c-1", 1e24, 1)
    ADD_CONST(A21, 133, "c-1", "c-1", 1, 1)
    ADD_CONST(sigma_g, 1.32, "*10^-20 cm^2 lambda = 2090", "m^2", 1e24, 1)
    ADD_CONST(sigma_a, 0.25, "*10^-20 cm^2 lambda = 2090", "m^2", 1e24, 1)
    ADD_CONST(K_sigma, 0.96, "cm-1", "m-1", 1e-2, 1)
    ADD_CONST(d, 0.55, "mm", "m", 1000, 1)
    ADD_CONST(rho_bad, 0.035, "c-1", "c-1", 1, 1)
    ADD_CONST(L, 12, "cm", "m", 100, 1)
    ADD_CONST(T1, 3, "%", "1", 100, 1)
    ADD_CONST(T2, 35, "%", "1", 100, 1)
    ADD_CONST(l,40, "mm", "m", 1000, 1)
    ADD_CONST(P, 20, "W полная мощность, дожигание", "W", 1, 1)
      ADD_VAR(P_pump, P * (1 - exp(-K_sigma * 2 * l)), "W")

    ADD_CONST(pi, 3.14, "1", "1", 1, 0)

      ADD_VAR(S, pi * pow(d,2) / 4, "m")
    ADD_CONST(lambda_pump, 1908, "нм", "m", 1e9, 1)
    ADD_CONST(lambda_gen, 2090, "нм", "m", 1e9, 1)
    ADD_CONST(c, 3e8, "m" , "m", 1, 0)
    ADD_CONST(h, 6.62e-34, "Дж*с", "Дж*с", 1, 0)
      ADD_VAR(R0, P_pump / (S * l * h * c / lambda_pump), "m-3 * c-1")
      ADD_VAR(V, S*L, "m^3")
      ADD_VAR(V_a, S * l, "m^3")
      ADD_VAR(B_g, sigma_g * c / V, "c * m-1")
      ADD_VAR(B_a, sigma_a * c / V, "c * m-1")
      ADD_VAR(rho, rho_bad - c / L * (log(1 - T1) + log(1 - T2))/ 2, "c-1" )

//нормировка
    ADD_VAR(R0n,R0 / N0, "c-1")
    ADD_VAR(K_ap_n, K_ap * N0, "c-1")
    ADD_VAR(q_pump, P_pump * L * lambda_pump / h / pow(c,2), "1")


