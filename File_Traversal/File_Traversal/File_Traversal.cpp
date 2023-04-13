#include <Windows.h>
#include <iostream>
#include <strsafe.h>
#include <stdio.h>
#include <string.h>

//Function Declaration
BOOL ShowFilesInDirectory(const char* szfile);

int main(int argc, char* argv[])
{
    //Variable Declaration

    char szfile[MAX_PATH];
    HRESULT hr;
    BOOL flag;


    //code
    hr = StringCchCopyA(szfile, MAX_PATH, argv[1]);
    if (FAILED(hr))
    {
        exit(EXIT_FAILURE);
    }


    hr = StringCchCatA(szfile, MAX_PATH, "\\*");
    if (FAILED(hr))
    {
        exit(EXIT_FAILURE);
    }


    //Function call
    flag = ShowFilesInDirectory(szfile);
    if (flag == TRUE)
    {
        std::cout << "\nTraversal Done!!!";
    }

    return(0);
}


BOOL ShowFilesInDirectory(const char* szfile)
{
    //variable declaration
    char localfile[MAX_PATH];
    HANDLE hfind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA fd;
    long int filesize;
    BOOL flag;
    char FullPath[MAX_PATH];
    int str_size;
    HRESULT hr;



    hr = StringCchCopyA(localfile, MAX_PATH, szfile);
    if (FAILED(hr))
    {
        return(FALSE);
    }


    hfind = FindFirstFileA(localfile, &fd);
    if (INVALID_HANDLE_VALUE == hfind)
    {
        std::cout << "Exit Failure" << std::endl;
        FindClose(hfind);
        return(FALSE);
    }

    do
    {

        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(fd.cFileName, ".") != 0) && (strcmp(fd.cFileName, "..") != 0))
        {
            hr = StringCchCopyA(localfile, MAX_PATH, szfile);
            if (FAILED(hr))
            {
                FindClose(hfind);
                return(FALSE);
            }


            filesize = (fd.nFileSizeHigh * (MAXDWORD)+fd.nFileSizeLow);
            str_size = strlen(localfile);

            if (localfile[str_size - 1] == '*')
            {
                localfile[str_size - 1] = '\0';
            }



            StringCchCatA(localfile, MAX_PATH, fd.cFileName);

            printf("File Name: %50s<DIR>\n", localfile);

            StringCchCatA(localfile, MAX_PATH, "\\*");

            flag = ShowFilesInDirectory(localfile);
            if (flag == FALSE)
            {
                return(FALSE);
            }
        }
        else
        {
            filesize = (fd.nFileSizeHigh * (MAXDWORD)+fd.nFileSizeLow);

            hr = StringCchCopyA(localfile, MAX_PATH, szfile);
            if (FAILED(hr))
            {
                FindClose(hfind);
                return(FALSE);
            }

            str_size = strlen(localfile);
            if (localfile[str_size - 1] == '*')
            {
                localfile[str_size - 1] = '\0';
            }

            StringCchCopyA(FullPath, MAX_PATH, localfile);
            StringCchCatA(FullPath, MAX_PATH, fd.cFileName);
            printf("File Name: %50s", FullPath);
            printf("\tFile Size: %20d bytes\n", filesize);
        }
    } while (FindNextFileA(hfind, &fd) != 0);


    FindClose(hfind);
    return TRUE;
}




