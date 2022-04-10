#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	int id;
	int fileSize;
	char content[MAX_LENGTH];
	char fileName[MAX_LENGTH];
	int i; // Index for loop

	PrintString("Input name of file: ");

	ReadString(fileName, MAX_LENGTH); // To read name of file

	id = Open(fileName); // Goi ham Open de mo file

	if (id != -1) // Check if it is valid or not
	{
		// Seek to the end of file to get size
		fileSize = Seek(-1, id);
		i = 0;
		Close(id);
		
		id = Open(fileName);
		Read(content, fileSize, id); // Read the file
		Close(id);


		PrintString(content); // Print out file content
		PrintChar('\n');
	}
	else
	{
		PrintString("Error!!\n\n");
	}

	Halt();
	/* not reached */
}
