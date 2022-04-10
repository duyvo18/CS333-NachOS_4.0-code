// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"

/*
 * SynchConsoleInput/SynchConsoleOutput of console I/O operations
 */
#include "synchconsole.h"

#include "syscall.h"
#include "ksyscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
  int type = kernel->machine->ReadRegister(2);

  DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

  switch (which)
  {
  case SyscallException:
    switch (type)
    {
    case SC_Halt:
    {
      DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

      SysHalt();

      ASSERTNOTREACHED();
      break;
    }

    case SC_Add:
    {
      DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

      /* Process SysAdd Systemcall*/
      int result;
      result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
                      /* int op2 */ (int)kernel->machine->ReadRegister(5));

      DEBUG(dbgSys, "Add returning with " << result << "\n");
      /* Prepare Result */
      kernel->machine->WriteRegister(2, (int)result);

      /* Modify return point */
      {
        /* set previous programm counter (debugging only)*/
        kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

        /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
        kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

        /* set next programm counter for brach execution */
        kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
      }

      return;

      ASSERTNOTREACHED();

      break;
    }

    case SC_PrintNum:
    {
      DEBUG(dbgSys, "Printing an integer.\n");

      SysPrintNum(kernel->machine->ReadRegister(4));

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_PrintChar:
    {
      DEBUG(dbgSys, "Printing a character.\n");

      SysPrintChar(kernel->machine->ReadRegister(4));

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_PrintString:
    {
      DEBUG(dbgSys, "Printing a string.\n");

      SysPrintString(kernel->machine->ReadRegister(4));

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_RandomNum:
    {
      DEBUG(dbgSys, "Generating a random integer.\n");

      int num = SysRandomNum();
      DEBUG(dbgSys, "Num: " << num << "\n");

      kernel->machine->WriteRegister(2, (int)num);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_ReadNum:
    {
      DEBUG(dbgSys, "Reading an integer.\n");

      int num = SysReadNum();
      DEBUG(dbgSys, "Num: " << num << "\n");

      kernel->machine->WriteRegister(2, num);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_ReadChar:
    {
      DEBUG(dbgSys, "Reading a character.\n");

      int c = SysReadChar();
      DEBUG(dbgSys, "Char: " << c << "\n");

      kernel->machine->WriteRegister(2, (int)c);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_ReadString:
    {
      DEBUG(dbgSys, "Reading a string.\n");

      SysReadString((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5));

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Create:
    {
      DEBUG(dbgSys, "Creating a file.\n");

      int res = SysCreate((int)kernel->machine->ReadRegister(4));
      kernel->machine->WriteRegister(2, res);

      DEBUG(dbgSys, "Create file return " << res << ".\n");

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Remove:
    {
      DEBUG(dbgSys, "Removing a file.\n");

      int res = SysRemove((int)kernel->machine->ReadRegister(4));
      kernel->machine->WriteRegister(2, res);

      DEBUG(dbgSys, "Remove file return " << res << ".\n");

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Open:
    {
      DEBUG(dbgSys, "Opening file.\n");

      int openFileAddr = SysOpenFile(kernel->machine->ReadRegister(4));

      if (openFileAddr == NULL)
      {
        DEBUG(dbgSys, "Unable to open file.\n");
      }
      else
      {
        DEBUG(dbgSys, "OpenFile addr: " << openFileAddr << ".\n");
      }

      kernel->machine->WriteRegister(2, openFileAddr);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Close:
    {
      DEBUG(dbgSys, "Closing a file.\n");

      int flag = SysCloseFile(kernel->machine->ReadRegister(4));
      DEBUG(dbgSys, "Close file return " << flag << ".\n");

      kernel->machine->WriteRegister(2, flag);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Read:
    {
      DEBUG(dbgSys, "Reading a file.\n");

      int virAdd = kernel->machine->ReadRegister(4);    // get address of buffer from register 4
      int charCount = kernel->machine->ReadRegister(5); // get number of chars from register 5
      int id = kernel->machine->ReadRegister(6);        // get OpenFie_ID from register 6

      char *buffer = UserToKernel(virAdd, charCount);

      kernel->machine->WriteRegister(2, SysRead(buffer, charCount, id));
      KernelToUser(virAdd, charCount, buffer);

      delete[] buffer;

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Write:
    {
      DEBUG(dbgSys, "Writing to a file.\n");

      int virAdd = kernel->machine->ReadRegister(4);    // get address of buffer from register 4
      int charCount = kernel->machine->ReadRegister(5); // get number of chars from register 5
      int id = kernel->machine->ReadRegister(6);        // get OpenFie_ID from register 6

      char *buffer = UserToKernel(virAdd, charCount);

      // Find the length of the buffer
      int length = 0;
      while (buffer[length] != '\0')
        ++length;

      kernel->machine->WriteRegister(2, SysWrite(buffer, length, id));

      delete[] buffer;

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    case SC_Seek:
    {
      DEBUG(dbgSys, "Seeking a file.\n");

      int pos = SysSeekFile(kernel->machine->ReadRegister(4),
                            kernel->machine->ReadRegister(5));
      DEBUG(dbgSys, "Seek to offset " << pos << ".\n");

      kernel->machine->WriteRegister(2, pos);

      IncreasePC();
      return;

      ASSERTNOTREACHED();
    }

    default:
    {
      cerr << "Unexpected system call " << type << "\n";
      break;
    }
    }
    break;

  case NoException:
  {
    // Return control to system
    kernel->interrupt->setStatus(SystemMode);
    break;
  }

  case PageFaultException:
  {
    cerr << "No valid translation found\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case ReadOnlyException:
  {
    cerr << "Attempted to write to \"read-only\" page\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case BusErrorException:
  {
    cerr << "Translation resulted in an invalid physical address\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case AddressErrorException:
  {
    cerr << "Unaligned reference or one that was beyond the end of the address space\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case OverflowException:
  {
    cerr << "Interger overflow in add or sub\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case IllegalInstrException:
  {
    cerr << "Unimplemented or reserved instruction\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }

  case NumExceptionTypes:
  {
    cerr << "NumExceptionTypes\n";

    DEBUG(dbgSys, "Shutdown, initiated by ExceptionHandler.\n");
    SysHalt();

    ASSERTNOTREACHED();
    break;
  }
  default:
  {
    cerr << "Unexpected user mode exception" << (int)which << "\n";
    break;
  }
  }
  ASSERTNOTREACHED();
}
