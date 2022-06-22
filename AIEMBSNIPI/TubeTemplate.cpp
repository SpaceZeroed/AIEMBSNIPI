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
	system("mode con cols=160 lines=40"); // размер окна. Вроде 1600 на 400 пикселей
	SetConsoleTitle(L"Pressure calculation"); // заголовок окна
	setlocale(LC_CTYPE, "rus"); // вызов функции настройки национальных параметров
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	consolRect = csbInfo.srWindow; //координаты углов консоли
	SetConsoleTextAttribute(hStdOut, woкkWindowAttributes);
	system("CLS"); // установка атрибутов цвета рабочей области

	double Bo = 1.197, //объемный коэффициент нефти
		Bg = 0.0091, //объемный коэффициент газа
		Bw = 0.0, //объемный коэффициент воды
		Rs = 50.6, //растворимость газа
		Rsw = 0, //растворимость газа
		D = 0.1524, //диаметр трубы, м
		qw_ny = 0, //начальный объемный расход воды, м^3/c
		qo_ny = 1590.0 / 86400, //начальный объемный расход нефти, м^3/c
		qg_ny = 283000.0 / 86400, //начальный объемный расход газа	, м^3/c	
		mu_o = 0.00097, //вязкость нефти, Па*с
		mu_w = 1, //вязкость воды, Па*с
		mu_g = 0.000016, //вязкость газа, Па*с
		rho_o = 762.64, //плотность нефти, кг/м^3
		rho_w = 1000, //плотность воды, кг/м^3
		rho_g = 94.19,//плотность газа, кг/м^3
		Roughness = 0.000018288, // шероховатость, м
		Angle = 90, // угол, градус
		PInflow = 117.13 * 100000, //давление на входе, Н/м^2
		TInflow = 82, //температура на входе, градус цельсия
		SurfaceTension = 0.0084, //поверхносное натяжение, Н/м
	//  начальные условия  
	FlowMapOrkizhevskiy flow;
	Result grad;

	double qo = qo_ny * Bo;//объемный расход нефти, м^3/c
	double qw = qw_ny * Bw;//объемный расход воды, м^3/c

	double fo = qo / (qw + qo);//доля нефти
	double fw = 1 - fo;//доля воды


	PhaseInfo Liquid;
	Liquid.q = qo + qw;
	Liquid.mu = mu_o * fo + mu_w * fw;
	Liquid.rho = rho_o * fo + rho_w * fw;
	Liquid.rho_sc = Liquid.rho;

	PhaseInfo Gas;
	Gas.mu = mu_g;
	Gas.q = (qg_ny - qo_ny * Rs - qw_ny * Rsw) * Bg;
	Gas.rho = rho_g;
	Gas.rho_sc = Gas.rho;

	PhaseInteract phaseInteract;
	phaseInteract.lgSurfaceTension = SurfaceTension;
	
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
