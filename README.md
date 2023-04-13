# File_Handling_Using_Win32API

## Copy_Application_Using_Multithread
This code is a multi-threaded file processing program that reads data from a file, stores it in linked list and then sorts the linked list. After sorting, each thread reads 1MB of data from the sorted linked list and writes it into a separate file.

The program first creates a file named "data.txt" and writes 10MB of random data into it. Then, it creates 10 threads to read data from the file in parallel, where each thread reads 1MB of data and stores it in a new node of the linked list. Once all the data has been read and stored in the linked list, the program sorts the linked list by the data value.

After sorting, the program creates 10 more threads, where each thread reads 1MB of data from the sorted linked list and writes it into a new file. Each thread creates a new file with the name "A.txt", "B.txt", "C.txt", and so on, depending on the thread number.

In summary, this program demonstrates how to use multithreading to process large files efficiently by reading data in parallel and sorting the data using a linked list.

## File_Generator
This C++ program generates a 10MB file named "data.txt" filled with random alphabetic characters using the Win32 API. The program creates a file handle, dynamically allocates memory for the buffer, writes to the file, and then frees the allocated memory. To compile and execute the code, a Win32 project should be created in Visual Studio.

## File_Traversal
This program is a simple Windows console application that uses Win32 API to traverse and display the files and directories in a given directory path. The user needs to provide the directory path as a command-line argument when running the program. The program recursively traverses all the subdirectories and displays the file name and size of each file found. The program also handles errors that may arise during the traversal process.

To run the program, open a command prompt, navigate to the directory where the program executable is located, and type the following command:

File_Traversal.exe directory_path

## Find_File_In_Directory
This code is a C++ program that searches for a file in a directory and its subdirectories using Windows API functions. It takes two command-line arguments: the directory path to search in, and the name of the file to search for. If the file is found, the program outputs the file's name and size. If the file is not found, the program outputs a message indicating that the file was not found.

To use this program, compile it using a C++ compiler, and run the executable with the two command-line arguments as mentioned above. The program will output the search results to the console.

## Search_File_WildCard_Option
This code is a C++ program that searches for a file in a directory using Windows API functions. It takes two command-line arguments: the directory path to search in, and the name of the file to search for (wildcard option allowed). If the file is found, the program outputs the file's name and size. If the file is not found, the program exits without any message.

To use this program, compile it using a C++ compiler, and run the executable with the two command-line arguments as mentioned above. The program will output the search results to the console.

