/*  help.c
 *
 *	Use the PrintString(char[]) syscall to
 *  print information about the group and
 *  a short description for "sort" and "ascii" program
 *
 */

#include "syscall.h"

int main()
{
    PrintString("\n\n"
                "=====  Group Info  =====\n\n"
                "  1. Vo Khuong Duy        -  19125087\n"
                "  2. Nguyen Hoang Hieu    -  19125091\n"
                "  3. Nguyen Lam Vinh Phu  -  19125114\n"
                "\n\n"
                "=====  ascii.c Program =====\n\n"
                "Print all readable ASCII charaters and their hex code.\n"
                "\n\n"
                "=====  sort.c Program  =====\n\n"
                "Take an integer array from standard I/O in console\n"
                "and print the array in sorted order.\n"
                "The user will be asked to choose the array size and the order.\n\n\n");

    Halt();
}
