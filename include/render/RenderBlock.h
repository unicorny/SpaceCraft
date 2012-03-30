/* 
 * File:   RenderBlock.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:50
 */

#ifndef __SC_RENDER_BLOCK__
#define	__SC_RENDER_BLOCK__

#include "main.h"
#include "Block.h"

class RenderBlock {
public:
    RenderBlock(DRTexturePtr texture, BlockName name, DRIni* cfg);
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
    
    DRTexturePtr mTexture;
    GLuint    mList;

};

#endif	/* __SC_RENDER_BLOCK__ */

