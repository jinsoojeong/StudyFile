#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define _CRT_SECURE_NO_WARNINGS

enum { DIV = 1, MUL, ADD, MIN, ELSE, EXIT };

DWORD ShowMenu();
void Divide(double, double);
void Multiple(double, double);
void Add(double, double);
void Min(double, double);

int _tmain(int argc, TCHAR* argv[]) 
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	
	TCHAR command[] = _T("calc.exe");
	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
	
	DWORD sel;
	double num1, num2;
	while (true) {
		sel = ShowMenu();
		if (sel == EXIT)
			return 0;
		
		if (sel != ELSE) {
			_fputts(_T("Input Num1 Num2 : "), stdout);
			_tscanf_s(_T("%lf %lf"), &num1, &num2);
		
		}
	
		switch (sel) {
			case DIV:
				Divide(num1, num2);
				break;
			case MUL:
				Multiple(num1, num2);
				break;
			case ADD:
				Add(num1, num2);
				break;
			case MIN:
				Min(num1, num2);
				break;
			case ELSE:
				ZeroMemory(&pi, sizeof(pi));
				CreateProcess(
				NULL, command, NULL, NULL,
				TRUE, 0, NULL, NULL, &si, &pi);
				break;
		}	
	}
	
	return 0;
}

DWORD ShowMenu() 
{
	DWORD sel;

	_fputts(_T("------Menu------\n"), stdout);
	_fputts(_T("num1: Divide\n"), stdout);
	_fputts(_T("num2: Multiple\n"), stdout);
	_fputts(_T("num3: Add\n"), stdout);
	_fputts(_T("num4: Minus\n"), stdout);
	_fputts(_T("num5: Any other operations\n"), stdout);
	_fputts(_T("num6: EXIT"), stdout);
	_fputts(_T("SELECION >> "), stdout);
	_tscanf_s(_T("%d"), &sel);

	return sel;
}

void Divide(double a, double b) {
	_tprintf(_T("%f/%f=%f \n\n"), a, b, a / b);
}

void Multiple(double a, double b) {
	_tprintf(_T("%f*%f=%f \n\n"), a, b, a*b);
}

void Add(double a, double b) {
	_tprintf(_T("%f+%f=%f \n\n"), a, b, a + b);
	
}

void Min(double a, double b) {
	_tprintf(_T("%f-%f=%f \n\n"), a, b, a - b);
}

