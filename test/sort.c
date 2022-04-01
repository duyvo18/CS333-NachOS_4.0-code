/*  sort.c
 *
 *	Bubble sort with selectable ordering
 *
 * Use ReadNum() and PrintString(char[]) syscall to
 * read integer input and print user instructions
 * to the console.
 *
 * The users can choose the size of the input array and
 * what ordering do they want for the sorting process
 *
 */

#include "syscall.h"

// Array size limit
const int ARRAY_MAX = 100;

// Bubble sort in increasing order
void IncreasingSort(int *arr, int n)
{
  int i, j, tmp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = 0; j < n - i - 1; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

// Bubble sort in decreasing order
void DecreasingSort(int *arr, int n)
{
  int i, j, tmp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = 0; j < n - i - 1; j++)
    {
      if (arr[j] < arr[j + 1])
      {
        tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

int main()
{
  int n, i, order;
  int arr[ARRAY_MAX];

  // Take integer input for the array size
  PrintString("\n\nPlease enter the array's size: ");
  n = ReadNum();

  // Repeatedly prompt the user if the input size is
  // higher than the limit
  while (n > ARRAY_MAX)
  {
    PrintString("\n\nThe size of the array must be less than ");
    PrintNum(ARRAY_MAX);
    PrintString("\nPlease enter a new array's size: ");

    n = ReadNum();
  }

  // Iteratively prompt the user to input the array's elements
  for (i = 0; i < n; ++i)
  {
    PrintString("\nPlease enter the ");
    PrintNum(i + 1);
    PrintString(" element: ");

    arr[i] = ReadNum();
  }

  // Prompting the user to select their preferred order
  order = 0;
  while (order != 1 && order != 2)
  {
    PrintString(
        "\nPlease select the preferred ordering\n"
        "  1. Increasing.\n"
        "  2. Decreasing.\n"
        "> ");

    order = ReadNum();
  }

  // The variable order is handled to be either
  // 1 for increase or 2 decrease ordering
  switch (order)
  {
  case 1:
    IncreasingSort(arr, n);
    break;

  default:
    DecreasingSort(arr, n);
    break;
  }

  // Print the sorted array to the console
  PrintString("\nSorted array:\n");

  for (i = 0; i < n; ++i)
  {
    PrintNum(arr[i]);
    PrintString(" ");
  }

  PrintString("\n\n");

  Halt();

  return;
}