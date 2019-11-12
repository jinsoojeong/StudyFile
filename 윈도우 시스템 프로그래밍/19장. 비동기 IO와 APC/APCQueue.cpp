// APCQueue.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#define _WIN32_WINNT0x0400

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

VOID CALLBACK APCProc(ULONG_PTR);

int _tmain()
{
	HANDLE hThread = GetCurrentThread();

	QueueUserAPC(APCProc, hThread, (ULONG_PTR)1);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)2);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)3);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)4);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)5);

	Sleep(5000);
	SleepEx(INFINITE, TRUE);

    return 0;
}

VOID CALLBACK APCProc(ULONG_PTR dwParam)
{
	_tprintf(_T("Asynchronous procedure call num %u \n")
	, (DWORD)dwParam);
}

