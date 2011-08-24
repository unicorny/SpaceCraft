#ifndef __CORE_INI__
#define __CORE_INI__

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


#endif //__CORE_INI__

