
#pragma once


#define GUTILS_DLL_DECL 


/**********************************************************************/
/* Header Files                                                       */
/**********************************************************************/

#define DIRECTINPUT_VERSION         0x0800

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <Tlhelp32.h>



/**********************************************************************/
/* Macro Definitions                                                  */
/**********************************************************************/

#define TWO_BITS	0x3
#define THREE_BITS	0x7
#define FOUR_BITS	0xF /* all 4-bits of a NIBBLE are on, which is the hex value of "F" */
#define FIVE_BITS	0x1F


#define CLASS_DECL(x)	typedef struct x##_st x;
#define CLASS_DEF(x)	struct x##_st
#define CLASS(x)		CLASS_DECL(x) CLASS_DEF(x)

#define NEW_FUNC(className)				_new_##className
#define FREE_FUNC(className)			_free_##className

#define NEW(className)				NEW_FUNC(className)()
#define FREE(className, obj)		FREE_FUNC(className)(obj)


#define CLASS_CTOR_PROTO(className)	className *NEW_FUNC(className)();
#define CLASS_CTOR_START(className)	className *NEW_FUNC(className)() { className *obj = (className *)malloc(sizeof(className));
#define CLASS_CTOR_END				return obj; }
#define CLASS_CTOR(className)		CLASS_CTOR_START(className)	CLASS_CTOR_END

#define CLASS_FREE_PROTO(className)	void FREE_FUNC(className)(className *obj);
#define CLASS_FREE_START(className)	void FREE_FUNC(className)(className *obj) { 
#define CLASS_FREE_END				free(obj); }
#define CLASS_FREE(className)		CLASS_FREE_START(className)	CLASS_FREE_END

#ifndef skip_spaces
#define skip_spaces(str)            { while ( isspace(*str) ) str++; }
#endif

#define ENDSTR(str)             (str+strlen(str))



#define SCREEN_WIDTH	1920
#define SCREEN_HEIGHT	1200

#define Rad2Deg 57.2957795785523f/* (180.0 / PI) */


#define NULL_FLOAT	-5.01894f
#define AMPILFY	(7.702f * 2.0f)
extern float l_pitch;
extern float l_yaw;
extern float l_roll;
extern float d_yaw;
extern float d_pitch;

extern float ax;
extern float ay;

extern POINT cursor;


extern double heading;		// set in main.cpp's ArmA log-watcher loop.
extern double weaponAngle;



void Init();

extern bool switched_imu;
void imu_to_mouseMove(float pitch, float yaw, float roll);

void SetMouseAcceleration(BOOL mouseAccel);
void MoveMouse(int dx, int dy);
void MoveMouseAbs(int x, int y);

class DirectXScanCodes
{
    public:
        DirectXScanCodes(void);
		void KeyDown(char ch);
		void KeyUp(char ch);
    private:
        INPUT input;
};
char DirectXCode(int ch);

void ClickMouse();

void eulerAngles(double w, double x, double y, double z, float* pitchPtr, float* rollPtr, float* yawPtr) ;

extern LARGE_INTEGER Frequency;
extern LARGE_INTEGER LogBeginTime;

void ResetBeginTime();
int LogMS();


#include <string>
#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
//using namespace std;



bool ListFiles(std::string path, std::string mask, std::vector<std::string>& files);
void ClearDirFiles(char *dir, char *filePatternMatch);
void DisplayError(LPTSTR lpszFunction);

char *slurpText(const char* filename);

int a2hex(int c);
int hex2a(int h);
char *hex_string(unsigned char *data, int len, char *outputString);

void runf(char *cmdFormat, ...);



typedef struct Tuple_st
{
	long long Item1;
	long long Item2;
} Tuple;
bool Intersection(Tuple *a, Tuple *b, Tuple *overlap);

float AxisSubtract(float minuend, float subtrahend);
float AxisAdd(float a, float b);

void MakeAxisString(char *xStr, uint8_t joystickX1);

int round_int(double r);

