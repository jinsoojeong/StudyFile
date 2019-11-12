// namedpipe_asynch_server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUF_SIZE 1024

int CommToClient(HANDLE);

int _tmain()
{
	LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
	HANDLE hPipe;

	while (true)
	{
		hPipe = CreateNamedPipe (
			pipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUF_SIZE / 2, BUF_SIZE / 2,
			2000, NULL);

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("CreateNamedPipe failed"));
			return -1;
		}

		BOOL isSucess;

		isSucess = ConnectNamedPipe(hPipe, NULL) ? TRUE :
			(GetLastError() == ERROR_PIPE_CONNECTED);
		if (isSucess)
			CommToClient(hPipe);
		else
			CloseHandle(hPipe);
	}

    return 1;
}

int CommToClient(HANDLE hPipe)
{
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];
	BOOL isSuccess;
	DWORD fileNameSize;

	isSuccess = ReadFile(
		hPipe, fileName, MAX_PATH * sizeof(TCHAR),
		&fileNameSize, NULL);

	if (!isSuccess || fileNameSize == 0)
	{
		_tprintf(_T("Pipe read message error!"));
		return -1;
	}

	FILE *filePtr = _tfopen(fileName, _T("r"));

	if (filePtr == nullptr)
	{
		_tprintf(_T("File open fault! \n"));
		return -1;
	}

	OVERLAPPED overlappedInst;

	memset(&overlappedInst, 0, sizeof(overlappedInst));
	overlappedInst.hEvent = CreateEvent(
		NULL, TRUE, TRUE, NULL);

	DWORD bytesWritten = 0, bytesRead = 0;
	DWORD bytesWrite = 0, bytesTransfer = 0;

	while (!feof(filePtr))
	{
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
		bytesWrite = bytesRead;

		isSuccess = WriteFile(hPipe, dataBuf, bytesWrite,
			&bytesWritten, &overlappedInst);

		if (!isSuccess && GetLastError() != ERROR_IO_PENDING)
		{
			_tprintf(_T("Pipe write message error! \n"));
			break;
		}

		//

		//
		
		WaitForSingleObject(overlappedInst.hEvent, INFINITE);
		GetOverlappedResult(
			hPipe, &overlappedInst, &bytesTransfer, FALSE);

		_tprintf(_T("Transferred data size : %u \n"), bytesTransfer);
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 1;



}

