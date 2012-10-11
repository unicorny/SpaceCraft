#ifndef __SC_SHADER_MANAGER__
#define __SC_SHADER_MANAGER__

#include "main.h"
#include "ShaderProgram.h"

class ShaderManager
{
public:
    ~ShaderManager() {if(isInitialized()) exit();};

    // Singleton-Daten
    static ShaderManager&		Instance();
    inline static ShaderManager& getSingleton() {return Instance();};
    inline static ShaderManager* getSingletonPtr() {return &Instance();};
    static bool	isInitialized()	{return Instance().mInitalized;};
    
    DRReturn init();
    
    void exit();
    
    //! lädt oder return instance auf Textur
    ShaderProgramPtr getShader(const char* vertexShader, const char* fragmentShader);
    ShaderProgramPtr getShader(ShaderProgramParameter* shaderParameter);
       
    
private:
    ShaderManager();
            
    DHASH makeShaderHash(const char* vertexShader, const char* fragmentShader);    
    
    //DRHashList mShaderEntrys;
    std::map<DHASH, ShaderProgramPtr>               mShaderEntrys;
    typedef std::pair<DHASH, ShaderProgramPtr>      SHADER_PROGRAM_ENTRY;
    bool                                            mInitalized;
};

#endif //__SC_SHADER_MANAGER__