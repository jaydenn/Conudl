#include <cmath>

//nuclear form factor as a function of recoil energy
double ffactorSI(double A, double Er) 
{
    //Standard Helm       
    return 9.05322e6 * exp(-1.9394e-5 * A * Er ) * (-0.00692 * sqrt(2.17706 + pow(-0.6 + 1.23 * pow(A,0.3333),2)) * sqrt(A * Er) * cos(0.00692 * sqrt(2.17706 + pow(-0.6 + 1.23 * pow(A,0.3333),2)) * sqrt(A * Er)) + sin( 0.00692 * sqrt(2.17706 + pow(-0.6 + 1.23 * pow(A,0.3333),2)) * sqrt(A * Er) )) / ( pow( 2.17706 + pow(-0.6 + 1.23 * pow(A,0.3333),2),1.5) * pow(A * Er,1.5));

}
