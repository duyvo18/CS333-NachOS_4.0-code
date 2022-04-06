#include "syscall.h"

int main(int argc, char **argv)
{
    char *tmp[100];
    int byteRead = 100;
    
    int srcOpenFile = Open(argv[1]);
    int destOpenFile = Open(argv[2]);

    PrintString("\n > File opened\n");
    

    // PrintString("\n> Seeking to dest EOF...\n");
    // Seek(-1, destOpenFile);

    // PrintString("\n> Writing to dest...\n");
    // do
    // {
    //     byteRead = Read(tmp, 100, srcOpenFile);
    //     Write(tmp, 100, destOpenFile);
    // } while (byteRead != 100);
    
    // PrintString("\n> Closing files...\n");
    // Close(srcOpenFile);
    // Close(destOpenFile);
    
    PrintString("\n> Program ended\n");

    Halt();
    /* not reached */
}
