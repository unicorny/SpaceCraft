
/*/*************************************************************************
 *                                                                       *
 * EngineDLL, Engine for my programs, using SDL, OpenGL, OpenAL			 *
 * Copyright (C) 2007, 2008, 2009 Dario Rekowski.						 *
 * Email: dariofrodo@gmx.de   Web: www.mathe-programme.de.tk             *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * any later version.													 *
 *																		 *
 * This program is distributed in the hope that it will be useful,		 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of		 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		 *
 * GNU General Public License for more details.							 *
 *																		 *
 * You should have received a copy of the GNU General Public License	 *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 *************************************************************************/


#ifndef __DR_ENGINE2MAIN__
#define __DR_ENGINE2MAIN__

//Export Makro
//#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
#ifdef _WIN32
    #ifndef BUILD_ENGINE_DLL
        #define ENGINE_API __declspec(dllimport)
    #else
        #define ENGINE_API __declspec(dllexport)
    #endif
#else //_WIN32
    #ifdef BUILD_ENGINE_DLL
        #define ENGINE_API
    #else
        #define ENGINE_API
    #endif
#endif //_WIN32

#define XWIDTH g_v2WindowLength.x
#define YHEIGHT g_v2WindowLength.y
#define GRADTORAD 0.017453292f
#define RADTOGRAD 57.29577951f
const float PI = 3.1415926535f;

/*#define LOG_SOUND_INTERN(text, f, l, fu) DRLog.WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#F8900\">OpenAL Fehler:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, f, l, fu)
#define LOG_ERROR_SOUND(str, r) {if(DRSoundError(str, DRRemoveDir(__FILE__), __LINE__, DR_FUNCTION_)) return r;}
#define LOG_WARNING_SOUND(str) {DRSoundError(str, DRRemoveDir(__FILE__), __LINE__, DR_FUNCTION_);}

#define LOG_LUA_INTERN(text, luaText) DRLog.WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF7000\">LUA Fehler:</font></b> %s, lua: %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, luaText, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)
#define LOG_ERROR_LUA(t, r){const char* text = lua_tostring(DRLoadFromLua::getSingleton().getLuaState(), -1); if(!lua_isstring(DRLoadFromLua::getSingleton().getLuaState(), -1)) text = "nichts"; lua_pop(DRLoadFromLua::getSingleton().getLuaState(), 1); LOG_LUA_INTERN(t, text); return r;}
#define LOG_ERROR_LUA_VOID(t) {const char* text = lua_tostring(DRLoadFromLua::getSingleton().getLuaState(), -1); if(!lua_isstring(DRLoadFromLua::getSingleton().getLuaState(), -1)) text = "nichts"; lua_pop(DRLoadFromLua::getSingleton().getLuaState(), 1); LOG_LUA_INTERN(t, text); return;}

*/
#define LOG_SDL_INTERN(text, f, l, fu) DRLog.writeToLog("<tr><td><font size=\"2\"><b><font color=\"#BF8000\">SDL Fehler:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, f, l, fu)
#define LOG_ERROR_SDL(r) {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2){ LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__); return r;}}
#define LOG_ERROR_SDL_VOID() {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2){ LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__); return;}}
#define LOG_WARNING_SDL() {const char* pcErrorSDL = SDL_GetError(); if(strlen(pcErrorSDL) > 2) LOG_SDL_INTERN(pcErrorSDL, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__);}


#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif //_WIN32

//----------------------------------------------------------------------------------------------------------------------
//Externe Header
//Standard Bibliotheken

//Engines
//einbinden der Simple Direct Layer Bibliothek
#include <sdl/sdl.h>

//einbinden von OpenGL
//#include <GL/glu.h>   // Damit kann Glu32 genutzt werden.
#define GLEW_STATIC
#include "glew.h"
#define NO_SDL_GLEXT
#include <sdl/SDL_opengl.h>
#include <sdl/SDL_thread.h>

// ---------------------------------------------------------------------------------------------------
// in SDL 1.3 funtkioniert das keyboard noch nicht, daher werden hier manuell alle tasten belegt
#if SDL_VERSION_ATLEAST(1,3,0)
#define SDLK_a 4 
#define SDLK_d 7 
#define SDLK_e 8
#define SDLK_q 20
#define SDLK_s 22
#define SDLK_w 26
#define SDLK_1 30
#define SDLK_2 31
#define SDLK_3 32
#define SDLK_4 33
#define SDLK_5 34
#define SDLK_6 35
#define SDLK_7 36
#define SDLK_8 37
#define SDLK_9 38
#define SDLK_PAGEUP 75
#define SDLK_PAGEDOWN 78 
#define SDLK_RIGHT 79
#define SDLK_LEFT 80
#define SDLK_DOWN 81
#define SDLK_UP 82	
#define SDLK_MINUS 86
#define SDLK_PLUS 87
#endif

//----------------------------------------------------------------------------------------------------------------------
//Eigene Header
//#define LOGGING
#include <Core2/Core2Main.h>	//Meine Core.dll

//----------------------------------------------------------------------------------------------------------------------
//Interne Header
//#include "glExtensions.h"
#include "DRInterface.h"
#include "DRINetwork.h"
#include "DRIvlc.h"
#include "DRIImage.h"
#include "DRRandom.h"
#include "DRVideoConfig.h"
#include "DRTextur.h"
#include "DRGeometrie.h"
#include "DRGeometrieSphere.h"
#include "DRGeometrieIcoSphere.h"
#include "DRText.h"

/*#include "FileFormats.h"
#include "Utils.h"
#include "Font.h"
#include "TextM.h"
#include "SoundManager.h"
#include "LoadFromLua.h"
#include "TextureM.h"
#include "SpriteM.h"
#include "Starfield.h"
#include "glExtension.h"
#include "VertexBuffer.h"
#include "Engine_Lua.h"
*/
//----------------------------------------------------------------------------------------------------------------------
ENGINE_API extern SDL_Surface* g_pSDLWindow;
ENGINE_API extern DRVector2  g_v2WindowLength;
ENGINE_API extern Uint8*    g_piPressed;
ENGINE_API extern u16       g_CPU_Count;

/**	Engine Funktionen
	Verschieden Init-Funktionen und anderes
	Init alleine f�r SDL
	f�r SDL und OpenGL
	f�r SDL und Ogre3D
*/
ENGINE_API DRReturn EnInit_Simple(DRReal fVersion = 1.0f, bool initSound = false);
ENGINE_API DRReturn EnInit(DRReal fVersion = 1.0f, bool initSound = false);
ENGINE_API DRReturn EnInit_INI(const char* iniFileName);
ENGINE_API DRReturn EnInit_OpenGL(DRReal fVersion = 1.0f, DRVideoConfig video = DRVideoConfig(), const char* pcTitel = "OpenGL Render Fenster", const char* pcBMPIcon = NULL, bool bInitSound = true);
ENGINE_API DRReturn	EnGameLoop(DRReturn (*pMoveProc)(DRReal), DRReturn (*pRenderProc)(DRReal), bool bExitByEsc = true);
ENGINE_API void     EnExit();

ENGINE_API void		EnPostExitMessageToSDL();
ENGINE_API DRReal	EnSDL_Loop();
ENGINE_API bool		EnIsButtonPressed(SDLKey button);

//----------------------------------------------------------------------------------------------------------------------
// Ein paar Helferfunktionen
//! Fragt nach einem Fehler in OpenGL
ENGINE_API DRReturn DRGrafikError(const char* pcErrorMessage);

#endif //__DR_ENGINE2MAIN__
