#include <windows.h>
#include <conio.h>
#include <iostream>
#include <clocale>
#include "MenuDraw.h"
#include "MenuFunctions.h"
#include <vector>
#include "FlowMapOrk.h"
using namespace std;
void PressureGrad() {
	cout << "Вы выбрали меню 'Файл'\n";
}
//Функция меню <Действие>. Заполняется кодом пользователя
void RegimeMap() {
	int x = 200, y = 200;
	double D = 0.1524;
	vector<vector<int>> Array;
	flowmaps::FlowMapOrkizhevskiy flow;
	Array = flow.fillMap(D, 0.000018288, 90, 117.13 * 100000, 82);
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	RECT rct = { x,y - 1,x + 1102,y + 203 };
	FrameRect(hDC, &rct, CreateSolidBrush(RGB(255, 0, 0)));
	for (int i = 0; i < 202; i++) {
		for (int j = 0; j < 1100; j++) {
			switch (Array[i][j])
			{
			case 0:
				SetPixel(hDC, x + j + 1, y - i + 201, RGB(0, 0, 255)); // Пузырьковый-синий
				break;
			case 1:
				SetPixel(hDC, x + j + 1, y - i + 201, RGB(0, 255, 0)); // Пробковый-зеленый
				break;
			case 2:
				SetPixel(hDC, x + j + 1, y - i + 201, RGB(255, 0, 0)); // Переходный- красный
				break;
			case 3:
				SetPixel(hDC, x + j + 1, y - i + 201, RGB(255, 255, 255)); // Эмульсионный - белый
				break;
			case 4:
				SetPixel(hDC, x + j + 1, y - i + 201, RGB(0, 255, 255)); // на случай ошибки
				break;
			}

		}
	}

	ReleaseDC(hWnd, hDC);
}
void Marsh() {
	double Bo, Bg, Rs, D, qw_ny, qo_ny, qg_ny, Bw, mu_o, mu_w, rho_o, rho_w, Rsw, Roughness, Angle, PInflow, TInflow;
	int length;
	qo_ny = 1590.0 / 86400;
	qg_ny = 283000.0 / 86400;
	qw_ny = 0;
	D = 0.1524;
	Bo = 1.197;
	Bg = 0.0091;
	Bw = 0.0;

	// вязкость нефти
	mu_o = 0.00097;
	Roughness = 0.000018288;
	PInflow = 117.13 * 100000;
	TInflow = 82;
	rho_o = 762.64;
	Rs = 50.6;
	// ненужные данные   
	Rsw = 0;
	rho_w = 1000;
	mu_w = 1;
	double mu_g = 0.000016;
	double rho_g = 94.19;
	Angle = 90;
	//работа с взаимодействием фаз 
	double SurfaceTension = 0.0084;

	//
	
	flowmaps::FlowMapOrkizhevskiy flow;
	flow.setLiquid(qo_ny, qw_ny, Bo, Bw, mu_o, mu_w, rho_o, rho_w);
	flow.setGas(qg_ny, qo_ny, qw_ny, mu_g, Rs, Rsw, Bg, rho_g);
	flow.setPhaseInteract(SurfaceTension);
	cout << "write the length of tube ";
	cin >> length;
	double izm = flow.MethodMarch(length, D, 0.000018288, 90, 117.13 * 100000, 82);
}
//Функция меню <Выход> - завершение программы
void Exit() {
	int resp;
	cout << "Вы уверены, что хотите выйти из программы? (y/n)?";
	resp = getchar();
	if (resp == 'y' || resp == 'Y') { cls(1); exit(0); }
}
// Функция меню <Очистить>
void Clear(void)
{
	cls();
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	HBRUSH hBrush; //создаѐм объект-кисть
	hBrush = CreateSolidBrush(RGB(60, 119, 255)); //создаем сплошную кисть
	SelectObject(hDC, hBrush); //делаем кисть активной
	const RECT  rct = { 200,199,1310,404 };
	FillRect(hDC, &rct,hBrush);
	SelectObject(hDC, GetStockObject(WHITE_PEN));
}