#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	TCHAR str[2];

	__try
	{
		_tcscpy(str, _T("ABCDEFGHIJK"));
		_tprintf(_T("%s \n"), str);
	}
	__finally
	{
		_tprintf(_T("__finally block \n"));
	}


	return 0;
}