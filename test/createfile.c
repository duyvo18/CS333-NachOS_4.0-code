#include "syscall.h"

int main()
{
    char* filename[100];
    int res; 
    
    PrintString("Please enter the filename: ");
    ReadString(filename, 100);

    res = Create(filename);

    PrintString("Create file return ");
    PrintNum(res);
    PrintString(".\n");

    Halt();
    /* not reached */
}