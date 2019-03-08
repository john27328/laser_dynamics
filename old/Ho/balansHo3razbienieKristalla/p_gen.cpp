double BalansRK::P_gen(double q)
{
    //qDebug()<<"P_gen";
    return q_pump * q * ((-log(1 - T2) * c) / (2 * L)) * h * c / lambda_gen;
}
