#include <windows.h>
#include <iostream>
#include <strsafe.h>
#include <stdio.h>
#include <string.h>


//Function Declaration
BOOL SearchFilesInDirectory(const char* sDirectoryPath, const char* sSearchFile);

int main(int argc, char* argv[])
{
    //Variable Declaration

    char sDirectoryPath[MAX_PATH] = { 0 };
    HRESULT hReturnValue;
    BOOL bFlag;


    //code
    hReturnValue = StringCchCopyA(sDirectoryPath, MAX_PATH, argv[1]);
    if (FAILED(hReturnValue))
    {
        exit(EXIT_FAILURE);
    }


    hReturnValue = StringCchCatA(sDirectoryPath, MAX_PATH, "\\*");
    if (FAILED(hReturnValue))
    {
        exit(EXIT_FAILURE);
    }


    //Function call
    bFlag = SearchFilesInDirectory(sDirectoryPath, argv[2]);
    if (bFlag != TRUE)
    {
        std::cout << "\nFile Not Found!!!" << std::endl;
    }


    return(0);
}


BOOL SearchFilesInDirectory(const char* sDirectoryPath, const char* sSearchFile)
{
    //variable declaration
    char sLocalFile[MAX_PATH] = { 0 };
    char sFilePath[MAX_PATH] = { 0 };
    char sFullPath[MAX_PATH] = { 0 };
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA FindData;
    long int iFileSize = 0;
    BOOL bFlag = TRUE;

    int str_size = 0;
    HRESULT hReturnValue;
    BOOL SearchFlag = FALSE;


    hReturnValue = StringCchCopyA(sLocalFile, MAX_PATH, sDirectoryPath);
    if (FAILED(hReturnValue))
    {
        return(FALSE);
    }


    hFind = FindFirstFileA(sLocalFile, &FindData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        std::cout << "Exit Failure" << std::endl;
        return(FALSE);
    }

    do
    {

        if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(FindData.cFileName, ".") != 0) && (strcmp(FindData.cFileName, "..") != 0))
        {
            hReturnValue = StringCchCopyA(sLocalFile, MAX_PATH, sDirectoryPath);
            if (FAILED(hReturnValue))
            {
                FindClose(hFind);
                return(FALSE);
            }



            str_size = strlen(sLocalFile);

            if (sLocalFile[str_size - 1] == '*')
            {
                sLocalFile[str_size - 1] = '\0';
            }


            if (lstrcmpiA(sSearchFile, FindData.cFileName) == 0)
            {
                StringCchCopyA(sFilePath, MAX_PATH, sLocalFile);
                StringCchCatA(sFilePath, MAX_PATH, FindData.cFileName);
                SearchFlag = TRUE;
                printf("File Name: %50s<DIR>\n", sFilePath);
            }
            StringCchCatA(sLocalFile, MAX_PATH, FindData.cFileName);

            StringCchCatA(sLocalFile, MAX_PATH, "\\*");

            bFlag = SearchFilesInDirectory(sLocalFile, sSearchFile);
            if (bFlag == TRUE)
            {
                SearchFlag = TRUE;
                return(SearchFlag);
            }
        }
        else
        {


            hReturnValue = StringCchCopyA(sLocalFile, MAX_PATH, sDirectoryPath);
            if (FAILED(hReturnValue))
            {
                FindClose(hFind);
                return(FALSE);
            }

            str_size = strlen(sLocalFile);
            if (sLocalFile[str_size - 1] == '*')
            {
                sLocalFile[str_size - 1] = '\0';
            }


            if (lstrcmpiA(sSearchFile, FindData.cFileName) == 0)
            {
                StringCchCopyA(sFullPath, MAX_PATH, sLocalFile);
                StringCchCatA(sFullPath, MAX_PATH, FindData.cFileName);
                SearchFlag = TRUE;
                printf("File Name: %50s", sFullPath);
                iFileSize = (FindData.nFileSizeHigh * (MAXDWORD)+FindData.nFileSizeLow);
                printf("\tFile Size: %20d bytes\n", iFileSize);

            }

        }
    } while (FindNextFileA(hFind, &FindData) != 0);


    FindClose(hFind);
    return SearchFlag;
}





