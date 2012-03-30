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
Klasse für vereinfachten Datei zugriff
Autor: Dario Rekowski
Datum: 16.12.06

  //Dateistruktur eigener Dateien
	//Hauptheader
	const char = "DDatei";
	unsigned long			dwDateiGesLength;
	DRReal					fFileManagerVersion;
	unsigned long			dwNumBloecke;

  //BlockHeader
  unsigned short    u16HeaderLength;
  unsigned long		u32BlockLength;
  DHASH				ID;
  int				iStrLength;
  char				acDateiname[64];	//name falls Datei alleine stehen sollte

  .
  .
  .

  const char = "DEnde";
*/

#ifndef __DR_CORE2_FILE_MANAGER__
#define __DR_CORE2_FILE_MANAGER__

struct CORE2_API DRSHauptHeader
{
    char        acIdentifierCheck[8];
    u32         ulDateiGesLegth;
    DRReal	fFileVersion;
    u32         ulNumBloecke;
};
//Header am Anfang eines Blockes
struct CORE2_API DRSBlockHeaderF
{
    u16         u16HeaderLength;
    u32         u32BlockLength;
    DHASH	ID;
    int		iStrLength;
    char* 	pcDateiname;
};

//Header für einen Block (ein System)
struct CORE2_API DRSBlockHeader
{
    int         iBlockSize;
    char        acID[14];		//BlockType
};

//----------------------------------------------------------------------------------------------------------------------
//Hauptheader
struct CORE2_API DRSMasterHeader
{
    char  	acID[14];	//Identifer (Jetzt für alle eigenen Datenformate nehmen)
    DRReal	fVersion;	//Datei Version
    int		iHeaderSize;//Größe des Headers
    int		iNumBlocks;	//Anzahl Blöcke
};


#include <list>
#define FILEMANAGEREVRSION 1.01f

class CORE2_API DRFileManager
{
public:
	~DRFileManager() {if(isInitialized()) exit();};

	// Singleton-Daten
	static DRFileManager&		Instance();
	inline static DRFileManager& getSingleton() {return Instance();};
	inline static DRFileManager* getSingletonPtr() {return &Instance();};
	static bool	isInitialized()	{return Instance().m_bInitialized;};

	//Init, erstellen der IndexList (alle vorhandene Dateien prüfen)
	DRReturn init();	//"Fremd" Daten bekommen für Typ den HASH von "NULL"

	//Zeiger auf geöffnetem File liefern, schließen und freigeben muss der User.
	DRFile* getFileByHASH(DHASH ID) const;									//Nach beenden CloseFile aufrufen nicht vergessen
//	inline DRFile* GetFileByName(DRString* pstrTyp, DRString* pstrFilename);	//Nach beenden CloseFile aufrufen nicht vergessen
	DRFile* getFileByName(const char* pcTyp,   const char*  pcFilename) const;	//Nach beenden File.close() und File delete aufrufen nicht vergessen
	void closeFile(DRFile* pFile);
	void exit();												//Freigeben der IndexList

	//Neue Datei aufnehmen
	DRReturn addFile(const char* pcFilename);

	//Laden, Speichern und kombinieren
	DRFile* startWriting(const char* pcPfadName, const char* pcFilename, const char* pcTyp, unsigned long ulBlockLength);
	DRReturn endWriting(DRFile* pOpenFile);

	//Andere Dinge
	const char* getWholePfad(const char* pcFileName);
	DRReturn	addOrdner(const char* pcPfadName);
        
        static DRReturn addFolderToFileSystem(const char* folderName);

private:
	//Struktur f�r Index List

	DRFileManager() {}
	struct SIndex
	{
		DHASH ID;
		std::string strFilename;
		u32 ulCursorInFilePos;
	};

	DRHashList			    m_IndexList;					//HASH Index List
	std::list<std::string>	m_OrdnerList;				//Liste der zu durchsuchenden Ordner
	std::list<std::string>  m_OrdnerPfadList;

	bool				m_bInitialized;


	//Unterfunktionen
//	DRReturn FillIndexFromOrdner(DRString* pstrOrdner);
	DRReturn checkFile(const char* pcFile) const;
	DRReturn newFile(const char* pcFilename);
	bool isFileOK(DRFile* pOpenFile, bool bPointerOnEnd = true);

};

#endif //__DR_CORE2_FILE_MANAGER__
