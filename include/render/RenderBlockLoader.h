/* 
 * File:   RenderBlockLoader.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:50
 */

#ifndef RENDERBLOCKLOADER_H
#define	RENDERBLOCKLOADER_H

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
    
private:
    DRHashList* mRenderBlocks;
    DRIni* mCfg;

};

#endif	/* RENDERBLOCKLOADER_H */

