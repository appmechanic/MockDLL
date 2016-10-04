
#include "general.h"

float l_pitch = NULL_FLOAT;
float l_yaw = NULL_FLOAT;
float l_roll = NULL_FLOAT;
float d_yaw = 0.0;
float d_pitch = 0.0;

float ax;
float ay;

POINT cursor;

void Init()
{
	QueryPerformanceFrequency(&Frequency);
	ResetBeginTime();
	
	// when your program starts call the SystemParametersInfo with the SPI_SETBLOCKSENDINPUTRESETS, 
	// so that the screensaver will not be activated while your program is using the SendInput() 
	int rtnData;
	SystemParametersInfo(
		  SPI_SETBLOCKSENDINPUTRESETS,
		  TRUE,
		  &rtnData,
		  0
		);
}

void imu_reset_coord()
{
	ax = ay = 0.0f;
	//POINT origin;
	//GetCursorPos(&origin);
	//ax = (float)origin.x;
	//ay = (float)origin.y;
}

bool switched_imu = false;
void imu_to_mouseMove(float pitch, float yaw, float roll)
{
	if (l_pitch == NULL_FLOAT)
		imu_reset_coord();
	else // if (l_pitch != NULL_FLOAT)
	{
		float pitchChange = (l_pitch - pitch);
		float yawChange = l_yaw - yaw;
		if ( fabs(l_yaw - (yaw * -1.0f)) < fabs(yawChange) )
			yawChange = l_yaw - (yaw * -1.0f);
			
		ax += yawChange * AMPILFY;
		ay += pitchChange * AMPILFY;

		float dx = ax - cursor.x;
		float dy = ay - cursor.y;
			
		if ((abs(dx) > 1.0 || abs(dy) > 1.0))
		{
			if (switched_imu)
				switched_imu = false;
			else
				MoveMouse((int)dx, (int)dy);
			cursor.x += (int)dx;
			cursor.y += (int)dy;
		}
	}

	l_pitch = pitch;
	l_yaw = yaw;
	l_roll = roll;
}


void MoveMouse(int dx, int dy)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData=0;
	input.mi.dx =  dx;
	input.mi.dy =  dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1,&input,sizeof(input));
}
void MoveMouseAbs(int x, int y) // web: "Mouse events like this don't work in source engine games. If you try using it, it makes you spin around looking at the ground endlessly. It works fine in other games like cod4, but that doesnt mean it'll work in csgo."
{
	double screenX = GetSystemMetrics( SM_CXVIRTUALSCREEN );
	double screenY = GetSystemMetrics( SM_CYVIRTUALSCREEN );

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData=0;
	input.mi.dx = (int)( ((double)x) * ( 65535.0/screenX ) );
	input.mi.dy = (int)( ((double)y) * ( 65535.0/screenY ) );
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
	SendInput(1,&input,sizeof(input));
}

// Turns mouse acceleration on/off by calling the SystemParametersInfo function. - http://stackoverflow.com/questions/3902477/how-to-configure-mouse-enhance-pointer-precision-programatically
// When enhancePointerPrecision is TRUE, mouse acceleration is turned on; FALSE for off.
void SetMouseAcceleration(BOOL enhancePointerPrecision)
{
    int mouseParams[3];

    // Get the current values.
    SystemParametersInfo(SPI_GETMOUSE, 0, mouseParams, 0);

    // Modify the "Enhance Pointer Precision" checkbox.  TRUE for checked.
    mouseParams[2] = enhancePointerPrecision;

    // Update the system setting.
    SystemParametersInfo(SPI_SETMOUSE, 0, mouseParams, SPIF_SENDCHANGE);
}



	/**************************************************/
	/***	N a v i g a t i o n   &   I n p u t		***/
	/**************************************************/

#include "dinput.h"

DirectXScanCodes::DirectXScanCodes()
{
    input.type = INPUT_KEYBOARD;
    input.ki.time = 0;
    input.ki.wVk = 0; //We're doing scan codes instead
    input.ki.dwExtraInfo = 0;
}
char DirectXCode(int ch) { return ch == DIK_W ? 'W'
						  : ch == DIK_A ? 'A'
						  : ch == DIK_S ? 'S'
						  : ch == DIK_D ? 'D' : '?'; }

void DirectXScanCodes::KeyDown(char ch)
{
	//printf("KeyDown(%c)\n", DirectXCode(ch));
    input.ki.dwFlags = KEYEVENTF_SCANCODE;
    input.ki.wScan = ch;  
	int num = SendInput(1, &input, sizeof(INPUT));
	if (num == 0)
	{
		char buffer[256] = {0};
		DWORD errorCode = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, 256, NULL);
		printf("KeyDown(0x%x) failed:  [%s]", (int)ch, buffer);
	}
}
void DirectXScanCodes::KeyUp(char ch)
{
	//printf("KeyUp  (%c)\n", DirectXCode(ch));
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    input.ki.wScan = ch;  
	int num = SendInput(1, &input, sizeof(INPUT));
	if (num == 0)
	{
		char buffer[256] = {0};
		DWORD errorCode = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, 256, NULL);
		printf("KeyUp(0x%x) failed:  [%s]", (int)ch, buffer);
	}
}


// Synthesize keyboard typing for regular characters.
void KeybdType(char *str)
{
	INPUT inputs[] = {
						{ INPUT_KEYBOARD, { 0, 0, 0,			   0, 0 } },
						{ INPUT_KEYBOARD, { 0, 0, KEYEVENTF_KEYUP, 0, 0 } }
					};


	while (*str)
	{
		int num;
		SHORT vky = VkKeyScan( *str );
		inputs[0].ki.wVk = vky;
		inputs[1].ki.wVk = vky;

		num = SendInput(2, inputs, sizeof(INPUT));

		str++;
	}
}

// Presses the control-key in combination with a letter.
// For instance, KeybdControl('s') sends CNTRL+S which is Save in most programs.
// FIX - doesn't release the control key.  Very, very, annoying.
void KeybdControl(char ch)
{
	INPUT controlSeq[] = {
						{ INPUT_KEYBOARD, { VK_CONTROL,	0, 0,				0, 0 } },
						{ INPUT_KEYBOARD, { 0,			0, 0,				0, 0 } },
						{ INPUT_KEYBOARD, { 0,			0, KEYEVENTF_KEYUP,	0, 0 } },
						{ INPUT_KEYBOARD, { VK_CONTROL,	0, KEYEVENTF_KEYUP,	0, 0 } }
					};
	SHORT vky = VkKeyScan( ch );
	int num;

	controlSeq[1].ki.wVk = vky;
	controlSeq[2].ki.wVk = vky;
	num = SendInput(4, controlSeq, sizeof(INPUT));
}

// Synthesizes a sequence of characters with ALT pressed first.
// FIX - doesn't release the alt key.  Very, very, annoying.
void KeybdAlt(char *str)
{
	INPUT altSeq[] = {
						{ INPUT_KEYBOARD, { VK_MENU,	0, 0,				0, 0 } },
						{ INPUT_KEYBOARD, { VK_MENU,	0, KEYEVENTF_KEYUP,	0, 0 } },
					};
	char *start = str;


	SendInput(2, altSeq, sizeof(INPUT));	// Alt Down & Up
	while (*str)
	{
		SHORT vky = VkKeyScan( *str );
		altSeq[0].ki.wVk = vky;
		altSeq[1].ki.wVk = vky;
		SendInput(2, altSeq, sizeof(INPUT));
		str++;
	}

}

// Single click the left mouse button.
void ClickMouse()
{
	INPUT inputs[] = {
						{ INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0, 0 } },
						{ INPUT_MOUSE, { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0, 0 } },
					};
	int num;


	num = SendInput(2, inputs, sizeof(INPUT));
	if (num == 0)
	{
		char buffer[256] = {0};
		DWORD errorCode = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, 256, NULL);
		printf("Jump mouse failed:  [%s]", buffer );
	}
}



void eulerAngles(double w, double x, double y, double z, float* pitchPtr, float* rollPtr, float* yawPtr) 
{	// http://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
	float pt = (float)atan2 (2.0 * (x * w + y * z), 1 - 2.0 * (y * y + x * x)); // Pitch
	float rl = (float)asin (2.0 * (w * y - x * z));								// Roll
	float yw = (float)atan2 (2.0 * (z * w + x * y), 1 - 2.0 * (z * z + y * y)); // Yaw

	*pitchPtr = pt * Rad2Deg;
	*rollPtr  = rl * Rad2Deg;
	*yawPtr   = yw * Rad2Deg;
}

LARGE_INTEGER Frequency;
LARGE_INTEGER LogBeginTime;
void ResetBeginTime()
{
	QueryPerformanceCounter(&LogBeginTime);
}
int LogMS()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	LARGE_INTEGER ElapsedMilliseconds;
	ElapsedMilliseconds.QuadPart = CurrentTime.QuadPart - LogBeginTime.QuadPart;
	ElapsedMilliseconds.QuadPart *= 1000; // To guard against loss-of-precision, we convert to millisec
	ElapsedMilliseconds.QuadPart /= Frequency.QuadPart; // *before* dividing by ticks-per-second.
	return (int)ElapsedMilliseconds.QuadPart;
}





std::ofstream* StartLog(char *path)
{
	std::ofstream* logFS = new std::ofstream(path);
	*logFS << "<r>" << std::endl;
	return logFS;
}
void EndLog(std::ofstream* logFS)
{
	*logFS << "</r>" << std::endl;
	logFS->close();
}

void EnsureLogStarted(std::ofstream** logPtr, char *path)
{
	if (*logPtr == NULL)
		*logPtr = StartLog(path);
}


char *PathFmt(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    static char buffer[4096];
    vsprintf_s (buffer, sizeof(buffer), fmt, args);
    va_end(args);
	return buffer;
}

void DumpBinary(char *path, char *data, int len)
{
	std::ofstream myFile(path, std::ios::out | std::ios::binary);
	myFile.write(data, len);
	myFile.close();
}


void ClearDirFiles(char *dir, char *filePatternMatch)
{
	std::vector<std::string> files;
	if (ListFiles(dir, filePatternMatch, files))
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			DeleteFile(it->c_str());
}

#include <stack>

bool ListFiles(std::string path, std::string mask, std::vector<std::string>& files)
{
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
    std::string spec;
    std::stack<std::string> directories;

    directories.push(path);
    files.clear();

    while (!directories.empty()) {
        path = directories.top();
        spec = path + "\\" + mask;
        directories.pop();

        hFind = FindFirstFile(spec.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE)  {
            return false;
        } 

        do {
            if (strcmp(ffd.cFileName, ".") != 0 && 
                strcmp(ffd.cFileName, "..") != 0) {
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    directories.push(path + "\\" + ffd.cFileName);
                }
                else {
                    files.push_back(path + "\\" + ffd.cFileName);
                }
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            FindClose(hFind);
            return false;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return true;
}

#include <Strsafe.h>

extern void(*dll_logger)(const char *format, ...);
// Retrieve and output the system error message for the last-error code
void DisplayError(LPTSTR lpszFunction)
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, 
        NULL );

    lpDisplayBuf = 
        (LPVOID)LocalAlloc( LMEM_ZEROINIT, 
                            ( lstrlen((LPCTSTR)lpMsgBuf)
                              + lstrlen((LPCTSTR)lpszFunction)
                              + 40) // account for format string
                            * sizeof(TCHAR) );
    
    if (FAILED( StringCchPrintf((LPTSTR)lpDisplayBuf, 
                     LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                     TEXT("%s failed with error code %d as follows:\n%s"), 
                     lpszFunction, 
                     dw, 
                     lpMsgBuf)))
    {
		dll_logger("FATAL ERROR: Unable to output error code.\n");
    }
    
	dll_logger("DisplayError: %s\n", (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


// --- Parsing XML ------------------------------------

void slurpBinary(const char* filename, int len, char *buffer)
{
	std::ifstream file(filename, std::ios::binary);
	file.read(buffer, len);
	file.close();
}
char *slurpText(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	int len = (int)size;
	char *data = (char*)malloc( len + 1 );
	file.seekg(0, std::ios::beg);
	file.read(data, size);
	file.close();
	data[len] = 0;
	return data;
}











int a2hex(int c)
{
    if ( isdigit(c) )
        return c - '0';
    else if ( isupper(c) )
        return (c - 'A') + 10;
    else            // must be lower case
        return (c - 'a') + 10;
}

int hex2a(int h)
{
    if ( h >= 10 )
    {
        return (h - 10) + 'A';
    }
    else
    {
        return h + 48;
    }
}
char *hex_string(unsigned char *data, int len, char *outputString)
{
    int rinx, winx;
    for (rinx = 0, winx = 0; rinx < len; )
    {
        outputString[winx++] = hex2a(data[rinx] / 16);
        outputString[winx++] = hex2a(data[rinx++] % 16);
		if (rinx % 4 == 0 && rinx < len)
			outputString[winx++] = ':';
    }
    outputString[winx] = 0;

    return( outputString );
}





#undef _crt_va_start
#define _crt_va_start	(ap = (va_list )(&(cmdFormat))+((sizeof( cmdFormat )+sizeof( int  )-1)&~(sizeof( int  )-1)))
#undef _crt_va_end
#define _crt_va_end	(ap = (va_list )0)
#undef RtlZeroMemory
#define RtlZeroMemory	memset((void *)(&si),0,(sizeof( si )))

void run(char *cmd)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    RtlZeroMemory;
    si.cb = sizeof( si );
    CreateProcess(NULL,cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    //WaitForSingleObject(pi.hProcess,INFINITE);
}

void runf(char *cmdFormat, ...)
{
    va_list ap;
    char buf[8192];
    _crt_va_start;
    vsnprintf(buf,sizeof(buf),cmdFormat,ap);
    run(buf);
    _crt_va_end;
}




float AxisAdd(float a, float b)
{
	float sum = a + b;
	if (sum > 180.0f)
		sum -= 360.0f;
	else if (sum < -180.0f)
		sum += 360.0f;
	return sum;
}

float AxisSubtract(float minuend, float subtrahend)
{
	float diff = minuend - subtrahend;
	if (diff > 180.0f)
		diff -= 360.0f;
	else if (diff < -180.0f)
		diff += 360.0f;
	return diff;
}



bool Btwn(long long from, long long v, long long to)
{
	return from <= v && v <= to;
}
bool Intersection(Tuple *a, Tuple *b, Tuple *overlap)
{
    bool x = Btwn(b->Item1, a->Item1, b->Item2);
    bool y = Btwn(b->Item1, a->Item2, b->Item2);
    bool z = Btwn(a->Item1, b->Item2, a->Item2);
    if (!x && !y)
    {
        if (z) // A is not inside B, but B is inside A.  A completely encloses B
		{
			overlap->Item1 = b->Item1;
			overlap->Item2 = b->Item2;
		}
        else
            return false;
    }
    else if (!y) // x (but not x)                   Overlap, B before A
	{
		overlap->Item1 = a->Item1;
		overlap->Item2 = b->Item2;
	}
    else if (!x) // y (but not y)                   Overlap, A before B
	{
		overlap->Item1 = b->Item1;
		overlap->Item2 = a->Item2;
	}
    else // x & y                                   B completely encloses A
	{
		overlap->Item1 = a->Item1;
		overlap->Item2 = a->Item2;
	}
	return true;
}



int round_int(double r) { return (r > 0.0) ? (r + 0.5) : (r - 0.5); } // http://stackoverflow.com/questions/485525/round-for-float-in-c