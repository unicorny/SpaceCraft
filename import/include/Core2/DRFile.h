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

/*
File Klasse von Dario Rekowski
Zum laden, speichern und erstellen von Datein

  */
#ifndef __DR_CORE2_FILE__
#define __DR_CORE2_FILE__


//***************************************************************************************


//Fehlercodes
enum DRFileErrorCodes
{
	File_OK = 0,
	File_error_cannot_create_File = 1,
	File_error_file_is_not_open = 2,
	File_error_could_not_write_all = 3,
	File_error_could_not_read_all = 4,
	File_error_NullPointer = 5,
	File_error_pointer_distance_to_great = 6,
	File_error_could_not_set_pointer = 7,
	File_error_could_not_get_pointer = 8,
	File_error_File_didnt_exist = 9
};

//***********************************************************************************************+
// TODO: File Buffer einbauen 
class CORE2_API DRFile
{
public:
	//Konstruktor, Kopierkonstruktoren und Deskonstruktor
	DRFile();
#ifdef _WIN32
	DRFile(const wchar_t* pstFilename, const wchar_t* pstMode = L"a+b");
#endif
	DRFile(const char* pcFilename, const char* pcMode = "a+b");
	DRFile(const DRFile& otherFile);
	~DRFile();

	//�ffnen einer Datei
#ifdef _WIN32
	DRFileErrorCodes open(const wchar_t* pstFilename, const wchar_t* pstMode = L"a+b");
#endif
	DRFileErrorCodes open(const char* pcFilename, bool bForceNew = false, const char* pcMode = "a+b");

	//schlie�en eine Datei
	void close();

	//Pr�fen, ob die Datei ge�ffnet ist
	bool isOpen();
        
	//! \brief Gr��e der Datei
	//! \return 0 bei Error, sonst Dateigr��e
	unsigned long getSize();

	//Daten in die ge�ffnete Datei schreiben
	DRFileErrorCodes write(const void* pDatenIn, unsigned long ulSize, unsigned long ulCount, unsigned long* plWrittenBytesOut = NULL);

	//Daten aus datei lesen
	DRFileErrorCodes read(void* pDatenOut, unsigned long ulSize, unsigned long ulCount, unsigned long* plReadedBytesOut = NULL);

	//Zuweisungsoperator
	const DRFile& operator = (const DRFile& otherFile) {mFile = otherFile.mFile; return *this;}

	//Funktionen um per FilePointer zu navigieren

	//Aktuelle Position abfragen
	unsigned long getFilePointer();

	//File Pointer setzen
	//SEEK_CUR = Current Position, SEEK_SET = Begin, SEEK_END = Ende
	DRFileErrorCodes setFilePointer(s32 ulDistance, unsigned long ulStartPoint = SEEK_CUR);

	inline FILE* getFile() {return mFile;};

private:
	//Variablen

	//Zeiger auf die Datei
	//void*		m_pFile;
	FILE*		mFile;
	long		mFileSize;


};

CORE2_API bool DRIsFileExist(const char* pcFilename);

#endif //__DR_CORE2_FILE__
