// TubeTemplate1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "FlowMaps.h"
#include "FLowMapOrk.h"
#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace flowmaps;

 int main()
{


	 double Bo = 1.197, //Объемный коэффициент нефти
		 Bg = 0.0091, //Объемный коэффициент газа
		 Bw = 0.0, //Объемный коэффициент воды
		 Rs = 50.6, //Растворимость газа
		 Rsw = 0, //Растворимость газа
		 D = 0.1524, //Диаметр трубы, м
		 qw_ny = 0, //Начальный объемный расход воды, м^3/c
		 qo_ny = 1590.0 / 86400, //Начальный объемный расход нефти, м^3/c
		 qg_ny = 283000.0 / 86400, //Начальный объемный расход газа	, м^3/c	
		
		 mu_o = 0.00097, //Вязкость нефти, Па*с
		 mu_w = 1, //Вязкость воды, Па*с
		 mu_g = 0.000016, //Вязкость газа, Па*с
		 rho_o = 762.64, //Плотность нефти, кг/м^3
		 rho_w = 1000, //Плотность воды, кг/м^3
		 rho_g = 94.19,//Плотность газа, кг/м^3
		 Roughness = 0.000018288, // Шероховатость, м
		 Angle = 90, // Угол, градус
		 PInflow = 117.13 * 100000, //Давление на входе, Н/м^2
		 TInflow = 82, //Температура на входе, градус цельсия
		 SurfaceTension = 0.0084; //Поверхносное натяжение, Н/м

	//  начальные условия  
	FlowMapOrkizhevskiy flow;
	Result grad;

	double qo = qo_ny * Bo;//Объемный расход нефти, м^3/c
	double qw = qw_ny * Bw;//Объемный расход воды, м^3/c

	PhaseInfo Water;
	Water.q = qw;
	Water.mu = mu_w;
	Water.rho = rho_w;
	Water.rho_sc = rho_w;

	PhaseInfo Oil;
	Oil.q = qo;
	Oil.mu = mu_o;
	Oil.rho = rho_o;
	Oil.rho_sc = rho_o;

	PhaseInfo Gas;
	Gas.mu = mu_g;
	//Gas.q = (qg_ny - qo_ny * Rs - qw_ny * Rsw) * Bg;
	Gas.q = 0.518481;
	Gas.rho = rho_g;
	Gas.rho_sc = Gas.rho;

	PhaseInteract PhaseInteract;
	PhaseInteract.lgSurfaceTension = SurfaceTension;
	
	grad = flow.calc(
		Water,
		Oil,
		Gas,
		PhaseInteract,
		D,
		Roughness,
		Angle,
		PInflow,
		TInflow);

	cout << grad.pressureGradient<< "\n";
	double length;
	cin >> length;
	double otv = flow.MethodMarch(
		length,
		Water,
		Oil,
		Gas,
		PhaseInteract,
		D,
		Roughness,
		Angle,
		PInflow,
		TInflow);

	cout << otv;
	return 0;
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
