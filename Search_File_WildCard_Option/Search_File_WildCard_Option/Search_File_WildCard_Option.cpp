#include <iostream>
#include <Windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{

    if (argc != 3)
    {
        printf("Usage: %s <Directory path> <File Name(Wildcard option)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) > MAX_PATH)
    {
        printf("Directory path is too long!!!\n");
    }


    WIN32_FIND_DATAA FindData;  //Variable Declaration and Initialization
    memset(&FindData, 0, sizeof(FindData));

    long int liFileSize = 0;
    char sDirectoryPath[MAX_PATH] = { 0 };
    HANDLE hFind = INVALID_HANDLE_VALUE;
    HRESULT hReturnValue;
    char* sFullPath;


    hReturnValue = StringCchCopyA(sDirectoryPath, MAX_PATH, argv[1]);

    if (FAILED(hReturnValue))
    {
        exit(EXIT_FAILURE);
    }


    hReturnValue = StringCchCatA(sDirectoryPath, MAX_PATH, "\\");

    if (FAILED(hReturnValue))
    {
        exit(EXIT_FAILURE);
    }

    hReturnValue = StringCchCatA(sDirectoryPath, MAX_PATH, argv[2]);

    if (FAILED(hReturnValue))
    {
        exit(EXIT_FAILURE);
    }


    hFind = FindFirstFileA(sDirectoryPath, &FindData);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        exit(EXIT_FAILURE);
    }


    sFullPath = (char*)malloc(MAX_PATH * sizeof(char));   //Dynamic memory allocation of datatype char
    do
    {

        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            hReturnValue = StringCchCopyA(sFullPath, MAX_PATH, argv[1]);
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }

            hReturnValue = StringCchCatA(sFullPath, MAX_PATH, "\\");
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }


            hReturnValue = StringCchCatA(sFullPath, MAX_PATH, FindData.cFileName);
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }

            printf("File Name: %s<Dir>\n", sFullPath);
        }
        else
        {
            hReturnValue = StringCchCopyA(sFullPath, MAX_PATH, sDirectoryPath);
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }


            hReturnValue = StringCchCatA(sFullPath, MAX_PATH, "\\");
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }

            hReturnValue = StringCchCatA(sFullPath, MAX_PATH, FindData.cFileName);
            if (FAILED(hReturnValue))
            {
                exit(EXIT_FAILURE);
            }


            liFileSize = (FindData.nFileSizeHigh * (MAXDWORD)+FindData.nFileSizeLow);
            printf("File Name: %s", sFullPath);
            printf("\tFile Size: %d bytes\n", liFileSize);
        }
    } while (FindNextFileA(hFind, &FindData) != 0);

    FindClose(hFind);
    free(sFullPath);
    return(0);
}



