/*/*************************************************************************
 *                                                                         *
 * Core, Core-Lib for my programs, Core doesn't need any libraries	   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.                                                      *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/


#ifndef __DR_CORE2_MAIN__
#define __DR_CORE2_MAIN__

/*#ifndef _DEBUG
#define _DEBUG
#endif _DEBUG
*/

//#define _DEBUG

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/errno.h>
#endif

// standard includes
#include <stdio.h>
#include <string>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <cstdarg>
#include <cstddef>
#include <math.h>
#include <list>



/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef _WIN32
#pragma warning (disable : 4251) 
#define _CRT_SECURE_NO_WARNINGS 1
#ifdef BUILD_CORE2_DLL
    #define CORE2_API __declspec(dllexport)
#else
    #define CORE2_API __declspec(dllimport)
#endif
#else
#ifdef BUILD_CORE2_DLL
    #define CORE2_API
#else
    #define CORE2_API
#endif
#endif //_WIN32

//Makros um etwas schneller zu löschen
#define DR_SAVE_DELETE(x) {if(x){delete x; x = NULL;}}
#define DR_SAVE_DELETE_ARRAY(x) {if(x){delete [] x; x = NULL;}}
//#define DR_MAX(v1, v2) {v1 > v2 ? v1 : v2}
//#define DR_MIN(v1, v2) {v1 < v2 ? v1 : v2}
#define FMARK
#define CORE_API CORE2_API

// eigene Typen
typedef float DRReal;
#ifndef u32
typedef unsigned long u32;
#ifndef uint
typedef unsigned int uint;
#endif
#endif
#ifndef s32
typedef long s32;
#endif
#ifndef u16
typedef unsigned short u16;
#endif
#ifndef s16
typedef short s16;
#endif
#ifndef u8
typedef unsigned char u8;
#endif
#ifndef s8
typedef char s8;
#endif

#ifndef u64
typedef unsigned long long u64;
#endif // u64

typedef unsigned int HASH;
typedef unsigned long DHASH;

typedef std::string DRString;

// return struktur

enum /*CORE_API*/ DRReturn
{
	DR_OK = 0,
	DR_ERROR = 1,
	DR_ZERO_POINTER = 2,
	DR_NOT_ERROR = 3,
	DR_RENDER = 4
};

#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
 enum CORE2_API DRReturn;
#endif

//ä = \303\244
//ü = \303\274
//ö = \303\266
//ß = \303\237

// core includes (ohne log)
 #include "DRVector2i.h"
#include "DRVector2.h"
#include "DRVector3.h"
#include "DRColor.h"
#include "DRPlane.h"
#include "DRMatrix.h"
#include "DRRandom.h"
#include "DRObjekt.h"
#include "DRFrustumCulling.h"
#include "DRIni.h"
#include "DRDampingSpring.h"
//#include "DRGeometrieGenerator.h"
#include "DRHash.hpp"
#include "DRHashList.hpp"
#include "DRUtils.h"
#include "DRFile.h"
#include "DRLogger.h"
#include "DRIResource.h"
#include "DRResourcePtrHolder.h"
#include "DRResourcePtr.h"
#include "DRGameStateManager.h"

//Globale Variablen
CORE2_API extern DRLogger DRLog;

//makros für Log Benutzung
#define WRITETOLOG(str, pl) DRLog.writeToLog(str, pl);
//#define DR_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR_VOID(str) {DRLog.LOG_ERROR_INTERN(str); return;}
#define LOG_INFO(str) DRLog.LOG_INFO_INTERN(str);
#define LOG_WARNING(str) DRLog.LOG_WARNING_INTERN(str);

// core includes (mit log)
#include "DRFileManager.h"
#include "DRMemoryList.h"
#include "DRIndexReferenzHolder.h"

CORE2_API DRReturn Core2_init(const char* logFileName = "Logger.html");
CORE2_API void      Core2_exit();


#endif // __DR_CORE2_MAIN__
