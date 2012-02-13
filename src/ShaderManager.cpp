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
        
    for (uint i = 0; i < mShaderEntrys.getNItems(); i++)
	{
		ShaderEntry* shader = static_cast<ShaderEntry*>(mShaderEntrys.findByIndex(i));
        DR_SAVE_DELETE(shader->shader);
        DR_SAVE_DELETE(shader);
		DRGrafikError("Fehler beim Shader löschen in: ShaderManager::Exit");
	}
	mShaderEntrys.clear(true);    
    
    LOG_INFO("ShaderManager beendet");
}

DHASH ShaderManager::makeShaderHash(const char* vertexShader, const char* fragmentShader)
{
    DHASH hash = DRMakeFilenameHash(fragmentShader);
    hash |= DRMakeFilenameHash(vertexShader)<<8;
    return hash;
}

//! lädt oder return instance auf Textur
ShaderProgram* ShaderManager::getShader(const char* vertexShader, const char* fragmentShader)
{
    if(!mInitalized) return NULL;

    DHASH id = makeShaderHash(vertexShader, fragmentShader);
    
    //Schauen ob schon vorhanden
    ShaderEntry* entry = static_cast<ShaderEntry*>(mShaderEntrys.findByHash(id));

    if(entry && entry->shader != NULL)
    {
        entry->referenzCounter++;
        return entry->shader;
    }

    entry = new ShaderEntry;
    entry->referenzCounter = 0;
	
    entry->shader = new ShaderProgram;
    const char* fragmentPath = DRFileManager::Instance().getWholePfad(fragmentShader);
    const char* vertexPath = DRFileManager::Instance().getWholePfad(vertexShader);
    DRString fragmentShaderString = fragmentShader;
    DRString vertexShaderString = vertexShader;
    if(fragmentPath)
        fragmentShaderString = DRString(DRString(fragmentPath)+"/"+DRString(fragmentShader));
    if(vertexPath)
        vertexShaderString = DRString(DRString(vertexPath)+"/"+DRString(vertexShader));

    if(entry->shader->init(vertexShaderString.data(), fragmentShaderString.data()))
        LOG_ERROR("Fehler beim laden des Shaders", NULL);
    if(!entry->shader)
    {
        DR_SAVE_DELETE(entry);
        LOG_ERROR("No Memory for new Shader left", 0);
    }
    
    if(!mShaderEntrys.addByHash(id, entry))
    {
        DR_SAVE_DELETE(entry->shader);
        DR_SAVE_DELETE(entry);
        LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShader aufgetreten", 0);
    }
    entry->referenzCounter = 1;

    return entry->shader;
}
//! reduziert reference, bei null wird Textur gelöscht und OpenGL Texture in liste eingetragen
void ShaderManager::releaseShader(const char* vertexShader, const char* fragmentShader)
{
    if(!mInitalized) return;
    
    //suchen
    DHASH id = makeShaderHash(vertexShader, fragmentShader);
    releaseShader(id);
}

void ShaderManager::releaseShader(ShaderProgram* shader)
{
    releaseShader(shader->getID());
}
void ShaderManager::releaseShader(DHASH id)
{
    ShaderEntry* entry = static_cast<ShaderEntry*>(mShaderEntrys.findByHash(id));
    if(!entry) return;

    entry->referenzCounter--;
    if(entry->referenzCounter <= 0)
    {
        DR_SAVE_DELETE(entry->shader);
        mShaderEntrys.removeByHash(id);
        DR_SAVE_DELETE(entry);
    }
}