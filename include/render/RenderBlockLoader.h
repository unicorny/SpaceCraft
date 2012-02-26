/* 
 * File:   RenderBlockLoader.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:50
 */

#ifndef __SC_RENDER_BLOCK_LOADER_H
#define	__SC_RENDER_BLOCK_LOADER_H

//#include "main.h"
#include "RenderBlock.h"

class RenderBlockLoader {
public:
    RenderBlockLoader();
    RenderBlockLoader(const RenderBlockLoader& orig);
    virtual ~RenderBlockLoader();
    
    
    DRReturn init();
    void exit();
    RenderBlock* getRenderBlock(BlockName name);
    DRTextur* getTexture(const char* filename, GLint filter);
    
private:
    DRHashList* mTextures;
    DRHashList* mRenderBlocks;
    DRIni* mCfg;

};

#endif	/* __SC_RENDER_BLOCK_LOADER_H */

