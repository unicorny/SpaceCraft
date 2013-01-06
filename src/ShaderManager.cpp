#include "ShaderManager.h"

ShaderManager::ShaderManager()
: mInitalized(false)
{
    
}

ShaderManager& ShaderManager::Instance()
{
    static ShaderManager TheOneAndOnly;
    return TheOneAndOnly;
}

DRReturn ShaderManager::init()
{
    mInitalized = true;
    LOG_INFO("ShaderManager initalisiert");
    return DR_OK;
}

void ShaderManager::exit()
{
    mInitalized = false;
        
	mShaderProgramEntrys.clear();    
    
    LOG_INFO("ShaderManager beendet");
}

DHASH ShaderManager::makeShaderHash(const char* vertexShader, const char* fragmentShader)
{
    DHASH hash = DRMakeFilenameHash(fragmentShader);
    hash |= DRMakeFilenameHash(vertexShader)<<8;
    return hash;
}

ShaderProgramPtr ShaderManager::getShaderProgram(ShaderProgramParameter* shaderParameter)
{
    return getShaderProgram(shaderParameter->vertexShaderName.data(), shaderParameter->fragmentShaderName.data());
}

//! lädt oder return instance auf Textur
ShaderProgramPtr ShaderManager::getShaderProgram(const char* vertexShader, const char* fragmentShader)
{
    if(!mInitalized) return NULL;
    
    DHASH id = makeShaderHash(vertexShader, fragmentShader);
    
    //Schauen ob schon vorhanden
    if(mShaderProgramEntrys.find(id) != mShaderProgramEntrys.end())
    {
        return mShaderProgramEntrys[id];
    }
    
    ShaderProgramPtr shaderProgram(new ShaderProgram(id));    
    if(shaderProgram->init(getShader(vertexShader, GL_VERTEX_SHADER), getShader(fragmentShader, GL_FRAGMENT_SHADER)))
        LOG_ERROR("error loading shader program", NULL);
        
    if(!mShaderProgramEntrys.insert(SHADER_PROGRAM_ENTRY(id, shaderProgram)).second)
    {
        LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShaderProgram aufgetreten", 0);
    }
    
    return shaderProgram;
}

ShaderPtr ShaderManager::getShader(const char* shaderName, GLenum shaderType)
{
    if(!mInitalized) return NULL;
    
    DHASH id = DRMakeFilenameHash(shaderName);
    
    //Schauen ob schon vorhanden
    if(mShaderEntrys.find(id) != mShaderEntrys.end())
    {
        return mShaderEntrys[id];
    }
    
    DREngineLog.writeToLog("[ShaderManager::getShader] start loading shader (%s)!", shaderName);
    
    ShaderPtr shader(new Shader(id));
    
    const char* path = DRFileManager::Instance().getWholePfad(shaderName);
    DRString shaderString = shaderName;
    if(path)
        shaderString = DRString(DRString(path)+"/"+DRString(shaderName));
        
    if(!shader.getResourcePtrHolder()->mResource)
        LOG_ERROR("No Memory for new shader left", 0);
    
    if(shader->init(shaderString.data(), shaderType))
        LOG_ERROR("error loading shader", NULL);
        
    if(!mShaderEntrys.insert(SHADER_ENTRY(id, shader)).second)
    {
        LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShader aufgetreten", 0);
    }
    
    DREngineLog.writeToLog("[ShaderManager::getShader] end loading shader!");
    return shader;
}