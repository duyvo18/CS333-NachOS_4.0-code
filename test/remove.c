#include "syscall.h"

int main(int argc, char** argv)
{
    Remove(argv[1]);
    Halt();
}