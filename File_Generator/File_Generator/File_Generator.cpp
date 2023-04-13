//header files
#include <Windows.h>
#include <iostream>
#include <stdlib.h>

#define ONEKB 1024
#define FILESIZE ONEKB*ONEKB*10-1
//global function declarations

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char RandomCharacter();
char* CreateBuffer(int buffersize);

//global variable declaration

//Entry point function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdshow)
{
    HANDLE filehwnd;
    char* buffer = NULL;
    DWORD bWrittenFlag;
    BOOL Errorflag = FALSE;

    filehwnd = CreateFileA("data.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (filehwnd == INVALID_HANDLE_VALUE)
    {
        std::cout << "!!Error occured During File Creation";
        EXIT_FAILURE;
    }

    buffer = CreateBuffer(FILESIZE);
    if (buffer == NULL)
    {
        std::cout << "Buffer not created";
        EXIT_FAILURE;
    }

    Errorflag = WriteFile(filehwnd, buffer, FILESIZE, &bWrittenFlag, NULL);
    if (Errorflag == FALSE)
    {
        std::cout << "Error Occured during Writing File";
        EXIT_FAILURE;
    }

    if (FILESIZE != bWrittenFlag)
    {
        std::cout << "Error:Bytes Written != Bytes want to write";
        EXIT_FAILURE;
    }

    std::cout << "File Write Successfully";

    CloseHandle(filehwnd);
    free(buffer);

    return(0);
}


char RandomCharacter()      // Function for Generating Random Alphabet character
{
    int value;
    value = 65 + (rand() % 26);
    return (char)value;
}

char* CreateBuffer(int buffersize)  //Function for creating buffer of random characters
{
    int i;
    char* buffer = NULL;
    buffer = (char*)malloc(buffersize * sizeof(char));
    if (buffer == NULL)
    {
        std::cout << "Not Enough Memory";
        EXIT_FAILURE;
    }
    for (i = 0; i < buffersize; i++)
    {
        *(buffer + i) = RandomCharacter();
    }
    return buffer;
}




