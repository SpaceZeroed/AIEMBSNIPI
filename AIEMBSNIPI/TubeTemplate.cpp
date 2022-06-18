// TubeTemplate1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FlowMaps.h"
#include "FLowMapOrk.h"
#include <windows.h>
using namespace flowmaps;
bool KEY[256]; // id keyboard
void GetKEY() // обработка клавиатуры 
{
	int i = 0;
	while (i < 256)
	{
		if (GetAsyncKeyState(i)) KEY[i] = 1; else KEY[i] = 0;
		i++;
	}
}

void Map(int x, int y, vector<vector<int>> Array)
{
	
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	for (int i = 0; i < 1001; i++) {
		SetPixel(hDC, x + i, y, RGB(255, 255, 255));
	}
	for (int i = 0; i < 200; i++) {
		SetPixel(hDC, x, y + i, RGB(255, 255, 255));
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 1001; j++) {
			switch (Array[i][j])
			{
			case 0:
				SetPixel(hDC, x + j+1, y + i+1, RGB(0, 0, 255));
				break;
			case 1:
				SetPixel(hDC, x + j + 1, y + i + 1, RGB(0, 255, 0));
				break;
			case 2:
				SetPixel(hDC, x + j + 1, y + i + 1, RGB(255, 0, 0));
				break;
			case 3:
				SetPixel(hDC, x + j + 1, y + i + 1, RGB(255, 255, 255));
				break;
			case 4:
				SetPixel(hDC, x + j + 1, y + i + 1, RGB(0, 255, 255)); // на случай ошибки
				break;
			}
			
		}
	}

	ReleaseDC(hWnd, hDC);

}

int main()
{

	std::cout << "Hello World! I believe that our proect wil work\n";
	double Bo, Bg, Rs, D, qo, qw_ny, qo_ny, qg_ny, fo, fw, Bw, qw, mu_o, mu_w, rho_o, rho_w, Rsw, Roughness, Angle, PInflow, TInflow;
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
	flow.setLiquid(qo_ny, qw_ny, Bo, Bw, mu_o, mu_w, rho_o, rho_w);
	flow.setGas(qg_ny, qo_ny, qw_ny, mu_g, Rs, Rsw, Bg, rho_g);
	flow.setPhaseInteract(SurfaceTension);


	// реализация

	grad = flow.calc(D, 0.000018288, 90, 117.13 * 100000, 82);
	std::cout << grad.pressureGradient << "\n";

	int length;

	cout << "write the length of tube ";
	cin >> length;

	double izm = flow.MethodMarch(length, D, 0.000018288, 90, 117.13 * 100000, 82);

	Sleep(1500);
	system("cls");
	cin.ignore();
	vector<vector<int>> Array;
	Array = flow.fillMap(D, 0.000018288, 90, 117.13 * 100000, 82);
	HWND hwd = GetConsoleWindow();
	HDC hdc = GetDC(hwd);
	SelectObject(hdc, GetStockObject(WHITE_PEN));
	bool BoolKe = true; // for while
	cout << "To recreate press Enter, to quit press q"; // Заготовка для текста
	while (BoolKe)
	{

		GetKEY();
		if (KEY[13]) // enter
		{
			Map(50, 200, Array);
		}
		if (KEY[81]) // q
		{
			BoolKe = false;
		}
		Sleep(500);
	}
	//Map(50, 200, Array);
	// для рисования 
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
