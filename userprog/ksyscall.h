/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"

/*
 * C++'s sstream library to convert number/string to I/O stream
 */
#include <sstream>

/*
 * Copy buffer from user virtual memory to kernal memory space
 *
 * Input:
 *  - User virtual address (int)
 *  - User buffer size limit (int)
 *
 * Output:
 *  - Kernel buffer (char*)
 */
char *UserToKernel(int userAddr, int size)
{
  // Allocate kernel buffer
  char *kernBuffer = new char[size + 1];
  if (kernBuffer == NULL)
    return kernBuffer;

  // Empty kernel buffer
  memset(kernBuffer, '\0', size + 1);

  // Write user buffer to kernel buffer
  for (int i = 0; i < size; ++i)
  {
    int tmp;

    kernel->machine->ReadMem(userAddr + i, 1, &tmp);
    kernBuffer[i] = (char)tmp;

    // Early stop if needed
    if (tmp == 0)
      break;
  }

  return kernBuffer;
}

/*
 * Copy buffer from kernal memory space to user virtual memory
 *
 * Input:
 *  - User virtual address (int)
 *  - User buffer size limit (int)
 *  - Kernal buffer (char[])
 *
 * Output:
 *  - Kernal buffer size (char*)
 */
int KernelToUser(int userAddr, int bufferLimit, char kernBuffer[])
{
  // Buffer limit exceptions
  if (bufferLimit < 0)
    return -1;
  if (bufferLimit == 0)
    return bufferLimit;

  // Iteratively write kernel buffer to user virtual memory
  int tmp = 0, i = 0;
  do
  {
    tmp = (int)kernBuffer[i];
    kernel->machine->WriteMem(userAddr + i, 1, tmp);

    ++i;
  } while (i < bufferLimit && tmp != 0);

  kernel->machine->WriteMem(userAddr + i, 1, '\0');

  return i;
}

/*
 * Increase the program counter
 */
void IncreasePC()
{
  // Update previous programm counter
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

  // Update programm counter to next instruction (4 byte/instruction)
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

  // Update next programm counter for brach execution
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

void SysPrintNum(int num)
{
  // Convert int to string
  std::ostringstream ss;
  ss << num;
  string numStr = ss.str();

  // Iteratively print characters in num string
  int len = numStr.length();
  for (int i = 0; i < len; ++i)
  {
    kernel->synchConsoleOut->PutChar(numStr[i]);
  }
}

void SysPrintChar(char character)
{
  kernel->synchConsoleOut->PutChar(character);
}

void SysPrintString(int userBuffer)
{
  // Copy buffer from user virual memory to kernel
  char *kernelBuff = UserToKernel(userBuffer, 1000);

  // Find the length of the buffer
  int length = 0;
  while (kernelBuff[length] != '\0')
    ++length;

  for (int i = 0; i < length; i++)
    kernel->synchConsoleOut->PutChar(kernelBuff[i]);

  delete kernelBuff;
}

int SysRandomNum()
{
  srand(time(NULL));
  return rand() % 1000 + 0;
}

int SysReadNum()
{
  char c;
  int ans = 0;
  bool minus = false;
  c = kernel->synchConsoleIn->GetChar(); // get char one by one from the screen
  if (c == '-')                          // check it's negative ot not
    minus = true;
  else if (c > '9' || c < '0') // check it's valid or not
    return 0;
  else
    ans = c - '0'; // get number if it's bvalid

  while (1)
  {
    c = kernel->synchConsoleIn->GetChar();
    if (c == '\n' || c == ' ') // break point
      break;
    else if (c > '9' || c < '0') // Check valid
      return 0;
    ans = ans * 10 + (c - '0'); //
  }

  if (minus)
    return -ans; // return negative number

  return ans; // return positive number
}

char SysReadChar()
{
  char c;
  c = kernel->synchConsoleIn->GetChar(); // Get char from console
  return c;
}

void SysReadString(int buffer, int length)
{
  char *kerSpace = new char[length + 1]; // new space as kernel space

  memset(kerSpace, '\0', length + 1); // Initialized the kernel space with end of string

  for (int i = 0; i < length; ++i)
  {
    char tmp = kernel->synchConsoleIn->GetChar(); // Get char to kernel space directly not through kernel->machine->ReadMem()

    if (tmp == '\0' || tmp == '\n') // check the end of string
      break;

    kerSpace[i] = tmp;
  }

  int tmp = 0;
  int i = 0;
  do
  {
    tmp = (int)kerSpace[i];
    kernel->machine->WriteMem(buffer + i, 1, tmp); // Write chars (tmp) to the virtual address(buffer)
    i++;
  } while (i < length && tmp != 0); // check the end of string

  delete kerSpace;
}

int SysCreate(int userBuffer)
{
  // Copy buffer from user virual memory to kernel
  char *kernelBuff = UserToKernel(userBuffer, 1000);
  DEBUG(dbgSys, "File name input: \"" << kernelBuff << "\".\n");

  // Find the length of the buffer
  int length = 0;
  while (kernelBuff[length] != '\0')
    ++length;

  bool res = kernel->fileSystem->Create(kernelBuff);
  delete[] kernelBuff;

  if (res == true)
    return 0;
  else
    return -1;
}

int SysRemove(int userBuffer)
{
  // Copy buffer from user virual memory to kernel
  char *kernelBuff = UserToKernel(userBuffer, 1000);
  DEBUG(dbgSys, "File name input: \"" << kernelBuff << "\".\n");

  // Find the length of the buffer
  int length = 0;
  while (kernelBuff[length] != '\0')
    ++length;

  // if file not open
  bool res = kernel->fileSystem->Remove(kernelBuff);
  // elif file open
  // return -1
  delete kernelBuff;
  if (res == true) // and file was not open
    return 0;
  else // file delete fail or file was open
    return -1;
}

int SysOpenFile(int userStrBuffer)
{
  // Transfer str from user buffer to kernel
  char *kernelBuffer = UserToKernel(userStrBuffer, 1000);
  DEBUG(dbgSys, "File name input: \"" << kernelBuffer << "\".\n");

  // return the addr of OpenFile object as OpenFileID
  return (int)kernel->fileSystem->Open(kernelBuffer);
}

int SysCloseFile(int openFileAddr)
{
  // If OpenFile object is null (cannot open file), or is Console IO
  if (openFileAddr == 0 || openFileAddr == 1)
    return -1;

  DEBUG(dbgSys, "OpenFile addr: " << openFileAddr << ".\n");
  OpenFile *file = (OpenFile *)openFileAddr;

  // delete the OpenFile object is closing the file
  delete file;

  return 0;
}

int SysRead(char *buffer, int size, int id)
{
  if (size < 0 || id == 1)
  {
    return -1;
  }

  if (id == 0)
  {
    for (int i = 0; i < size; ++i)
    {
      buffer[i] = kernel->synchConsoleIn->GetChar();
      if (buffer[i] == EOF)
      {
        buffer[i] = 0;
        return -2;
      }
      return size;
    }
  }

  OpenFile *file = (OpenFile *)id;

  int charRead = file->Read(buffer, size);

  return charRead;
}

int SysWrite(char *buffer, int size, int id)
{
  if (size < 0 || id <= 0)
  {
    return -1;
  }

  if (id == 1)
  {
    for (int i = 0; i < size; ++i)
      kernel->synchConsoleOut->PutChar(buffer[i]);
    return size;
  }

  OpenFile *file = (OpenFile *)id;

  int charWrite = file->Write(buffer, size);
  return charWrite;
}

int SysSeekFile(int pos, int openFileAddr)
{
  // If OpenFile object is null (cannot open file), or is Console IO
  if (openFileAddr == 0 || openFileAddr == 1)
    return -1;

  // type cast the addr to OpenFile pointer
  OpenFile *file = (OpenFile *)openFileAddr;

  // if pos=-1 then get the length of the file, which is EOF offset
  if (pos == -1)
  {
    pos = file->Length();
  }

  // seek by reading to a temp buffer
  char *tmp = new char[pos + 1];
  int res = file->Read(tmp, pos);
  delete[] tmp;

  // if the final offset is equals to pos
  // then the input offset is seekable (valid offset)
  // otherwise return -1
  return res == pos ? pos : -1;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
