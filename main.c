#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "resource.h"
#include <math.h>
#include <CommCtrl.h>
#include <stdlib.h>
double pr1(double a, double b, int n, double(*pf1)(double));
double f1(double x);
double f2(double t, double k);
double pr2(double a, double b, int n, double(*pf2)(double), double k);
double B_k(double k);
double f3(double t, double k);
double UT(int k, double t);
double pr3(double a, double b, int n, double(*pf3)(double), double k);
double fun(double t);
double A_k(double k);
double U_0(); // U0 первый интеграл
void K();
double Umk(int n);
int i = 0;
double T = 1;
double A[50], B[50], UMK[50], U0, w = 2 * M_PI;
double f(double t);
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam);
BOOL APIENTRY ExampleDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY InfoDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void PaintGraph(HDC hdc, int left, int top, int right, int bottom);
void PaintGraph2(HDC hdc, int left, int top, int right, int bottom);
void PaintGrid(HDC hdc, int left, int top, int right, int bottom);
double f(double x);

double xmin = 0, xmax = 6, xstep = 1;
double ymin = -1., ymax = 1.5, ystep = 0.5;
int xshow = BST_CHECKED, yshow = BST_CHECKED, mstep = 4;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	K();
	WNDCLASS wc;
	HWND hWnd;
	HWND hInst;
	HWND hListBox;
	MSG msg;

	// Создаем класс окна
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowsProc;
	wc.hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "MainMenu";
	wc.lpszClassName = "SimpleApplication";

	// Регистрируем класс окна.
	RegisterClass(&wc);

	// Создаем окно
	hWnd = CreateWindow("SimpleApplication", // Класс окна
		"Окно", // Заголовок
		WS_OVERLAPPEDWINDOW, // Стиль
		CW_USEDEFAULT, // Координата X верхнего угла
		CW_USEDEFAULT, // Координата Y верхнего угла
		CW_USEDEFAULT, // Ширина окна
		CW_USEDEFAULT, // Высота окна
		NULL, // Дескриптор родительского окна
		NULL, // Дескрипрот меню
		hInstance, // Дескриптор экземпляра окна
		NULL); // lParam
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT r;
	static HPEN hpenGrid;
	static HPEN hpenGraph;
	static HPEN hpenGraph2;
	static CHOOSECOLOR cc;
	static COLORREF CustColors[16];
	static COLORREF CustColors2[16];
	static bShow = MF_CHECKED;
	static bShow2 = MF_CHECKED;
	static OPENFILENAME ofn;
	static char strFile[MAX_PATH];

	switch (uMsg)
	{
	case WM_CREATE:
		hpenGrid = CreatePen(PS_DOT, 1, RGB(170, 170, 170));
		hpenGraph = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		hpenGraph2 = CreatePen(PS_SOLID, 3, RGB(112, 26, 153));

		cc.lStructSize = sizeof(CHOOSECOLOR);
		cc.hwndOwner = hWnd;
		cc.lpCustColors = CustColors;
		cc.Flags = CC_RGBINIT;

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = strFile;
		ofn.nMaxFile = MAX_PATH;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		Rectangle(hdc, 50, 20, r.right - 20, r.bottom - 20);
		SelectObject(hdc, hpenGrid);
		PaintGrid(hdc, 50, 20, r.right - 20, r.bottom - 20);
		SelectObject(hdc, hpenGraph);
		if (bShow)
			PaintGraph(hdc, 50, 20, r.right - 20, r.bottom - 20);
		SelectObject(hdc, hpenGraph2);
		if (bShow2)
			PaintGraph2(hdc, 50, 20, r.right - 20, r.bottom - 20);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_COLOR1:
			if (ChooseColor(&cc))
			{
				DeleteObject(&hpenGraph);
				hpenGraph = CreatePen(PS_SOLID, 3, cc.rgbResult);
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
		case IDM_COLOR2:
			if (ChooseColor(&cc))
			{
				DeleteObject(&hpenGraph2);
				hpenGraph2 = CreatePen(PS_SOLID, 3, cc.rgbResult);
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
		case IDM_SHOW1:
			bShow = bShow == MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
			CheckMenuItem(GetMenu(hWnd), IDM_SHOW1, bShow);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case IDM_SHOW2:
			bShow2 = bShow2 == MF_CHECKED ? MF_UNCHECKED : MF_CHECKED;
			CheckMenuItem(GetMenu(hWnd), IDM_SHOW2, bShow2);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case IDM_OPEN:
			if (GetOpenFileName(&ofn))
			{
				MessageBox(hWnd, strFile, "Файл", MB_OK);
			}
			break;

		case IDM_ABOUT:
			MessageBox(hWnd, "Тестовая программа.\n\nv1.0 2019", "О программе", MB_OK);
			break;
		case IDM_SETUP:
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), "ExampleDialog", hWnd, ExampleDlgProc);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
			
		case IDM_LISTBOX:
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), "InfoDialog", hWnd, InfoDlgProc);
		}
		return 0;

	case WM_DESTROY:
		DeleteObject(hpenGrid);
		DeleteObject(hpenGraph);
		DeleteObject(hpenGraph2);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL APIENTRY InfoDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		int n; 
		char str[256];
		HWND hLb = GetDlgItem(hWnd, ID_LIST);
		for (n = 1; n < 50; n++)
		{
			sprintf(str, "%g", UMK[n-1]);
			SendMessage(hLb, LB_ADDSTRING, 0,str);
		}
		sprintf(str, "%g", U0);
		SetDlgItemText(hWnd, IDC_U0, str);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL APIENTRY ExampleDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char str[256];
	switch (uMsg)
	{
	case WM_INITDIALOG:
		sprintf(str, "%g", xmax);
		SetDlgItemText(hWnd, IDC_XMAX, str);
		sprintf(str, "%g", xmin);
		SetDlgItemText(hWnd, IDC_XMIN, str);
		sprintf(str, "%g", xstep);
		SetDlgItemText(hWnd, IDC_XSTEP, str);
		CheckDlgButton(hWnd, IDC_XSHOW, xshow);

		sprintf(str, "%g", ymax);
		SetDlgItemText(hWnd, IDC_YMAX, str);
		sprintf(str, "%g", ymin);
		SetDlgItemText(hWnd, IDC_YMIN, str);
		sprintf(str, "%g", ystep);
		SetDlgItemText(hWnd, IDC_YSTEP, str);
		CheckDlgButton(hWnd, IDC_YSHOW, yshow);

		sprintf(str, "%d", mstep);
		SetDlgItemText(hWnd, IDC_MSTEP, str);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			GetDlgItemText(hWnd, IDC_XMAX, str, 256);
			xmax = atof(str);
			GetDlgItemText(hWnd, IDC_XMIN, str, 100);
			xmin = atof(str);
			GetDlgItemText(hWnd, IDC_XSTEP, str, 100);
			xstep = atof(str);
			GetDlgItemText(hWnd, IDC_YMAX, str, 100);
			ymax = atof(str);
			GetDlgItemText(hWnd, IDC_YMIN, str, 100);
			ymin = atof(str);
			GetDlgItemText(hWnd, IDC_YSTEP, str, 100);
			ystep = atof(str);
			GetDlgItemText(hWnd, IDC_MSTEP, str, 100);
			mstep = atof(str);
			xshow = IsDlgButtonChecked(hWnd, IDC_XSHOW);
			yshow = IsDlgButtonChecked(hWnd, IDC_YSHOW);

		case IDCANCEL:
			EndDialog(hWnd, wParam);
			return TRUE;
		}

	}
	return FALSE;
}


void PaintGrid(HDC hdc, int left, int top, int right, int bottom)
{
	double p, pstep, mark;
	char str[100];
	pstep = (top - bottom) / (ymax - ymin)*ystep;
	SetTextAlign(hdc, TA_RIGHT | TA_BASELINE);
	for (p = bottom, mark = ymin; p >= top - 1; p += pstep, mark += ystep)
	{
		if (yshow == BST_CHECKED)
		{
			MoveToEx(hdc, left, p, NULL);
			LineTo(hdc, right, p);
		}
		TextOut(hdc, left - 3, p + 5, str, sprintf(str, "%.1f", mark));

	}
	pstep = (right - left) / (xmax - xmin)*xstep;
	SetTextAlign(hdc, TA_CENTER | TA_TOP);
	for (p = left, mark = xmin; p <= right + 1; p += pstep, mark += xstep)
	{
		if (xshow == BST_CHECKED)
		{
			MoveToEx(hdc, p, top, NULL);
			LineTo(hdc, p, bottom);
		}
		TextOut(hdc, p, bottom + 5, str, sprintf(str, "%.1f", mark));

	}
}
void PaintGraph(HDC hdc, int left, int top, int right, int bottom)
{
	int px;
	double x, xs = (xmax - xmin) / (right - left);
	double ky = (top - bottom) / (ymax - ymin);
	MoveToEx(hdc, left, (fun(xmin) - ymin) * ky + bottom, NULL);
	for (px = left + 1, x = xmin + xs; px <= right; px++, x += xs)
		LineTo(hdc, px, (fun(x) - ymin) * ky + bottom);
}void PaintGraph2(HDC hdc, int left, int top, int right, int bottom)
{
	int px;
	double x, xs = (xmax - xmin) / (right - left);
	double ky = (top - bottom) / (ymax - ymin);
	MoveToEx(hdc, left, (UT(mstep, xmin) - ymin) * ky + bottom, NULL);
	for (px = left + 1, x = xmin + xs; px <= right; px++, x += xs)
		LineTo(hdc, px, (UT(mstep, x) - ymin) * ky + bottom);
}

double fun(double t)
{
	double t2;

	t2 = t;
	if (t < 0)
		while (t2 < 0)
		{
			t2 += T;
		}
	else
		if (t > T)
			while (t2 > T)
			{
				t2 -= T;
			}
	if (t2 >= T / 2 && t2 < T)
	{
		return 0;
	}
	else
	{
		return exp(-(8 * t2) / (5 * T));

	}
}
double U_0()
{
	double a = -0.5, b = 0.5;
	double s1, s2 = 1e10, eps = 1e-6;
	int n = 100;
	do
	{
		s1 = s2;
		n *= 2;
		s2 = pr1(a, b, n, f1);

	} while (fabs(s1 - s2) > eps);
	return s2;
}
double f1(double t)
{
	return 2 / T * (fun(t));
}
double pr1(double a, double b, int n, double(*pf1)(double))
{
	double h = (b - a) / n, x, S = 0;
	for (x = a + h / 2; x < b; x += h)
		S += f1(x);
	return S * h;
}
double A_k(double k)
{
	double a = -0.5, b = 0.5;
	double s1, s2 = 1e10, eps = 1e-6;
	int n = 100;
	do
	{
		s1 = s2;
		n *= 2;
		s2 = pr2(a, b, n, f2, k);

	} while (fabs(s1 - s2) > eps);
	return s2;
}
double f2(double t, double k)
{
	return (2 / T) * (fun(t))*cos((w*k / T * t));
}
double pr2(double a, double b, int n, double(*pf2)(double), double k)
{
	double h = (b - a) / n, x, S = 0;
	for (x = a + h / 2; x < b; x += h)
		S += f2(x, k);
	return S * h;
}
double B_k(double k)
{
	double a = -0.5, b = 0.5;
	double s1, s2 = 1e10, eps = 1e-6;
	int n = 100;
	do
	{
		s1 = s2;
		n *= 2;
		s2 = pr3(a, b, n, f3, k);
	} while (fabs(s1 - s2) > eps);
	return s2;
}
double f3(double t, double k)
{
	return (2 / T) * (fun(t))*sin(w*k* t);
}
double pr3(double a, double b, int n, double(*pf3)(double), double k)
{
	double h = (b - a) / n, x, S = 0;
	for (x = a + h / 2; x < b; x += h)
		S += f3(x, k);
	return S * h;
}


double UT(int k, double t)
{
	double l = 0;
	int m = 1;
	for (m = 1; m <= k; m++)
		l += (A[m - 1] * cos((w*m*t)) + B[m - 1] * sin((w*m*t)));// мне кажется что это будет более правельно, ведь в ф-ии К там запись массива начинается с 0
	return l + U0 / 2;

}
void K()
{
	int m = 50;
	int n;
	for (n = 0; n <= m; n++)
	{
		A[n] = A_k(n + 1);
		B[n] = B_k(n + 1);
		UMK[n] = Umk(n);
	}
	U0 = U_0();

}
double Umk(int n)
{
	return sqrt(A[n] * A[n] + B[n] * B[n]);
}
/*
BOOL APIENTRY InfoDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char str[256];
	int i;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		for (i = 0; i < 51; i++)
		{
			sprintf(str, "%d     %5.5f   ", i, UMK[i]);
			SendDlgItemMessage(hwnd, IDC_LISTVIEW, LB_ADDSTRING, 0, (LPARAM)str);

		}


		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, wParam);
			return TRUE;

		}
	}
	return FALSE;
}
