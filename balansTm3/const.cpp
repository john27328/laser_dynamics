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

    ADD_CONST(N_3Pr, 4.1,"10^26 м^3","m^3", 1e-26, 1)
    ADD_CONST(N_pr, 3,"%","%", 1, 1)
      ADD_VAR(N, N_3Pr/3*N_pr, "1")

    ADD_CONST(beta0 ,3500, "c", "c" , 1, 0)
      ADD_VAR(beta, beta0/3. * N_pr, "c")
    ADD_CONST(W31, 360, "cm^-3", "cm^-3", 1, 1)
    ADD_CONST(W32, 40, "cm^-3", "cm^-3", 1, 1)
    ADD_CONST(W21, 67, "cm^-3", "cm^-3", 1, 1)
    ADD_CONST(h, 6.62e-34, "?", "?", 1, 0 )
    ADD_CONST(c, 3e8, "m/c", "m/c", 1, 0)
    ADD_CONST(n, 1.7, "1", "1", 1, 1)
    ADD_CONST(lambda_p, 793, "nm", "m", 1e9, 1)
    ADD_CONST(lambda_g, 1908, "nm", "m", 1e9, 1)
    ADD_CONST(sigma_e_gen, 0.25, "*10^-24 1/m^2", "1/m^2", 1e24, 1)
    ADD_CONST(sigma_a_gen, 0.04, "*10^-24 1/m^2", "1/m^2", 1e24, 1)
    ADD_CONST(P, 40, "W", "W", 1, 1)
    ADD_CONST(d, 1, "mm", "m", 1e3, 1)
    ADD_CONST(alpha_i,1,"поправочный коэффициент", "1", 1, 1)
    ADD_CONST(beta_j, 1, "поправочный коэффициент 0.2", "1", 1, 1)
    ADD_CONST(L_res, 18, "mm", "m", 1e3, 1)
    ADD_CONST(l, 6, "mm", "m", 1e3,1)
      ADD_VAR(Le, L_res + (n - 1) * l, "m")
      ADD_VAR(Va, 3.14 * pow(d,2) / 4 * l, "m")
      ADD_VAR(V, Le/l*Va, "")
    ADD_CONST(T_out, 20, "%", "1", 100, 1)
      ADD_VAR(gamma_1, - log(1-0.005), "1")
      ADD_VAR(gamma_2, - log(1-T_out), "1")
    ADD_CONST(gamma_i_key, 0, "занулить, чтобы убрать внутрение потери", "", 1,  1)
      ADD_VAR(gamma_i, - (gamma_i_key ? 0 : 1) * log(1-exp(-1/(sigma_a_gen*N*l))), "1")
      ADD_VAR(rho, c / Le * (gamma_i + (gamma_1 + gamma_2) / 2), "")
      ADD_VAR(B_e, alpha_i * c / V * sigma_e_gen, "")
      ADD_VAR(B_a, beta_j * c / V * sigma_a_gen, "")
      ADD_VAR(K, (lambda_p * P * 4) / (N * l * h * c * 3.14 * pow(d,2)), "")
    ADD_CONST(tOpenQS, 1000, "mks", "s", 1e6, 1)
    ADD_CONST(fQS, 1, "kHz", "Hz", 1e-3,1)



