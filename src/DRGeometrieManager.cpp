#include "DRGeometrieManager.h"

DRGeometrieManager::DRGeometrieManager()
: mInitalized(false)
{
    
}

DRGeometrieManager& DRGeometrieManager::Instance()
{
    static DRGeometrieManager TheOneAndOnly;
    return TheOneAndOnly;
}

DRReturn DRGeometrieManager::init()
{
    mInitalized = true;
    LOG_INFO("DRGeometrieManager initalisiert");
    return DR_OK;
}

void DRGeometrieManager::exit()
{
    mInitalized = false;    
    for (int i = 0; i < mGeometrieEntrys.getNItems(); i++)
	{
		GeometrieEntry* geometrie = static_cast<GeometrieEntry*>(mGeometrieEntrys.findByIndex(i));
        DR_SAVE_DELETE(geometrie->geometrie);
        DR_SAVE_DELETE(geometrie);
	}
	mGeometrieEntrys.clear(true);    
    LOG_INFO("DRGeometrieManager beendet");
}

DHASH DRGeometrieManager::makeGeometrieHash(GLuint gridSize, GeometrieGridFormat format)
{
    DHASH id = gridSize | format<<4 | DRMakeStringHash("grid")>>9;
    return id;    
}

DRGeometrieHeightfield* DRGeometrieManager::getGrid(GLuint gridSize, GeometrieGridFormat gridFormat)
{
    if(!mInitalized) return 0;
    DHASH id = makeGeometrieHash(gridSize, gridFormat);

	//Schauen ob schon vorhanden
	GeometrieEntry* entry = static_cast<GeometrieEntry*>(mGeometrieEntrys.findByHash(id));
	
	if(entry && entry->geometrie != NULL)
	{
		entry->referenzCounter++;
		return dynamic_cast<DRGeometrieHeightfield*>(entry->geometrie);
	}

	entry = new GeometrieEntry;
	entry->referenzCounter = 0;
    
    DRVector3 edges[4];
    edges[0] = DRVector3(1.0f, 1.0f, 1.0f);
    edges[1] = DRVector3(-1.0f, 1.0f, 1.0f);
    edges[2] = DRVector3(1.0f, -1.0f, 1.0f);
    edges[3] = DRVector3(-1.0f, -1.0f, 1.0f);
    entry->geometrie = new DRGeometrieHeightfield();
    dynamic_cast<DRGeometrieHeightfield*>(entry->geometrie)->init(gridSize, edges, gridFormat);
    
    if(!mGeometrieEntrys.addByHash(id, entry))
	{
        DR_SAVE_DELETE(entry->geometrie);
		DR_SAVE_DELETE(entry);
		LOG_ERROR("Unerwarteter Fehler in DRGeometrieManager::getGrid aufgetreten", 0);
	}
	entry->referenzCounter = 1;

	return dynamic_cast<DRGeometrieHeightfield*>(entry->geometrie);

    return NULL;
}