#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#include "ErrorMessage.h"
#include "WriteDataToFile.h"
#define ONEKB 1024
#define ONEMB ONEKB*1000
#define FILESIZE ONEMB*10
#define ASCIIVALUEFORA 65


BOOL WriteDataToFile(HANDLE hFileToWrite)				//Function for Writing data in File
{
	char* pcBuffer = NULL;
	BOOL bReturnValue = FALSE;
	DWORD dwBytesWritten = 0;
	pcBuffer = (char*)malloc(ONEMB * sizeof(char));
	if (pcBuffer == NULL)
	{
		std::cout << "Error:Not get enough storage" << std::endl;
		return FALSE;
	}



	for (int i = 0; i < 10; i++)
	{
		bReturnValue = FillBufferWithChar(pcBuffer, (char)(ASCIIVALUEFORA + i));
		if (bReturnValue == FALSE)
		{
			std::cout << "Buffer Writing Failed" << std::endl;
			free(pcBuffer);
			return FALSE;
		}

		bReturnValue = WriteFile(hFileToWrite, (LPCVOID)pcBuffer, (DWORD)(ONEMB * sizeof(char)), &dwBytesWritten, NULL);
		if (bReturnValue == FALSE)
		{
			std::cout << "File Writing Failed!!" << std::endl;
			ErrorMessage((LPTSTR)TEXT("Write"));
			free(pcBuffer);
			return FALSE;
		}
		if (dwBytesWritten != (ONEMB*sizeof(char)))
		{
			std::cout << "Error:Bytes Written != bytes want to write" << std::endl;
			free(pcBuffer);
			return FALSE;
		}
	}
	free(pcBuffer);
	//CloseHandle(hFile);
	return TRUE;
}



BOOL FillBufferWithChar(char* pcBuffer, char character)		//Function for Writing buffer
{
	int i;
	for (i = 0; i < ((ONEMB)*sizeof(char)); i++)
	{
		pcBuffer[i] = character;
	}
	return TRUE;
}