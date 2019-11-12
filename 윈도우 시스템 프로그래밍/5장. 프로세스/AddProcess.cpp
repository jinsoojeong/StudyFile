#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#ifdef _UNICODE
#define _ttoi _wtoi
#define _gettchar getwchar
#else
#define _ttoi atoi
#define _gettchar getchar
#endif

int _tmain(int argc, TCHAR* argv[])
{
	DWORD val1, val2;
	val1 = _ttoi(argv[1]);
	val2 = _ttoi(argv[2]);

	_tprintf(_T("%d + %d = %d \n"), val1, val2, val1 + val2);

	_gettchar();

	return 0;
}