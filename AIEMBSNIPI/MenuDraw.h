#pragma once
#include "FlowMaps.h"
#include "FLowMapOrk.h"

using namespace flowmaps;

// описание функций, которые управляют работой меню
void DrawMenu( FlowMapOrkizhevskiy flow); //создание меню
void gotoxy(int x, int y); // перевод курсора в точку x,y
void itemMenu(int sel, bool activate); // выделить пункт меню
void cls(int it = 0); // очистка консоли; при it==0 оставляем
// строку меню иначе очищаем всю консоль
void getCursorPosition(void); // запомнить положение курсора
//в глобальную переменную curspos
void showCursor(bool visible);// скрыть/показать курсор