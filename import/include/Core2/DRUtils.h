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
Hilfsfunktionen für die Arbeit mit strings
von Dario Rekowski
*/
#ifndef __DR_UTILS__
#define __DR_UTILS__

//- - -- - - - - - - - - -- - - - - - - - - - - - -- -  - - - - - - - - - - - - - -
//liefert die nächste 2er Potenz zurück
CORE2_API int DRGetNextPotenz(int iIn);

// DRGetNumChar = strlen

//Sagt ob ein Zeichen ein Großbuchstabe ist, doer nicht

//CORE2_API bool DRisGrosB(char zeichen); = isUppercase();

//Sagt ob ein Zeichen eine Zahl ist (oder , und hochzeichen)
CORE2_API bool DRisMathChar(char czeichen);

//Liefert den Term vor oder nach cTrenn zurück
CORE2_API DRReturn DRGetTermVorChar(DRString* sInput, DRString* sOutput, char cTrenn);
CORE2_API DRReturn DRGetTermNachChar(DRString* sInput, DRString* sOutput, char cTrenn);

//Wandelt einen String in eine Zahl um
//CORE2_API double DRMakeNumberFromString(DRString* sInput);
//atof oder atoi

//Zähle Zeichen eines Types in einem String
CORE2_API int DRCountCharsOf(DRString* sInput, char cCountable);

//Zähle Zahlen in einem String
CORE2_API int DRCountNumbers(DRString* sInput);

//Sagt ob ein zeichen eine Zahl ist
CORE2_API bool DRisNumber(char cZeichen);

//Entfernt HTML Tags
CORE2_API DRReturn DRRemoveHTMLTags(char* pcIn, char* pcOut, u32 dwOutLength);

//Entfernen einer Pfadangabe aus einem Dateinamen
CORE2_API const char* DRRemoveDir(const char* pcFilename);

//reserviert Speicher in String Größe, kopiert übergebenden String rein und gibt Zeiger auf neues char array zurück
//CORE2_API char* DRCopyString(const char* pcString);
//strcpy

//überprüft, ob ein String nach dem letzten Punkt, exakt das gleiche hat, wie im String pcEndung
CORE2_API bool DRCheckEndung(const char* pcInput, const char* pcEndung);

/*//Liefert den Pfad, also alles vor dem letzten / oder \\*/
CORE2_API DRReturn DRGetPfad(const char* pcInput, char* pcOutput, int iOutBufferSize = 256);

/*
CORE2_API DRVector2	DRVector2TransformCoords(const DRVector2& v, const DRMatrix& m, DRReal* const pfOutW = NULL);																																																					// 2D-Vektor mit Matrix multiplizieren
CORE2_API DRVector2	DRVector2TransformNormal(const DRVector2& v, const DRMatrix& m);																																																												// 2D-Normalenvektor mit Matrix multiplizieren
CORE2_API DRVector2	DRVector2TransformNormal_TranspInv(const DRVector2& v, const DRMatrix& m);																																																										// 2D-Normalenvektor mit Matrix multiplizieren
CORE2_API DRVector3	DRVector3TransformCoords(const DRVector3& v, const DRMatrix& m, DRReal* const pfOutW = NULL);																																																					// 3D-Vektor mit Matrix multiplizieren
CORE2_API DRVector3	DRVector3TransformNormal(const DRVector3& v, const DRMatrix& m);																																																												// 3D-Normalenvektor mit Matrix multiplizieren
CORE2_API DRVector3	DRVector3TransformNormal_TranspInv(const DRVector3& v, const DRMatrix& m);																																																										// 3D-Normalenvektor mit Matrix multiplizieren
*/


//CORE2_API bool DRIfPointInQuad(DRVector3 vPoint, DRVector3 vMin, DRVector3 vMax);

#endif // __DR_UTILS__
