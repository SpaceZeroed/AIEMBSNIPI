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

	double Bo, //объемный коэффициент нефти
		Bg, //объемный коэффициент газа
		Rs, //
		D, //диаметр трубы
		qw_ny, //объемный расход воды
		qo_ny, //объемный расход нефти
		qg_ny, // объемный расход газа
		Bw, //объемный коэффициент воды
		mu_o, //вязкость нефти
		mu_w, //вязкость воды
		rho_o, //
		rho_w, //
		Rsw, //
		Roughness, // 
		Angle, // 
		PInflow, //
		TInflow; //
	double Bo, Bg, Rs, D, qw_ny, qo_ny, qg_ny, Bw, mu_o, mu_w, rho_o, rho_w, Rsw, Roughness, Angle, PInflow, TInflow;
	//  начальные условия  
	FlowMapOrkizhevskiy flow;
	Result grad;

	qo_ny = 1590.0 / 86400;
	qg_ny = 283000.0 / 86400;
	qw_ny = 0;
	D = 0.1524;
	Bo = 1.197;
	Bg = 0.0091;
	Bw = 0.0;

	//// вязкость нефти
	mu_o = 0.00097;
	Roughness = 0.000018288; // !!!!
	PInflow = 117.13 * 100000;
	TInflow = 82;
	rho_o = 762.64;
	Rs = 50.6;
	//// ненужные данные   
	Rsw = 0;
	rho_w = 1000;
	mu_w = 1;
	double mu_g = 0.000016;
	double rho_g = 94.19;
	Angle = 90;
	////работа с взаимодействием фаз 
	double SurfaceTension = 0.0084;

	double qo = qo_ny * Bo;
	double qw = qw_ny * Bw;

	double fo = qo / (qw + qo);
	double fw = 1 - fo;

	mainFase = DefineMainFase(mu_o, fw);

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
