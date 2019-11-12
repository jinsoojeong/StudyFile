#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	int a, b;

	__try
	{
		_tprintf(_T("input divide string [ a / b ] : "));
		_tscanf_s(_T("%d / %d"), &a, &b);

		if (b == 0)
			return -1;
	}
	__finally
	{
		_tprintf(_T("__finally block \n"));
	}

	_tprintf(_T("result : %d \n"), a / b);

	return 0;
}