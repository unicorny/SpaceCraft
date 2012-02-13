/* 
 * File:   RenderBlock.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 23:50
 */

#include "RenderBlock.h"

RenderBlock::RenderBlock(DRTextur* texture, BlockName name, DRIni* cfg) 
: mTexture(texture)
{

    if(!texture || !cfg) LOG_ERROR_VOID("pointer zero");
    char buffer[5];
    memcpy(buffer, name.zeichen, 4);
    buffer[4] = '\0';
    
    DRVector2 tileSize = cfg->getVector2(buffer, "tileSize");
    DRVector2 textureSize((DRReal)texture->getWidth(), (DRReal)texture->getHeight());
    //DRLog.writeVector2ToLog(tileSize, "tileSize");
    tileSize /= textureSize;
    //DRLog.writeVector2ToLog(tileSize, "tileSize");    
    
    DRVector2 tileStarts[8];
    DRString tileNames[] = {"tile", "tileTop", "tileBottom", "tileSides",
                            "tileLeft", "tileRight", "tileFront", "tileBack"};
    for(int i = 0; i < 8; i++)
    {
        tileStarts[i] = cfg->getVector2(buffer, tileNames[i].data());
        if(tileStarts[i].x == -1) continue;
        tileStarts[i] /= textureSize;
        tileStarts[i].y = (1.0f-tileSize.y) - tileStarts[i].y;
        //tileStarts[i].y = texture->getWidth()-tileStarts[i].y-tileSize.y;
        //DRLog.writeVector2ToLog(tileStarts[i], "tile");
    }
    
    
    mList = glGenLists(1);
    glNewList(mList, GL_COMPILE); 
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D, 2);
    mTexture->bind();
        
    glBegin(GL_QUADS);
    int cur = 0;
    
    //Front
    cur = getTileStartIndex(tileStarts, 6, 3, 0); //tileFront, tileSides, tile
                
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(0.5f, -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(0.5f, 0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    //Right
    cur = getTileStartIndex(tileStarts, 5, 3, 0); //tileRight, tileSides, tile
    
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(0.5f, -0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(0.5f, 0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f(0.5f, 0.5f, 0.5f);
    
    //Back
    cur = getTileStartIndex(tileStarts, 7, 3, 0); //tileBack, tileSides, tile
    
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    //Left
    cur = getTileStartIndex(tileStarts, 4, 3, 0); //tileLeft, tileSides, tile
    
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    
    //Top
    cur = getTileStartIndex(tileStarts, 1, 0); //tileTop, tile
    
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f,  0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    
    //Bottom
    cur = getTileStartIndex(tileStarts, 2, 0); //tileBottom, tile
    
    setTextureCoords(tileStarts[cur], tileSize, 0);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f,  -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 2);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    setTextureCoords(tileStarts[cur], tileSize, 3);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    
    glEnd();
    
    
    glEndList();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

RenderBlock::RenderBlock(const RenderBlock& orig) {
}

RenderBlock::~RenderBlock() 
{
    glDeleteLists(mList, 1);
}

DRReturn RenderBlock::render()
{
    glCallList(mList);
    return DR_OK;
}

int RenderBlock::getTileStartIndex(DRVector2* tileStartsArray, int first, int second, int third/* = -1 */)
{
    if(tileStartsArray[first].x != -1.0f)
        return first;
    if(tileStartsArray[second].x != -1.0f)
        return second;
    if(third != -1)
        return third;
    
    return second;
}

/*! 
     * \param count  0 = top-left
     *               1 = top-right
     *               2 = bottom-right
     *               3 = bottom-left
     */
void RenderBlock::setTextureCoords(DRVector2 tileStart, DRVector2 tileSize, int count)
{
  //  DRLog.writeVector2ToLog(tileStart, "tileStart inside");
    switch(count)
    {
        case 0: 
            glTexCoord2f(tileStart.x, tileStart.y);
            break;
        case 1:
            glTexCoord2f(tileStart.x+tileSize.x, tileStart.y);
            break;
        case 2:
            glTexCoord2f(tileStart.x+tileSize.x, tileStart.y+tileSize.y);
            break;
        case 3:
            glTexCoord2f(tileStart.x, tileStart.y+tileSize.y);
            break;
        default:
            LOG_WARNING("count ist ungültig");
                
    }
}


