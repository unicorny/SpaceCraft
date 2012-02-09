/*/*************************************************************************
 *                                                                       *
 * Core, Core-Lib for my programs, Core doesn't need any libraries		 *
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

/*
Logging System �hnlich dem der TriBase-Engine von David Scherfgen
Programmierer: Dario Rekowski
*/
//Hier kommt alles rein, was mit Fehlerbehandlung zu tun hat

//Eine Aufz�hlung f�r R�ckgabewerte f�r die meisten Funktionen
//k�nnen wir je nach dem noch erweitern
#ifndef __DR_LOGGER__
#define __DR_LOGGER__

//Vorw�rtsdeklaarionen f�r den Logger
class DRVector2;
class DRVector3;
class DRColor;
class DRMatrix;

#if _MSC_VER < 1300
	#define DR_FUNCTION_ ("(Unbekannt)")
#else
	#define DR_FUNCTION_ (__FUNCTION__)
#endif

//Klasse f�r LogDatei wo alle Fehler reingeschrieben werden k�nnen
class CORE2_API DRLogger
{

private:
	//Membervariablen
	DRFile m_File;			//Log-File
	char  m_acFilename[256];//Dateinamen des Log-Files

public:
	//Und die �ffentlichen Funktionen
	DRLogger();
	~DRLogger();

	DRReturn init(const char* pcFilename);
	void exit();

	//in die Log-Datei schreiben
	DRReturn writeToLog(const char* pcText, ...);
        DRReturn writeToLog(DRString text);
	DRReturn writeToLogDirect(const char* pcText, ...);

	DRReturn writeVector2ToLog(const DRVector2& v, const char* name = NULL);			// 2D-Vektor ins Logbuch schreiben
    DRReturn writeVector3ToLog(const DRVector3& v, const char* pcName = NULL);			// 3D-Vektor ins Logbuch schreiben
    DRReturn writeMatrixToLog(const DRMatrix& m, const char* name = NULL);			// Matrix ins Logbuch schreiben
	//DRReturn WritePlaneToLog(DRPlane& p);				// Ebene ins Logbuch schreiben
	DRReturn writeColorToLog(const DRColor& c);				// Farbe ins Logbuch schreiben


	//inline um Variablen abzufragen
	inline DRFile* getFile() {return &m_File;};

	//Funktionszeiger f�r SDL Mutex bei verwendung von SDL
	void (*mLockMutex)(void);
	void (*mUnlockMutex)(void);
	void*	mMutex;


//	DRReturn WriteDXError(HRESULT hr);


	//Info Text in Logbuch Datei schreiben

	#define LOG_INFO_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#008000\">INFO:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text,  DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)
	// Fehlertext in Logbuchdatei schreiben
	#define LOG_ERROR_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text,  DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)
	// Warnungstext in Logbuchdatei schreiben
	#define LOG_WARNING_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">WARNUNG:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)

};

#endif //__DR_LOGGER__
