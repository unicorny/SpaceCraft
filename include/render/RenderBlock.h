/* 
 * File:   RenderBlock.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:50
 */

#ifndef RENDERBLOCK_H
#define	RENDERBLOCK_H

#include "main.h"
#include "Block.h"

class RenderBlock {
public:
    RenderBlock(DRTextur* texture, BlockName name, DRIni* cfg);
    RenderBlock(const RenderBlock& orig);
    virtual ~RenderBlock();
    
    DRReturn render();
    
private:    
    
    static int getTileStartIndex(DRVector2* tileStartsArray, int first, int second, int third = -1 );
    /*! 
     * \param count  0 = top-left
     *               1 = bottom-left
     *               2 = bottom-right
     *               3 = top-right
     */
    static void setTextureCoords(DRVector2 tileStart, DRVector2 tileSize, int count);
    
    DRTextur* mTexture;
    GLuint    mList;

};

#endif	/* RENDERBLOCK_H */

