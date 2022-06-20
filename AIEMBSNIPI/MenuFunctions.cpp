#include <windows.h>
#include <conio.h>
#include <iostream>
#include <clocale>
#include "MenuDraw.h"
#include "MenuFunctions.h"
using namespace std;
void File() {
	cout << "Вы выбрали меню 'Файл'\n";
}
//Функция меню <Действие>. Заполняется кодом пользователя
void Do() {
	long val = 0;
	cout << "Введите целое число: ";
	cout << "Квадрат " << val << " равен " << val * val << "\n";
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
}