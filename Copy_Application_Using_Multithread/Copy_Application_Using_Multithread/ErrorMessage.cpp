#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <strsafe.h>
#include "ErrorMessage.h"
#define ONEKB 1024
#define ONEMB ONEKB*1000
#define FILESIZE ONEMB*10
#define ASCIIVALUEFORA 65



void ErrorMessage(LPTSTR ErrorType)
{
	// Retrieve the system error message for the last-error code

	LPVOID cMessageBuffer = NULL;
	LPVOID cDisplayBuffer = NULL;
	DWORD dwErrorId = GetLastError();

	cDisplayBuffer = (LPVOID)LocalAlloc(LMEM_ZEROINIT, ONEKB * sizeof(char));
	if (cMessageBuffer == NULL)
	{
		ErrorMessage((LPTSTR)TEXT("Memory allocation for cMessage buffer"));
		return;
	}

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&cMessageBuffer,
		0, NULL);


	
	cDisplayBuffer=(LPVOID)LocalAlloc(LMEM_ZEROINIT, ((strlen((const char*)cMessageBuffer) + strlen((const char*)ErrorType)) + 40) * sizeof(char));
	if (cDisplayBuffer == NULL)
	{
		ErrorMessage((LPTSTR)TEXT("LocalAlloc for cDisplayBuffer"));
		return;
	}

	StringCchPrintf((LPTSTR)cDisplayBuffer,LocalSize(cDisplayBuffer),TEXT("%s failed with error %d: %s"),ErrorType, dwErrorId, (LPTSTR)cMessageBuffer);
	

	wprintf(L"%s\n",(LPCTSTR)cDisplayBuffer);

	LocalFree(cMessageBuffer);
	LocalFree(cDisplayBuffer);
	return;
}