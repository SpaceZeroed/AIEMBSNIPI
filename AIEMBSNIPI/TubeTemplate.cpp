// TubeTemplate1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FlowMaps.h"
#include "FLowMapOrk.h"
using namespace flowmaps;

int main()
{
    PhaseInfo Liquid;
    PhaseInfo Gas;
    double Bo, Bg, Rs, D, qo, qw_ny, qo_ny, qg_ny, fo, fw, Bw, qw, mu_o, mu_w, rho_o, rho_w, Rsw, Roughness, Angle, PInflow, TInflow;
    //  начальные условия  

    //      
    qo_ny = 1590.0 / 86400;
    //     
    qw_ny = 0;
    // 
    D = 0.1524;

    Bo = 1.197;
    Bg = 0.0091;
    Bw = 0.0;

    //  
    mu_o = 0.00097;
    // 
    Roughness = 0.000018288;
    PInflow = 117.13 * 100000;
    TInflow = 82;
    qo = qo_ny * Bo;
    qw = qw_ny * Bw;

    rho_o = 762.64;
    Rs = 50.6;

    fo = qo / (qw + qo);
    fw = 1 - fo;

    // ненужные данные   
    Rsw = 0;
    rho_w = 1000;
    mu_w = 1;

    //работа с жидкостью
    Liquid.q = qo + qw;
    Liquid.mu = mu_o * fo + mu_w * fw;
    Liquid.rho = rho_o * fo + rho_w * fw;
    Liquid.rho_sc = Liquid.rho;

    //работаем с газом 
    qg_ny = 283000.0 / 86400;
    Gas.mu = 0.000016;
    Gas.q = (qg_ny - qo_ny * Rs - qw_ny * Rsw) * Bg;
    Gas.rho = 94.19;
    Gas.rho_sc = Gas.rho;
    Angle = 90;

    //работа с взаимодействием фаз 
    PhaseInteract PhaseInteract;
    PhaseInteract.lgSurfaceTension = 0.00841;
    Result grad;

    // реализация
    FlowMapOrkizhevskiy myex = FlowMapOrkizhevskiy();
    grad = myex.calc(Liquid, Gas, PhaseInteract, D, 0.000018288, 90, 117.13 * 100000, 82);
    std::cout << grad.pressureGradient << "\n";

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
