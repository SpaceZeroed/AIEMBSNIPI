#include <windows.h>
#include <conio.h>
#include <iostream>
#include <clocale>
#include "MenuDraw.h"
#include "MenuFunctions.h"
using namespace std;
void File() {
	cout << "�� ������� ���� '����'\n";
}
//������� ���� <��������>. ����������� ����� ������������
void Do() {
	long val = 0;
	cout << "������� ����� �����: ";
	cout << "������� " << val << " ����� " << val * val << "\n";
}
//������� ���� <�����> - ���������� ���������
void Exit() {
	int resp;
	cout << "�� �������, ��� ������ ����� �� ���������? (y/n)?";
	resp = getchar();
	if (resp == 'y' || resp == 'Y') { cls(1); exit(0); }
}
// ������� ���� <��������>
void Clear(void)
{
	cls();
}