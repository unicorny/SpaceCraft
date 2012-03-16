/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef __DR_ENGINE2_TEXTUR__
#define __DR_ENGINE2_TEXTUR__

class ENGINE2_API DRTextur
{
public:
    DRTextur();
    DRTextur(DRIImage* loadedImage, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DRTextur(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    virtual ~DRTextur();

    DRReturn load(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DRReturn loadFromMemory(DRColor* colors, DRVector2 size, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    void unload(bool full = true);
    
    bool isLoaded() {return mSucessfullLoaded;}

    DRReturn bind();
    
    __inline__ u32 getWidth() {if(mParent) return mParent->getWidth(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}
    __inline__ u32 getHeight() {if(mParent) return mParent->getHeight(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}
    
    //! return texture ID and set it to zero
    __inline__ GLuint removeTexturID() {GLuint id = mTexturID; mTexturID = 0; return id;}
protected:
    DRReturn load(GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    
    DRIImage* mParent;
    unsigned mTexturID;
    
    bool mSucessfullLoaded;

};

#endif //__DR_ENGINE2_TEXTUR__
