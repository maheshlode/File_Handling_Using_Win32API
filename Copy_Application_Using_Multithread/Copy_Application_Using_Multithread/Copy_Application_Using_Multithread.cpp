#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#include "ErrorMessage.h"
#include "WriteDataToFile.h"
#include "CreateNodeToInsertData.h"
#include <string>

#define ONEKB 1024
#define ONEMB ONEKB*1000
#define FILESIZE ONEMB*10
#define ASCIIVALUEFORA 65
#define NUMBER_THREADS 10
#define FILENAME "data.txt"

struct Node
{
	char* cDataBuffer = NULL;
	struct Node* pNextNode = NULL;
};


struct datareadinfo
{
	HANDLE hFileForDataReading = { 0 };
	DWORD dwStartOffset = 0;
};
typedef datareadinfo DATAREADINFO;

struct datawriteinfo
{
	DATANODE* pdnHeadNode=NULL;
	int iPositionOfDataNodeToRead=0;

};
typedef datawriteinfo DATAWRITEINFO;

//Function Declaration
DWORD WINAPI ThreadFunctionForReadingData(DATAREADINFO*);
DWORD WINAPI ThreadFunctionForWritingData(DATAWRITEINFO*);



int main()
{
	//Variable related to File
	HANDLE hFileCreate=INVALID_HANDLE_VALUE;
	BOOL bReturnValue=FALSE;
	char* pchDataReadBuffer = NULL;

	//Variable Related to Thread
	DWORD dwReturnValueOfWaitForObject = 0;
	int iThread = 0;
	DWORD dwThreadId[NUMBER_THREADS] = { 0 };
	HANDLE hThreadForReading[NUMBER_THREADS] = { 0 };
	DATAREADINFO* driDataReadInfoForThread[NUMBER_THREADS] = { 0 };
	DATAWRITEINFO* dwiDataWriteInfoForThread[NUMBER_THREADS] = { 0 };

	//Variable for FreeLinkedList
	DATANODE* dnHeadNodeOfLinkedList = NULL;



	pchDataReadBuffer = (char*)malloc(ONEMB * sizeof(char));
	if (pchDataReadBuffer == NULL)
	{
		std::cout << "Error:Not get enough storage" << std::endl;
		exit(EXIT_FAILURE);
	}
	memset(pchDataReadBuffer, 0, ONEMB * sizeof(char));


	//File Creating
	hFileCreate =CreateFileA(FILENAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileCreate == INVALID_HANDLE_VALUE)
	{
		std::cout << "File Creation failed!!" << std::endl;
		ErrorMessage((LPTSTR)TEXT("File Creation"));
		exit(EXIT_FAILURE);
	}


	//File Writing
	bReturnValue =WriteDataToFile(hFileCreate);
	if (bReturnValue == FALSE)
	{
		std::cout << "Writing data to File Failed!!" << std::endl;
		CloseHandle(hFileCreate);
		exit(EXIT_FAILURE);
	}

	std::cout << "Data Write Successfully in File!!\n" << std::endl;


	//Initialize data_read_info for Threads
	
	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		driDataReadInfoForThread[iThread] = (DATAREADINFO*)malloc(sizeof(DATAREADINFO));
		if (driDataReadInfoForThread[iThread] == NULL)
		{
			printf("\nMemory allocation Failed\n");
			return FALSE;
		}

		driDataReadInfoForThread[iThread]->hFileForDataReading = hFileCreate;
		driDataReadInfoForThread[iThread]->dwStartOffset = (iThread * ONEMB);

	}

	//Creating Threads for Reading data and Writing to Linked list

	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		hThreadForReading[iThread] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunctionForReadingData, (LPVOID)driDataReadInfoForThread[iThread], 0, &dwThreadId[iThread]);
		if (hThreadForReading[iThread] == NULL)
		{
			ErrorMessage((LPTSTR)TEXT("Thread Creation"));
			exit(EXIT_FAILURE);
		}
	}

	//Wait for Objects before closing handles
	dwReturnValueOfWaitForObject =WaitForMultipleObjects(NUMBER_THREADS, hThreadForReading, TRUE, INFINITE);
	if (dwReturnValueOfWaitForObject == WAIT_FAILED)
	{
		ErrorMessage((LPTSTR)TEXT("Wait Object"));
		return FALSE;
	}
	
	// Close Handle of All threads
	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		CloseHandle(hThreadForReading[iThread]);
		if (driDataReadInfoForThread[iThread] != NULL)
		{
			free(driDataReadInfoForThread[iThread]);
			driDataReadInfoForThread[iThread] = NULL;
		}
	}

	

	//Display, Sort  and Free Linked List
	dnHeadNodeOfLinkedList =DisplayDataInLinkedList();

	SortDataInLinkedList(dnHeadNodeOfLinkedList);

	printf("\nAfter Sorting Linked List\n");
	DisplayDataInLinkedList();


	
	CloseHandle(hFileCreate);
	free(pchDataReadBuffer);




	//Reading 1MB data from linked list and writing it in newly created file


	//Initialize data_write_info for thread
	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		dwiDataWriteInfoForThread[iThread] = (DATAWRITEINFO*)malloc(sizeof(DATAWRITEINFO));
		if (dwiDataWriteInfoForThread[iThread] == NULL)
		{
			printf("\nMemory allocation Failed\n");
			return FALSE;
		}

		dwiDataWriteInfoForThread[iThread]->pdnHeadNode = dnHeadNodeOfLinkedList;
		dwiDataWriteInfoForThread[iThread]->iPositionOfDataNodeToRead = iThread;

	}


	//Creating a Threads for Reading_Data from sorted linked_list and Writing into newly created File

	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		hThreadForReading[iThread] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunctionForWritingData, (LPVOID)dwiDataWriteInfoForThread[iThread], 0, &dwThreadId[iThread]);
		if (hThreadForReading[iThread] == NULL)
		{
			ErrorMessage((LPTSTR)TEXT("Thread Creation"));
			exit(EXIT_FAILURE);
		}
	}

	//Wait for Objects before closing handles
	dwReturnValueOfWaitForObject = WaitForMultipleObjects(NUMBER_THREADS, hThreadForReading, TRUE, INFINITE);
	if (dwReturnValueOfWaitForObject == WAIT_FAILED)
	{
		ErrorMessage((LPTSTR)TEXT("Wait Object"));
		return FALSE;
	}

	// Close Handle of All threads
	for (iThread = 0; iThread < NUMBER_THREADS; iThread++)
	{
		CloseHandle(hThreadForReading[iThread]);
		if (dwiDataWriteInfoForThread[iThread] != NULL)
		{
			free(dwiDataWriteInfoForThread[iThread]);
			dwiDataWriteInfoForThread[iThread] = NULL;
		}
	}

	printf("\nSorted data Successfully write in separate files by thread\n");

	bReturnValue = FreeLinkedList(dnHeadNodeOfLinkedList);
	if (bReturnValue == TRUE)
	{
		printf("\nLinked List Free Successfully!!!\n");
	}

	return 0;
}





//Thread Functions For Reading and Writing Data



DWORD WINAPI ThreadFunctionForReadingData(DATAREADINFO* hFileForDataReading)
{
	//Variable Related to File
	BOOL bInsertDataReturnValue = FALSE;
	char* pchDataReadBuffer = NULL;
	pchDataReadBuffer = (char*)malloc((ONEMB * sizeof(char)));
	if (pchDataReadBuffer == NULL)
	{
		printf("\nMemory allocation Failed\n");
		return FALSE;
	}

	BOOL bReturnValue = FALSE;
	OVERLAPPED lpReadingInformation = {0};

	lpReadingInformation.Offset = hFileForDataReading->dwStartOffset;

	//File Reading
	bReturnValue = _Notnull_ ReadFileEx(hFileForDataReading->hFileForDataReading, pchDataReadBuffer, (ONEMB*sizeof(char)), &lpReadingInformation,NULL);
	if (bReturnValue == FALSE)
	{
		std::cout << "Error:Failed to Read File" << std::endl;
		ErrorMessage((LPTSTR)TEXT("Read file by thread"));
		CloseHandle((hFileForDataReading->hFileForDataReading));
		return FALSE;
	}
	

	//calling InsertDataNodeFunction by Thread to store data in Node
	bInsertDataReturnValue =InsertDataToNode(pchDataReadBuffer);

	if (bInsertDataReturnValue == FALSE)
	{
		std::cout << "Data insertion in node Failed!!!" << std::endl;
		return FALSE;
	}
	printf("\n1MB Data write successfully in the [%d]node of linked list\n", (lpReadingInformation.Offset)/(ONEMB));

	return 0;
}




DWORD WINAPI ThreadFunctionForWritingData(DATAWRITEINFO* pdwWriteInfoForThread)
{
	//variables
	BOOL bReturnValue=FALSE;
	HANDLE hFileCreationForThread = { 0 };
	char cFileName[ONEKB] = { (char)(ASCIIVALUEFORA+pdwWriteInfoForThread->iPositionOfDataNodeToRead),'\0' };
	char* pcBufferToWrite = NULL;
	DWORD dwBytesWritten = 0;
	strcat_s(cFileName,".txt");
	DATANODE* pdnTraverseNode;
	int iPositionCount = 0;

	hFileCreationForThread = CreateFileA(cFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileCreationForThread == INVALID_HANDLE_VALUE)
	{
		std::cout << "File Creation failed!!" << std::endl;
		ErrorMessage((LPTSTR)TEXT("File Creation"));
		exit(EXIT_FAILURE);
	}

	//Memory allocation for pcBufferToWrite

	pcBufferToWrite = (char*)malloc(ONEMB * sizeof(char));
	if (pcBufferToWrite == NULL)
	{
		ErrorMessage((LPTSTR)TEXT("Memory allocation"));
		return FALSE;
	}

	pdnTraverseNode = pdwWriteInfoForThread->pdnHeadNode;
	while (iPositionCount != pdwWriteInfoForThread->iPositionOfDataNodeToRead)
	{
		pdnTraverseNode = pdnTraverseNode->pNextNode;
		iPositionCount++;
	}
	pcBufferToWrite = pdnTraverseNode->cDataBuffer;
	
	bReturnValue = WriteFile(hFileCreationForThread, (LPCVOID)pcBufferToWrite, (DWORD)(ONEMB * sizeof(char)), &dwBytesWritten, NULL);
	if (bReturnValue == FALSE)
	{
		std::cout << "File Writing Failed!!" << std::endl;
		ErrorMessage((LPTSTR)TEXT("Write"));
		free(pcBufferToWrite);
		CloseHandle(hFileCreationForThread);
		return FALSE;
	}
	if (dwBytesWritten != (ONEMB * sizeof(char)))
	{
		std::cout << "Error:Bytes Written != bytes want to write" << std::endl;
		free(pcBufferToWrite);
		return FALSE;
	}
	//free(pcBufferToWrite);
	CloseHandle(hFileCreationForThread);

}