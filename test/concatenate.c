#include "syscall.h"

int main()
{
    int srcFile, dstFile;

    char *srcFilename = "source.txt";
    char *destFilename = "destination.txt";

    char *destContent = "This is content. ";
    char *srcContent = "Yes, yes it is.";

    int maxLen = 100;
    char *tmp[maxLen];

    int byteRead;

    // Create and write to file
    Create(srcFilename);
    Create(destFilename);

    srcFile = Open(srcFilename);
    dstFile = Open(destFilename);

    Write(srcContent, maxLen, srcFile);
    Write(destContent, maxLen, dstFile);

    Close(srcFile);
    Close(dstFile);

    // Concatenate srcFile to the end of destFile
    srcFile = Open(srcFilename);
    dstFile = Open(destFilename);

    Seek(-1, dstFile);

    byteRead = Read(tmp, maxLen, srcFile);
    Write(tmp, maxLen, dstFile);

    Close(srcFile);
    Close(dstFile);
    
    Halt();
    /* not reached */
}
