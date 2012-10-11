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
        
	mShaderEntrys.clear();    
    
    LOG_INFO("ShaderManager beendet");
}

DHASH ShaderManager::makeShaderHash(const char* vertexShader, const char* fragmentShader)
{
    DHASH hash = DRMakeFilenameHash(fragmentShader);
    hash |= DRMakeFilenameHash(vertexShader)<<8;
    return hash;
}

ShaderProgramPtr ShaderManager::getShader(ShaderProgramParameter* shaderParameter)
{
    return getShader(shaderParameter->vertexShaderName.data(), shaderParameter->fragmentShaderName.data());
}

//! lädt oder return instance auf Textur
ShaderProgramPtr ShaderManager::getShader(const char* vertexShader, const char* fragmentShader)
{
    if(!mInitalized) return NULL;
    
    DHASH id = makeShaderHash(vertexShader, fragmentShader);
    
    //Schauen ob schon vorhanden
    if(mShaderEntrys.find(id) != mShaderEntrys.end())
    {
        return mShaderEntrys[id];
    }
    
    DREngineLog.writeToLog("start loading shader (%s, %s)!", vertexShader, fragmentShader);
    
    ShaderProgramPtr shader(new ShaderProgram(id));
    
    const char* fragmentPath = DRFileManager::Instance().getWholePfad(fragmentShader);
    const char* vertexPath = DRFileManager::Instance().getWholePfad(vertexShader);
    DRString fragmentShaderString = fragmentShader;
    DRString vertexShaderString = vertexShader;
    if(fragmentPath)
        fragmentShaderString = DRString(DRString(fragmentPath)+"/"+DRString(fragmentShader));
    if(vertexPath)
        vertexShaderString = DRString(DRString(vertexPath)+"/"+DRString(vertexShader));
    
    if(!shader.getResourcePtrHolder()->mResource)
        LOG_ERROR("No Memory for new shader left", 0);
    
    if(shader->init(vertexShaderString.data(), fragmentShaderString.data()))
        LOG_ERROR("error loading shader", NULL);
        
    if(!mShaderEntrys.insert(SHADER_PROGRAM_ENTRY(id, shader)).second)
    {
        LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShader aufgetreten", 0);
    }
    
    DREngineLog.writeToLog("end loading shader!");
    return shader;
}
