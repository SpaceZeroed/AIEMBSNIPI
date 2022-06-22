#include <windows.h>
#include <conio.h>
#include <iostream>
#include <clocale>
#include "MenuDraw.h"
#include "MenuFunctions.h"
#include <vector>
#include "FlowMapOrk.h"

using namespace std;
using namespace flowmaps;

void PressureGrad( FlowMapOrkizhevskiy flow) {
	double grad;
	grad = flow.calc().pressureGradient;
	std::cout << grad << "\n";
}
//Функция меню <Действие>. Заполняется кодом пользователя
void RegimeMap(FlowMapOrkizhevskiy flow) {
	int x = 50, y = 200;	
	vector<vector<int>> Array;
	Array = flow.fillMap();
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	RECT rct = { x,y - 1,x + 1102,y + 203 };
	cout << "Blue is Bubble, Green is Cork, Red is Transitional, White is Emulsion";
	char str1[] = "N_Lv";
	TextOutA(hDC, 50, 170, str1, strlen(str1));
	char str2[] = "N_Gv";
	TextOutA(hDC, 1170, 380, str2, strlen(str2));
	char str3[] = "10";
	TextOutA(hDC, 70, 405, str3, strlen(str3));
	char str4[] = "100";
	TextOutA(hDC, 250, 405, str4, strlen(str4));
	char str5[] = "200";
	TextOutA(hDC, 350, 405, str5, strlen(str5));
	char str6[] = "1000";
	TextOutA(hDC, 1150, 405, str6, strlen(str6));
	char str7[] = "10";
	TextOutA(hDC, 30, 300, str7, strlen(str7));
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
void Marsh(FlowMapOrkizhevskiy flow) {
	double length;
	cout << "write the length of tube ";
	cin >> length;
	double izm = flow.MethodMarch(length);
}
//Функция меню <Выход> - завершение программы
void Exit(FlowMapOrkizhevskiy flow) {
	int resp;
	cout << "Вы уверены, что хотите выйти из программы? (y/n)?";
	resp = getchar();
	if (resp == 'y' || resp == 'Y') { cls(1); exit(0); }
}
// Функция меню <Очистить>
void Clear(FlowMapOrkizhevskiy flow)
{
	cls();
	HWND hWnd = GetConsoleWindow();
	HDC hDC = GetDC(hWnd);
	HBRUSH hBrush; //создаѐм объект-кисть
	hBrush = CreateSolidBrush(RGB(60, 119, 255)); //создаем сплошную кисть
	SelectObject(hDC, hBrush); //делаем кисть активной
	const RECT  rct = { 50,199,1310,444 };
	FillRect(hDC, &rct,hBrush);
	SelectObject(hDC, GetStockObject(WHITE_PEN));
}