#pragma once
#include "FlowMaps.h"
#include "FLowMapOrk.h"

using namespace flowmaps;

// �������� �������, ������� ��������� ������� ����
void DrawMenu( FlowMapOrkizhevskiy flow); //�������� ����
void gotoxy(int x, int y); // ������� ������� � ����� x,y
void itemMenu(int sel, bool activate); // �������� ����� ����
void cls(int it = 0); // ������� �������; ��� it==0 ���������
// ������ ���� ����� ������� ��� �������
void getCursorPosition(void); // ��������� ��������� �������
//� ���������� ���������� curspos
void showCursor(bool visible);// ������/�������� ������