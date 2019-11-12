#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int ReadStringAndWrite(void);

int main()
{
	int state = 0;

	while (true)
	{
		state = ReadStringAndWrite();

		if (state == -1)
		{
			_tprintf(_T("Some problem occurred \n"));
		}

		if (state == 0)
		{
			_tprintf(_T("Graceful exit \n"));
			break;
		}
	}
    return 0;
}

int ReadStringAndWrite()
{
	FILE *fPtr = NULL;
	TCHAR *strBufPtr = NULL;

	__try
	{
		fPtr = _tfopen(_T("string.dat"), _T("a+t"));

		if (fPtr == NULL)
			return -1;

		DWORD strLen = 0;
		
		_tprintf(_T("Input string Length(0 to exit : "));
		_tscanf(_T("%d"), &strLen);

		strBufPtr = (TCHAR*)malloc((strLen + 1) * sizeof(TCHAR));

		if (strBufPtr == NULL)
			return -1;

		_tprintf(_T("Input string : "));
		_tscanf(_T("%s"), &strBufPtr);

		_ftprintf(fPtr, _T("%s \n"), strBufPtr);
	}
	__finally
	{
		if (fPtr != NULL)
			fclose(fPtr);

		if (strBufPtr != NULL)
			free(strBufPtr);
	}

	return 1;
}

