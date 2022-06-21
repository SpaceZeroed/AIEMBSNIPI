// Код функций для управлением строчным меню
#include <windows.h>
#include <conio.h>
#include <iostream>
#include "MenuFunctions.h"
#include "MenuDraw.h"
#include "FlowMaps.h"
#include "FLowMapOrk.h"

using namespace std;
using namespace flowmaps;

#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
typedef void(*FUN) (void); //Указатель на функцию void f(void)
//они будут выполнять пункты меню
typedef struct { //Структура для элемента меню
	int x, y; //Столбец и строка консоли
	 const char* str; //Наименование пункта меню
	FUN f; //Функция, привязанная к пункту меню
} ITEM;
// Глобальные переменные, используемые в функциях меню
HANDLE hStdOut; //дескриптор консольного окна
CONSOLE_SCREEN_BUFFER_INFO csbInfo;//информация о консольном окне
SMALL_RECT consolRect; //координату углов консольного окна
COORD curspos = { 0,1 }; //координаты текстового курсора
WORD woкkWindowAttributes = 158;//атрибуты рабочей области
WORD inactiveItemAttributes = 31; //атрибуты цвета неактивного
// пункта меню
WORD activeItemAttributes = 160; // атрибуты цвета активного
// пункта меню
// Изменяемые элементы меню
enum menuitems { MNUGRAD,MNUMARSH, MNUMAP, MNUCLEAR, MNUEXIT };
extern const int numMenu = 5; //количество пунктов меню
ITEM menu[numMenu] = { //положение (x,y), заголовок,
// указатель на функцию
{ 1, 0, "Gradient", PressureGrad},
{11,0, " Marsh   ", Marsh},
{ 21, 0, "  Map    ", RegimeMap},
{ 31, 0, "  Clear    ", Clear},
{ 41, 0, "  Exit   ", Exit}
};
// Длина строк заголовков " Файл ", " Действие "," Очистить ",
// " Выход " должна быть подобрана в соответствии с их
// X - координатами в массиве menu[]
void DrawMenu(FlowMapOrkizhevskiy flow) { //Управление меню
	menuitems sel = MNUGRAD; // Номер текущего пункта меню
	SetConsoleTextAttribute(hStdOut, inactiveItemAttributes);
	string s(80, ' '); cout << s.c_str(); //залить фон строки меню
	for (int i = 0; i < numMenu; i++) { //Напечатать заголовки
	//пунктов меню
		gotoxy(menu[i].x, menu[i].y);
		cout << menu[i].str;
	}
		itemMenu(sel, true); // выделить пункт меню
	fflush(stdin); //очистить буфер клавиатуры
	int iKey = 67;
	while (1)
	{
		if (_kbhit())
		{
			iKey = _getch();
			switch (iKey)
			{
			case KEY_ARROW_RIGHT:
				if (sel < numMenu - 1) {
					itemMenu(sel, false); // сделать неактивным пункт меню
					sel = (menuitems)(sel + 1);
					itemMenu(sel, true); // выделить активный пункт меню
				}
				else {
					itemMenu(sel, false); // сделать неактивным пункт меню
					sel = MNUGRAD; // прокрутка влево
					itemMenu(sel, true); // выделить активный пункт меню
				}
				showCursor(false);
				break;
			case KEY_ARROW_LEFT:
				if (sel > 0) {
					itemMenu(sel, false);
					sel = (menuitems)(sel - 1);
					itemMenu(sel, true);
				}
				else {
					itemMenu(sel, false);
					sel = MNUEXIT; // прокрутка влево
					itemMenu(sel, true);
				}
				showCursor(false);
				break;
			case KEY_ENTER:
				gotoxy(curspos.X, curspos.Y); //возвращаем курсор из строки
				// меню в прежнюю позицию
				SetConsoleTextAttribute(hStdOut,
					woкkWindowAttributes); // Установить цвет
					// рабочих сообщений
				showCursor(true);
				switch (sel)
				{
				case MNUGRAD:
					PressureGrad(flow);
					
					getCursorPosition(); // запомнить положение курсора
					break;
				case MNUMARSH:
					
					Marsh(flow);
					getCursorPosition();// запомнить положение курсора
					break;
				case MNUMAP:
					
					RegimeMap(flow);
					getCursorPosition(); // запомнить положение курсора
					break;
					
				case MNUCLEAR:
					Clear();
					curspos = { 0, 1 }; // после очистки курсор
					//в левый верхний угол консоли
					break;
				case MNUEXIT:
					int resp;
					cout << "Вы уверены, что хотите выйти из программы ? (y / n) ? ";
						resp = getchar();
					if (resp == 'y' || resp == 'Y')
					{
						gotoxy(0, 0); cls(1); exit(0);
					}
					getCursorPosition(); // запомнить положение курсора,
					// если отменили выход
					break;
				}
				fflush(stdin); //очистить буфер клавиатуры
				gotoxy(menu[sel].x, menu[sel].y); // курсор в
				// текущий пункт меню
				showCursor(false);
				break;
			case 120: // выход по клавише x
			case 88: // выход по клавише X
			case 27: // выход по клавише ESC
				gotoxy(0, 0);
				cls(1);
				exit(0); //завершение программы
			}
		}
	}
}
// Текстовый курсор в точку x,y
void gotoxy(int x, int y)
{
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(hStdOut, cursorPos);
	//SetConsoleCursorPosition(hStdOut, {x,y});
}
// запись текущего положения текстового курсора в глобальную
// переменную curspos
void getCursorPosition(void)
{
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	curspos = csbInfo.dwCursorPosition;// положение курсора
}
// очистка тестовой области консоли. Если it==0, то очистка со
// строки следующей за строкой меню, иначе очистка с левого
// верхнего угла консоли
void cls(int it)
{
	int i;
	string s(80, ' ');
	SetConsoleTextAttribute(hStdOut, woкkWindowAttributes);
	if (it == 0) gotoxy(0, consolRect.Top + 1);
	else gotoxy(0, consolRect.Top);
	for (i = consolRect.Top; i < consolRect.Bottom; i++) // очистка от
	// первой строки до строки с курсором
		cout << s.c_str(); // залить фон строки меню
	gotoxy(0, 0);
}
// выделить пункт меню с номером sel
void itemMenu(int sel, bool activate)
{
	WORD itemAttributes;
	if (activate) itemAttributes = activeItemAttributes;
	else itemAttributes = inactiveItemAttributes;
	gotoxy(menu[sel].x, menu[sel].y);
	SetConsoleTextAttribute(hStdOut, itemAttributes);
	cout << menu[sel].str;
}
// скрыть/показать текстовый курсор в консоли
void showCursor(bool visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(hStdOut, &ccInfo);
}