
#include "..\dllmain.h"
#include "CPrecisionTimer.h"


static long long lastLogTime = 0;

void __cdecl MyPrinter(const char *format, ...)
{
	long long us = micros();
	long long show = (lastLogTime == 0) ? us : (us - lastLogTime);
	printf("%10lld - ", show);
	lastLogTime = us;

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}


void null_logger(const char *format, ...)
{
}
void (*dll_logger)(const char *format, ...) = null_logger;



char *ble_log_path = NULL;

HANDLE logMutex = CreateMutex(NULL, FALSE, NULL);

static bool oldLogDeleted = false;
static void Log2(const char* fmt, va_list args)
{
#if ALLOW_LOGGING
	WaitForSingleObject(logMutex, INFINITE);
	long long us = micros();
	long long show = (lastLogTime == 0) ? us : (us - lastLogTime);

	if (ble_log_path)
	{
		if (!oldLogDeleted)
		{
			DeleteFile(ble_log_path);
			oldLogDeleted = true;
		}

		// This is not perfect.  Down-sides:
		//	1) though the log is precise, the printf to STDOUT can get garbled at times.
		//	2) since the file is buffered, an external log watcher would not show all messages as the program runs.
		//int nbrOfCharsWritten = sprintf(logFileBuffer + logFileBufferInx, "%10lld - ", show);	logFileBufferInx += nbrOfCharsWritten;
		//nbrOfCharsWritten = vsprintf(logFileBuffer + logFileBufferInx, fmt, args);				logFileBufferInx += nbrOfCharsWritten;
		//strcat(logFileBuffer + logFileBufferInx, "\n");											logFileBufferInx++;
		//if (logFileBufferInx >= sizeof(logFileBuffer) - 500)
		//	Log2Flush();

		//printf("%10lld - ", show);
		//vprintf(fmt, args);
		//printf("\n");


		FILE *fp;
		fopen_s(&fp, ble_log_path, "at");
		fprintf(fp, "%10lld - ", show);
		vfprintf(fp, fmt, args);
		fprintf(fp, "\n");
		fclose(fp);
	}
	else
	{
		printf("%10lld - ", show);
		vprintf(fmt, args);
		printf("\n");
	}


	lastLogTime = us;
	ReleaseMutex(logMutex);
#endif
}


void GDH_Log(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	Log2(format, args);
	va_end(args);
}



void StrCatF(char *s, int size, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int len = strlen(s);
	int ret = vsprintf_s(s + len, size-len, format, args);
	va_end(args);
}

/*
* Find the first occurrence of the byte string s in byte string l.
*/
const void *memmem(const void *l, size_t l_len, const void *s, size_t s_len)
{
	register char *cur, *last;
	const char *cl = (const char *)l;
	const char *cs = (const char *)s;

	/* we need something to compare */
	if (l_len == 0 || s_len == 0)
		return NULL;

	/* "s" must be smaller or equal to "l" */
	if (l_len < s_len)
		return NULL;

	/* special case where s_len == 1 */
	if (s_len == 1)
		return memchr(l, (int)*cs, l_len);

	/* the last position where its possible to find "s" in "l" */
	last = (char *)cl + l_len - s_len;

	for (cur = (char *)cl; cur <= last; cur++)
		if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
			return cur;

	return NULL;
}





unsigned short BitConverter_ToUInt16(unsigned char* buf, int offset)
{
	int lsb = buf[0 + offset];
	int msb = buf[1 + offset];
	return (unsigned short)((msb << 8) | lsb);
}
unsigned int BitConverter_ToUInt32(unsigned char* buf, int offset)
{
	unsigned int b0 = buf[0 + offset];
	unsigned int b1 = buf[1 + offset];
	unsigned int b2 = buf[2 + offset];
	unsigned int b3 = buf[3 + offset];
	return (unsigned int)((b3 << 24) | (b2 << 16) | (b1 << 8) | b0);
}
int BitConverter_ToInt32(unsigned char* buf, int offset)
{
	int b0 = buf[0 + offset];
	int b1 = buf[1 + offset];
	int b2 = buf[2 + offset];
	int b3 = buf[3 + offset];
	return (int)((b3 << 24) | (b2 << 16) | (b1 << 8) | b0);
}
long long BitConverter_ToInt64(unsigned char* buf, int offset)
{
	int b0 = buf[0 + offset];
	int b1 = buf[1 + offset];
	int b2 = buf[2 + offset];
	int b3 = buf[3 + offset];
	int b4 = buf[4 + offset];
	int b5 = buf[5 + offset];
	int b6 = buf[6 + offset];
	int b7 = buf[7 + offset];
	// TODO:
	return (long long)((b3 << 24) | (b2 << 16) | (b1 << 8) | b0);
}


#define BYTE_1	0x000000ff
#define BYTE_2	0x0000ff00
#define BYTE_3	((unsigned long)0x00ff0000)
#define BYTE_4	((unsigned long)0xff000000)
void BitConverter_4(unsigned char *data, int offset, unsigned long id)
{
	data[offset + 0] = (unsigned char)(id & BYTE_1);
	data[offset + 1] = (unsigned char)((id & BYTE_2) >> 8);
	data[offset + 2] = (unsigned char)((id & BYTE_3) >> 16);
	data[offset + 3] = (unsigned char)((id & BYTE_4) >> 24);
}




long long __cdecl micros() { return PT.LapseMicrosecs(); }




float NormalizeAngle(float a)
{
	if (a < -180.0f)		a += 360.0f;
	else if (a > 180.0f)	a -= 360.0f;
	return a;
}