// nonBlocking_fileIO.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

TCHAR strData1[] =
	_T("하나둘셋넷 둘둘셋넷~ \n");
TCHAR strData2[] =
	_T("onetwothreefour twotwothreefour~ \n");
TCHAR strData3[] =
	_T("1234 2234~ \n");

VOID WINAPI
	FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

OVERLAPPED overlappedFilePtr;

int _tmain()
{
	TCHAR fileName[] = _T("data.txt");

	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0,
		CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File creation fault! \n"));
		return -1;
	}

	OVERLAPPED overlappedInstOne;
	memset(&overlappedInstOne, 0, sizeof(OVERLAPPED));
	overlappedInstOne.hEvent = (HANDLE) "First I/O";
	WriteFileEx(hFile, strData1, sizeof(strData1),
		&overlappedInstOne, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData1) * sizeof(TCHAR);

	OVERLAPPED overlappedInstTwo;
	memset(&overlappedInstTwo, 0, sizeof(OVERLAPPED));
	overlappedInstTwo.hEvent = (HANDLE) "Second I/O";
	WriteFileEx(hFile, strData2, sizeof(strData2),
		&overlappedInstTwo, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData2) * sizeof(TCHAR);

	OVERLAPPED overlappedInstThree;
	memset(&overlappedInstThree, 0, sizeof(OVERLAPPED));
	overlappedInstThree.hEvent = (HANDLE) "Third I/O";
	WriteFileEx(hFile, strData3, sizeof(strData3),
		&overlappedInstThree, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData3) * sizeof(TCHAR);

	SleepEx(INFINITE, TRUE);
	CloseHandle(hFile);

    return 0;
}

VOID WINAPI FileIOCompletionRoutine(
	DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
	QueueUserAPC
	_tprintf(_T("FIle write result \n"));
	_tprintf(_T("Error code : %u \n"), errorCode);
	_tprintf(_T("Transfered bytes len : %u \n"), numOfBytesTransfered);
	_tprintf(_T("The other info : %s \n"), (TCHAR *)overlapped->hEvent);

}