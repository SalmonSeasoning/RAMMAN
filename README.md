# RAMMAN
Memory Read and Write processes

## How to use
You can either modify and compile this as a DLL or you can compile it as a .lib.
However, it might be more effective to just leave the project as is, and use the
singular header file instead.

The following shows you how to set this up for a process.
```
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "MemReadWrite.h"
int main()
{
	RAMMAN::MemReadWrite mrw = MemReadWrite();
	DWORD pId = mrw.GetProcessIdByName("target application name");
	mrw.SetHandle( pId );
	// your code goes here
	std::cin.get();
}
```
There are 3 constructors for `MemReadWrite`. The first is the process ID (`DWORD`).
The second is the process name (`LPCWCHAR`). The third is simply the process handle (`HANDLE`).
These set the object's private handle. `.SetHandle( HANDLE )` and `.GetHandle()` can be used
to set and get the handle.

## Functions
I suggest you take a look in the actual header file and read the comments for documentation
on the functions' purposes.

`GetProcessHandleByName` and `GetProcessHandleByName2` are technically the same, but use
different methods of getting the process handle.

`GetProcessIdByName` and `GetProcessIdByName2` are also technically the same, but yet again
use different methods of getting the same process ID.

For similar functions / methods, use the one that works best on average on multiple systems.
If one of these functions are not consistent, use the other one. If both are consistent,
which one you use really is up to you.