/* 
 * File:   RenderBlockLoader.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 23:50
 */

#include "RenderBlockLoader.h"

RenderBlockLoader::RenderBlockLoader() 
: mRenderBlocks(NULL), mCfg(NULL)
{
    mRenderBlocks = new DRHashList;
}

RenderBlockLoader::RenderBlockLoader(const RenderBlockLoader& orig) 
{
    LOG_WARNING("not implentet yet");
}

RenderBlockLoader::~RenderBlockLoader() 
{
    if(mRenderBlocks->getNItems() > 0)
        printf("RenderBlockLoader::~RenderBlockLoader mRenderBlocks isn't empty\n");
    mRenderBlocks->clear(true);
    DR_SAVE_DELETE(mRenderBlocks);
}

RenderBlock* RenderBlockLoader::getRenderBlock(BlockName name)
{
    RenderBlock* rb = NULL;
    GLint filter = GL_LINEAR;
    if(rb = (RenderBlock*)mRenderBlocks->findByHash(name))
    {
        return rb;
    }
   // DRLog.writeToLog("try to load block: %c%c%c%c", name.zeichen[0], name.zeichen[1], name.zeichen[2], name.zeichen[3]);
    char buffer[5];
    memcpy(buffer, name.zeichen, 4);
    buffer[4] = '\0';
    DRString* strFilter = mCfg->getStr(buffer, "filter");
    if(*strFilter == DRString("nearest"))
        filter = GL_NEAREST;
        
    DRTexturePtr tex = DRTextureManager::Instance().getTexture(mCfg->getStr(buffer, "texture")->data(), false, filter, filter);
  //  DRLog.writeToLog("TexturPointer: %d", (unsigned int)tex);
    rb = new RenderBlock(tex, name, mCfg);
    mRenderBlocks->addByHash(name, rb);    
    return rb;
}

DRReturn RenderBlockLoader::init()
{
    mCfg = new DRIni("data/blockView/setup.ini");
    return DR_OK;
}

void RenderBlockLoader::exit()
{
    for(u32 i = 0; i < mRenderBlocks->getNItems(); i++)
    {
        RenderBlock* rb = (RenderBlock*)mRenderBlocks->findByIndex(i);
        DR_SAVE_DELETE(rb);
    }
    mRenderBlocks->clear(false);
       
}

