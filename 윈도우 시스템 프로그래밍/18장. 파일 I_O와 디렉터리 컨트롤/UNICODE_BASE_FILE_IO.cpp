#include "stdafx.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100

int file_CreateWrite();
int file_Open_Read();

int _tmain()
{
	file_CreateWrite();
	file_Open_Read();
	return 0;
}

int file_CreateWrite()
{
	TCHAR fileName[] = _T("data.txt");
	TCHAR fileData[] = _T("Just test string~");

	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_WRITE,
		0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File createion fault! \n"));
		return -1;
	}

	DWORD numOfByteWritten = 0;

	WriteFile(
		hFile, fileData, sizeof(fileData), &numOfByteWritten, NULL
	);

	_tprintf(_T("Written data size : %u \n"), numOfByteWritten);

	CloseHandle(hFile);

	return 0;
}

int file_Open_Read()
{
	TCHAR fileName[] = _T("data.txt");
	TCHAR fileData[STRING_LEN];

	HANDLE hFile = CreateFile(
		fileName, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File open fault! \n"));
		return -1;
	}

	DWORD numOfByteRead = 0;

	ReadFile(
		hFile, fileData, sizeof(fileData), &numOfByteRead, NULL
	);
	fileData[numOfByteRead / sizeof(TCHAR)] = 0;

	_tprintf(_T("Read data size : %u \n"), numOfByteRead);
	_tprintf(_T("Read string : %s \n"), fileData);

	CloseHandle(hFile);

	return 0;
}