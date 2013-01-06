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
    ShaderProgramPtr getShaderProgram(const char* vertexShader, const char* fragmentShader);
    ShaderProgramPtr getShaderProgram(ShaderProgramParameter* shaderParameter);       
    
    ShaderPtr getShader(const char* shaderName, GLenum shaderType);
private:
    ShaderManager();
            
    DHASH makeShaderHash(const char* vertexShader, const char* fragmentShader);    
    
    //DRHashList mShaderEntrys;
    std::map<DHASH, ShaderProgramPtr>               mShaderProgramEntrys;
    typedef std::pair<DHASH, ShaderProgramPtr>      SHADER_PROGRAM_ENTRY;
    std::map<DHASH, ShaderPtr>                      mShaderEntrys;
    typedef std::pair<DHASH, ShaderPtr>             SHADER_ENTRY;
    bool                                            mInitalized;
};

#endif //__SC_SHADER_MANAGER__