#include "syscall.h"

int main()
{
    char* a = "abc.txt";

    int res = Create(a, 10);

    PrintNum(res);

    Halt();
    /* not reached */
}
