#include "syscall.h"

int main(int argc, char** argv)
{
    Create(argv[1]);
    Halt();
}