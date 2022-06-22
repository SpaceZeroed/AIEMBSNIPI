// TubeTemplate1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "FlowMaps.h"
#include "FLowMapOrk.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include "MenuDraw.h"
#include "MenuFunctions.h"

using namespace flowmaps;

extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; //информация о
// консольном окне в структуре csbInfo
extern SMALL_RECT consolRect; //координаты углов консоли
extern WORD woкkWindowAttributes; //атрибуты рабочей
extern HANDLE hStdOut;
//области консоли
bool KEY[256]; // id keyboard
// обработка клавиатуры 
void GetKEY() 
{
	int i = 0;
	while (i < 256)
	{
		if (GetAsyncKeyState(i)) KEY[i] = 1; else KEY[i] = 0;
		i++;
	}
}
// Рисование карты режимов
void Map(int x, int y, vector<vector<int>> Array)
{

	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	RECT rct = { x,y-1,x+1102,y+203 };
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
	
	flow.setLiquid(qo_ny, qw_ny, Bo, Bw, mu_o, mu_w, rho_o, rho_w);
	flow.setGas(qg_ny, qo_ny, qw_ny, mu_g, Rs, Rsw, Bg, rho_g);
	flow.setPhaseInteract(SurfaceTension);
	flow.setArguments(D, 0.000018288, 90, 117.13 * 100000, 82);
	DrawMenu(flow);
	
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
