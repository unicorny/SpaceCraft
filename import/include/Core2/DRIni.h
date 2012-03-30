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

#ifndef __DR_CORE2_INI__
#define __DR_CORE2_INI__

/*
class to read from ini Files
(without speziell windows functiones)
working without core init

  Dario Rekowski
  */

#include <map>
#include <string>


class CORE_API DRIni
{
public:
	explicit DRIni(const char* fileName);
	~DRIni();

	DRVector3 getVector3(const char* section, const char* key);
	DRVector2 getVector2(const char* section, const char* key);
	DRColor   getColor(const char* section, const char* key);
	int getInt(const char* section, const char* key);
	DRReal getReal(const char* section, const char* key);
	std::string* getStr(const char* section, const char* key);

	inline bool isValid() {return mValid;}
private:
	FILE* mFilePointer;
	bool  mValid;
	std::string mTemp;
	typedef std::map<std::string, std::string> EntryMap;
	typedef std::pair<std::string, std::string> STRINGENTRY;
	typedef std::pair<std::string, EntryMap> MAPENTRY;
	std::map<std::string, EntryMap> mKeyMap;


	void readEntry();
	void readParameters(std::string _name, std::string line);

};


#endif //__DR_CORE2_INI__

