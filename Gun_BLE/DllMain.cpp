

#include "dllmain.h"






char* log_data;



void Unity_Log(char* message)
{

	log_data = message;

}

// Function definitions:
extern "C"
{
//	GUNBLE_DLLDECL FuncPtr Log;


	


	void __stdcall VRsenal_RumbleOff()
	{
		Log("VRsenal_RumbleOff");
		Unity_Log("VRsenal_RumbleOff");
	}




	void __stdcall VRsenal_InitHaptic(char *audioDeviceName)
	{
		Log("VRsenal_InitHaptic(%s)", audioDeviceName);
	

		char buffer[150];

		snprintf(buffer, 100, "VRsenal_InitHaptic(%s)", audioDeviceName);


		Unity_Log(buffer);


	}


	bool __stdcall VRsenal_Init(char *comPort, void(*logger)(const char *format, ...), int gunNbr)
	{
		return VRsenal_Init2(comPort, logger, NULL);
	}




	bool __stdcall VRsenal_Init2(char *comPort, void(*logger)(const char *format, ...), SpecificGun *g)
	{
		if (logger == NULL)
			dll_logger = null_logger;
		else
			dll_logger = logger;

		// ----------------------------------------------------------------------
		Log("Opening Bluegiga's BLED112 & connecting to Gun Peripheral...");

		Unity_Log("Opening Bluegiga's BLED112 & connecting to Gun Peripheral...");



		return true;
	}
	void __stdcall VRsenal_Shutdown()
	{
		Log("VRsenal_Shutdown");

		Unity_Log("VRsenal_Shutdown");

	}


	void __stdcall SetLogger(void(*logger)(const char *format, ...))
	{
		if (logger == NULL)
			dll_logger = null_logger;
		else
			dll_logger = logger;
	}



	bool __stdcall VRsenal_RetryInit()
	{
		return true;
	}
	void __stdcall VRsenal_DisconnectFromBluetooth()
	{
		Log("VRsenal_DisconnectFromBluetooth");

		Unity_Log("VRsenal_DisconnectFromBluetooth");


	}
	void __stdcall VRsenal_CloseCOMPort()
	{
		Log("VRsenal_CloseCOMPort");

		Unity_Log("VRsenal_CloseCOMPort");

	}






	bool __stdcall VRsenal_Update(Joystick &lowerLeft, Joystick &lowerRight,
		Joystick &upperLeft,
		Joystick &upperRight,
		bool &trigger,
		GameStartAt &startAt,
		bool &requestShutdown)
	{
		Log("VRsenal_Update");

		startAt.level = NO_START_AT;
		requestShutdown = false;

		lowerLeft.selectButton = lowerRight.selectButton = upperLeft.selectButton = upperRight.selectButton = false;
		lowerLeft.X = lowerRight.X = upperLeft.X = upperRight.X = 0.0f;
		lowerLeft.Y = lowerRight.Y = upperLeft.Y = upperRight.Y = 0.0f;
		trigger = false;

		return true;
	}


	





		int __stdcall GetSimpleInt(int	n)
	{
		return		2 + n;
	}



		int x = 100;



	void __stdcall VRsenal_RumbleOn()
	{
		Log("VRsenal_RumbleOn");


		Unity_Log("VRsenal_RumbleOn");


	//	Debug("VRsenal_RumbleOn", "fds");

	}
	
	



	GUNBLE_DLLDECL void DoWork(UpdateCallback progressCallback)
	{


			if (progressCallback)
			{
				x++;
				// send progress update
				progressCallback(x);
			}
		
		

	}



	

	GUNBLE_DLLDECL void DoLog(LoggerCallback loggerCallback)
	{


		if (loggerCallback)
		{
		
			// send progress update
			loggerCallback(log_data);
	
			log_data = "";

		}



	}












	GUNBLE_DLLDECL void SetDebugFunction(FuncPtr fp)
	{
		Log = fp;
	}

	 


	void __stdcall VRsenal_GameContinue()
	{
		Log("VRsenal_GameContinue");

		Unity_Log("VRsenal_GameContinue");


	//	Debug("VRsenal_GameContinue");

	}
	void __stdcall VRsenal_HapticEvent(wchar_t *name)
	{

		char buffer[100];

		//sprintf_s(buffer, "VRsenal_HapticEvent(%S)", name);

		snprintf(buffer, 100, "VRsenal_HapticEvent(%S)", name);

	//	Log("VRsenal_HapticEvent");


		Log("VRsenal_HapticEvent(%S)", name);

		Unity_Log(buffer);


	}
	void __stdcall VRsenal_MatchEnd(int score, bool success, wchar_t *stats)
	{
		Log("VRsenal_MatchEnd(score=%d, success=%c, stats=[%S])", score, success ? 'Y' : 'N', stats);
	}



}