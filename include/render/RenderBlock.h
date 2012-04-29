/* 
 * File:   RenderBlock.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:50
 */

<<<<<<< HEAD
#ifndef __SC_RENDER_BLOCK__
#define	__SC_RENDER_BLOCK__
=======
#ifndef __SC_RENDER_BLOCK_H
#define	__SC_RENDER_BLOCK_H

>>>>>>> c1a3e92256268443cee33f382589cb05f672be4a

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

<<<<<<< HEAD
#endif	/* __SC_RENDER_BLOCK__ */
=======
#endif	/* __SC_RENDER_BLOCK_H */
>>>>>>> c1a3e92256268443cee33f382589cb05f672be4a

