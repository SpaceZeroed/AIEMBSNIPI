// ��� ������� ��� ����������� �������� ����
#include <windows.h>
#include <conio.h>
#include <iostream>
#include "MenuFunctions.h"
#include "MenuDraw.h"
using namespace std;
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
typedef void(*FUN) (void); //��������� �� ������� void f(void)
//��� ����� ��������� ������ ����
typedef struct { //��������� ��� �������� ����
	int x, y; //������� � ������ �������
	char* str; //������������ ������ ����
	FUN f; //�������, ����������� � ������ ����
} ITEM;
// ���������� ����������, ������������ � �������� ����
HANDLE hStdOut; //���������� ����������� ����
CONSOLE_SCREEN_BUFFER_INFO csbInfo;//���������� � ���������� ����
SMALL_RECT consolRect; //���������� ����� ����������� ����
COORD curspos = { 0,1 }; //���������� ���������� �������
WORD wo�kWindowAttributes = 158;//�������� ������� �������
WORD inactiveItemAttributes = 31; //�������� ����� �����������
// ������ ����
WORD activeItemAttributes = 160; // �������� ����� ���������
// ������ ����
// ���������� �������� ����
enum menuitems { MNUFILE, MNUDO, MNUCLEAR, MNUEXIT };
extern const int numMenu = 4; //���������� ������� ����
char temp1[10] ="Simple";
char temp2[10] = "Marsh";
char temp3[10] = "Map";
char temp4[10] = "Exit";
ITEM menu[numMenu] = { //��������� (x,y), ���������,
// ��������� �� �������
{ 1, 0, temp1, File},
{ 11, 0, temp2, Do },
{ 21, 0, temp3, Clear },
{ 31, 0, temp4, Exit }
};
// ����� ����� ���������� " ���� ", " �������� "," �������� ",
// " ����� " ������ ���� ��������� � ������������ � ��
// X - ������������ � ������� menu[]
void DrawMenu() { //���������� ����
	menuitems sel = MNUFILE; // ����� �������� ������ ����
	SetConsoleTextAttribute(hStdOut, inactiveItemAttributes);
	string s(80, ' '); cout << s.c_str(); //������ ��� ������ ����
	for (int i = 0; i < numMenu; i++) { //���������� ���������
	//������� ����
		gotoxy(menu[i].x, menu[i].y);
		cout << menu[i].str;
	}
		itemMenu(sel, true); // �������� ����� ����
	fflush(stdin); //�������� ����� ����������
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
					itemMenu(sel, false); // ������� ���������� ����� ����
					sel = (menuitems)(sel + 1);
					itemMenu(sel, true); // �������� �������� ����� ����
				}
				else {
					itemMenu(sel, false); // ������� ���������� ����� ����
					sel = MNUFILE; // ��������� �����
					itemMenu(sel, true); // �������� �������� ����� ����
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
					sel = MNUEXIT; // ��������� �����
					itemMenu(sel, true);
				}
				showCursor(false);
				break;
			case KEY_ENTER:
				gotoxy(curspos.X, curspos.Y); //���������� ������ �� ������
				// ���� � ������� �������
				SetConsoleTextAttribute(hStdOut,
					wo�kWindowAttributes); // ���������� ����
					// ������� ���������
				showCursor(true);
				switch (sel)
				{
				case MNUFILE:
					File();
					getCursorPosition(); // ��������� ��������� �������
					break;
				case MNUDO:
					Do();
					getCursorPosition(); // ��������� ��������� �������
					break;
					
				case MNUCLEAR:
					Clear();
					curspos = { 0, 1 }; // ����� ������� ������
					//� ����� ������� ���� �������
					break;
				case MNUEXIT:
					int resp;
					cout << "�� �������, ��� ������ ����� �� ��������� ? (y / n) ? ";
						resp = getchar();
					if (resp == 'y' || resp == 'Y')
					{
						gotoxy(0, 0); cls(1); exit(0);
					}
					getCursorPosition(); // ��������� ��������� �������,
					// ���� �������� �����
					break;
				}
				fflush(stdin); //�������� ����� ����������
				gotoxy(menu[sel].x, menu[sel].y); // ������ �
				// ������� ����� ����
				showCursor(false);
				break;
			case 120: // ����� �� ������� x
			case 88: // ����� �� ������� X
			case 27: // ����� �� ������� ESC
				gotoxy(0, 0);
				cls(1);
				exit(0); //���������� ���������
			}
		}
	}
}
// ��������� ������ � ����� x,y
void gotoxy(int x, int y)
{
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(hStdOut, cursorPos);
	//SetConsoleCursorPosition(hStdOut, {x,y});
}
// ������ �������� ��������� ���������� ������� � ����������
// ���������� curspos
void getCursorPosition(void)
{
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	curspos = csbInfo.dwCursorPosition;// ��������� �������
}
// ������� �������� ������� �������. ���� it==0, �� ������� ��
// ������ ��������� �� ������� ����, ����� ������� � ������
// �������� ���� �������
void cls(int it)
{
	int i;
	string s(80, ' ');
	SetConsoleTextAttribute(hStdOut, wo�kWindowAttributes);
	if (it == 0) gotoxy(0, consolRect.Top + 1);
	else gotoxy(0, consolRect.Top);
	for (i = consolRect.Top; i < curspos.Y + 1; i++) // ������� ��
	// ������ ������ �� ������ � ��������
		cout << s.c_str(); // ������ ��� ������ ����
	gotoxy(0, 0);
}
// �������� ����� ���� � ������� sel
void itemMenu(int sel, bool activate)
{
	WORD itemAttributes;
	if (activate) itemAttributes = activeItemAttributes;
	else itemAttributes = inactiveItemAttributes;
	gotoxy(menu[sel].x, menu[sel].y);
	SetConsoleTextAttribute(hStdOut, itemAttributes);
	cout << menu[sel].str;
}
// ������/�������� ��������� ������ � �������
void showCursor(bool visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(hStdOut, &ccInfo);
}