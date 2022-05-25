#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

#include "../h/SYSTEM.h"
#define llock asm {pushf;cli;}
#define uunlock asm popf

int syncPrintf(const char *format, ...)
{
	llock;
	System::lock();
	int res;
	va_list args;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);

	uunlock;
	System::unlock();
	return res;
}
