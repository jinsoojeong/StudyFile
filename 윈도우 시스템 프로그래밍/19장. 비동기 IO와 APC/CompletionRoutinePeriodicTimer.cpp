// CompletionRoutinePeriodicTimer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#define _WIN32_WINNT0x0400

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

VOID CALLBACK TimerAPCProc(LPVOID, DWORD, DWORD);

int _tmain()
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000000;

	hTimer =
		CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));

	if (!hTimer)
	{
		_tprintf(_T("CreateWaitableTimer failed (%d) \n"), GetLastError());
		return 1;
	}

	_tprintf(_T("Waiting for 10 seconds... \n"));

	SetWaitableTimer(
		hTimer, &liDueTime, 5000,
		TimerAPCProc, _T("Timer was signaled. \n"), FALSE);

	while (1)
		SleepEx(INFINITE, TRUE);

	return 0;
}

VOID CALLBACK TimerAPCProc(
	LPVOID lpArg, DWORD timerLowVal, DWORD timerHighVal)
{
	_tprintf(_T("%s"), (TCHAR *) lpArg);
	MessageBeep(MB_ICONEXCLAMATION);
}

