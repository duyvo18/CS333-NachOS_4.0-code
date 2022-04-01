#include "syscall.h"

int main(int argc, char** argv)
{
    int srcOpenFile = Open(argv[1]);
    int destOpenFile = Open(argv[2]);
    
    char* tmp;


    Seek(-1, destOpenFile);

    Halt();
    /* not reached */
}
