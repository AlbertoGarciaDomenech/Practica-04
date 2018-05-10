#include "colores.h"
#include <windows.h>
#include <conio.h>

void setColor(int color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}