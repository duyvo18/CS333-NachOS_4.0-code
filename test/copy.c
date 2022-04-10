#include "syscall.h"
#define MAX_LENGTH 32

int main(){

	int srcId;
	int destId;

	int fileSize;
	int i; //Index for loop
	char c; 
	char source[MAX_LENGTH];
	char dest[MAX_LENGTH];
	
	PrintString("Input the name of source file: ");
	ReadString(source, MAX_LENGTH); // To read name of source file
	
	PrintString("Input the name of destination file: ");
	ReadString(dest, MAX_LENGTH); // To read name of dest file
	srcId = Open(source);
	
	if (srcId != -1) //check if it is valid or not
	{
		destId = Create(dest);
		Close(destId);
		
		destId = Open(dest); 
		if (destId != -1) //check if it is valid or not
        {	
			fileSize = Seek(-1, srcId);

			Close(srcId);
			Close(destId);

			srcId = Open(source);
			destId = Open(dest);

			i = 0;
			
			for (; i < fileSize; i++) {
				Read(&c, 1, srcId); 
				Write(&c, 1, destId); 
			}
			
			PrintString(" Sucessful!\n\n");
			Close(destId); // 
		}
		else{
			PrintString("Error!!!\n\n");
		}
		
		Close(srcId); // 
	}
	else{
		PrintString("Failed to open file!!\n");
	}

    Halt();
    /* not reached */
}

