#pragma once


////////////////
// The Config //

#define ALLOW_LOGGING	1

#define USE_BLED112		1

#if USE_BLED112
#define BLUEFRUIT_DEVICE_ID		BLUEFRUIT_FEATHER1
#define BLED112_SHOW_DEBUG		1
#endif

#define BLE_PACKETS_ONLY_TEST	0

////////////////








#define BLUEFRUIT_DEV_BOARD				0xce, 0x74, 0x0d, 0xb2, 0x49, 0xc8 // no marks on it.
#define BLUEFRUIT_GUN1					0x3d, 0x57, 0x0f, 0xbf, 0x7a, 0x80
#define BLUEFRUIT_OFFICE_BREADBOARD		0xfe, 0xb5, 0x89, 0xc9, 0xcd, 0xf0
#define BLUEFRUIT_FEATHER1				0x1b, 0xfd, 0xbf, 0x0c, 0x11, 0xfc

#define TRINKET_FOR_DUBAI				0xdd, 0xd9, 0xda, 0xe6, 0xbc, 0xfb


typedef signed char         int8;
typedef signed short        int16;
typedef signed int			int32;
typedef signed __int64		int64;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64	uint64;
typedef float               f32;
typedef double              f64;


struct Joystick
{
	Joystick()
	{
	}
	Joystick(float _x, float _y, bool _button)
	{
		X = _x; Y = _y; selectButton = _button;
	}
	float X;
	float Y;
	bool selectButton;
	float Len() { return sqrt(X*X + Y*Y); }
};



struct AxisCalib
{
	unsigned short low;
	unsigned short mid;
	unsigned short high;

	float CalibratedValue(unsigned short pots_10bit)
	{
		if (mid == 1018)
		{
			switch (pots_10bit)
			{
			case 1020: return 1.0f;
			case 1019: return 0.8f;
			case 1018: return 0.0f;
			case 1017: return 0.0f;
			default:
				if (pots_10bit > 1020) return 1.0f;
				return -1.0f * (((float)(mid - pots_10bit)) / ((float)(mid - low)));
				break;
			}
		}

		if (pots_10bit > mid)
			return ((float)(pots_10bit-mid)) / ((float)(high - mid));
		else
			return -1.0f * (((float)(mid - pots_10bit)) / ((float)(mid - low)));
	}
};
struct JoystickCalib
{
	AxisCalib X, Y;
};


struct SpecificGun
{
	uint8 ble_address[6]; bool isFeather;

	JoystickCalib ll;
	JoystickCalib lr;
	JoystickCalib ul;
	JoystickCalib ur;
};


extern SpecificGun *selectedGun;

#define NO_START_AT	-1
struct GameStartAt
{
	int level;
	int difficulty;
	int timeLimit;
};





#ifdef GUNBLE_DLLEXPORTS
#define GUNBLE_DLLDECL	__declspec(dllexport)
#else
#define GUNBLE_DLLDECL	__declspec(dllimport)
#endif






#pragma once
// Custom function declarations goes here:
#ifdef __cplusplus



void null_logger(const char *format, ...);

extern void(*dll_logger)(const char *format, ...);



typedef void(*FuncPtr)(const char *format, ...);




typedef void(__stdcall * UpdateCallback)(int);

typedef void(__stdcall * LoggerCallback)(char*);

void Unity_Log(char*);


extern "C"
{


	GUNBLE_DLLDECL void SetDebugFunction(FuncPtr);

	GUNBLE_DLLDECL void DoWork(UpdateCallback progressCallback);
	
	GUNBLE_DLLDECL void DoLog(LoggerCallback loggerCallback);


}


extern "C"
{


	


#endif
	// Just argument type of functions must be declared (no names):
	GUNBLE_DLLDECL int	__stdcall GetSimpleInt(int);



	GUNBLE_DLLDECL void __stdcall MyPrinter(const char *format, ...);





#if BLED112_SHOW_DEBUG
#define Log	dll_logger



#else
#define Log		__noop
#endif
	GUNBLE_DLLDECL long long __stdcall micros();

	GUNBLE_DLLDECL void __stdcall VRsenal_InitHaptic(char *audioDeviceName);

	GUNBLE_DLLDECL bool __stdcall VRsenal_Init(char *comPort, void(*logger)(const char *format, ...), int gunNbr);
	GUNBLE_DLLDECL bool __stdcall VRsenal_Init2(char *comPort, void(*logger)(const char *format, ...), SpecificGun *g);
	GUNBLE_DLLDECL void __stdcall VRsenal_Shutdown();

	GUNBLE_DLLDECL bool __stdcall VRsenal_IsConnected();

	GUNBLE_DLLDECL bool __stdcall VRsenal_Update(Joystick &lowerLeft, Joystick &lowerRight,
		Joystick &upperLeft,
		Joystick &upperRight,
		bool &trigger,
		GameStartAt &startAt,
		bool &requestShutdown);
	GUNBLE_DLLDECL void __stdcall VRsenal_DisconnectFromBluetooth();
	GUNBLE_DLLDECL void __stdcall VRsenal_CloseCOMPort();

	GUNBLE_DLLDECL void __stdcall VRsenal_RumbleOn();



	GUNBLE_DLLDECL void __stdcall VRsenal_RumbleOff();





#if BLE_PACKETS_ONLY_TEST
	GUNBLE_DLLDECL long long __stdcall VRsenal_PacketNbr();
#endif


	GUNBLE_DLLDECL bool __stdcall VRsenal_RetryInit();



	GUNBLE_DLLDECL void __stdcall VRsenal_SendBLE(unsigned char b0, unsigned char b1, unsigned char b2);



	GUNBLE_DLLDECL extern SpecificGun VRsenal_Gun_001;
	GUNBLE_DLLDECL extern SpecificGun VRsenal_Gun_002;
	GUNBLE_DLLDECL extern SpecificGun VRsenal_Gun_003;
	GUNBLE_DLLDECL extern SpecificGun VRsenal_Gun_004;
	GUNBLE_DLLDECL extern SpecificGun magpulGun;
	GUNBLE_DLLDECL extern SpecificGun lightGun;
	GUNBLE_DLLDECL extern SpecificGun screenGun;
	GUNBLE_DLLDECL extern SpecificGun officeBreadboard;

	GUNBLE_DLLDECL extern bool doCalibration;

	GUNBLE_DLLDECL bool __stdcall StartBluegiga(char *comPort);
	GUNBLE_DLLDECL void __stdcall StopBluegiga();
	GUNBLE_DLLDECL void __stdcall SetLogger(void(*logger)(const char *format, ...));


	// Blueprint Interface
	GUNBLE_DLLDECL void __stdcall VRsenal_GameContinue();
	GUNBLE_DLLDECL void __stdcall VRsenal_HapticEvent(wchar_t *name);
	GUNBLE_DLLDECL void __stdcall VRsenal_MatchEnd(int score, bool success, wchar_t *stats);


	// The Devices
	GUNBLE_DLLDECL void __stdcall GunFeather_Setup(SpecificGun *sg);
	GUNBLE_DLLDECL void __stdcall HapticTrinket_Setup();






#ifdef __cplusplus
}
#endif




