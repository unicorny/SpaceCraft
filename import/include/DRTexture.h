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

#ifndef __DR_ENGINE2_TEXTURE__
#define __DR_ENGINE2_TEXTURE__

#define TEXTURE_WRAPPING_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE

/**!
 * TODO: 
 *  - remove global renderer grafik mem
 */

class ENGINE2_API DRTextureBuffer : public DRIResource
{
public:
    DRTextureBuffer(): mTextureID(0) {glGenTextures(1, &mTextureID);}
    ~DRTextureBuffer() {glDeleteTextures(1, &mTextureID);}
    operator GLuint() {return mTextureID;}
    
    virtual const char* getResourceType() const {return "TextureBuffer";}
    virtual bool less_than(DRIResource& tex) const
    {
        return mTextureID <  dynamic_cast<DRTextureBuffer&>(tex).mTextureID;
    }
protected:
    GLuint     mTextureID;
};
typedef DRResourcePtr<DRTextureBuffer> DRTextureBufferPtr;

class ENGINE2_API DRTexture : public DRIResource
{
    friend class DRTextureManager;
public:
	__inline__ bool isLoadingFinished() {return mLoadingState == 2;}
	__inline__ bool isTextureEmpty() {return mLoadingState == 3;}
	__inline__ bool isLoadingError() {return mLoadingState == -1;}
	__inline__ void setErrorByLoading() {mLoadingState = -1;}
        __inline__ void setFinishRender() {if(mLoadingState == 3) mLoadingState = 2;}

	void bind();
	__inline__ static void unbind() {glBindTexture(GL_TEXTURE_2D, 0);}

	//Parameter
	void setWrappingMode(GLint mode);
        void setFilter(GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
	DRVector2i getResolution();
        
        virtual const char* getResourceType() const {return "Texture";}
        virtual bool less_than(DRIResource& tex) const
        {
            return mTexturID <  dynamic_cast<DRTexture&>(tex).mTexturID;
        }

	//Debug
	__inline__ GLuint getId() {return *mTexturID;}
        __inline__ DRTextureBufferPtr getTextureBuffer() {return mTexturID;}

protected:
        DRTexture(const char* filename);
        DRTexture(DRTextureBufferPtr textureID, GLubyte* data = NULL, GLint dataSize = 0, GLint dataType = GL_UNSIGNED_BYTE);
        // create an empty texture
        //! \param dataSize size of data in bytes        
        virtual ~DRTexture();
        
        // load texture
	//! \brief load pixel data from File into system memory, can be called from another thread
	//! \return DR_OK if okay (set mLoadingState to 1)
	//! \return DR_ERROR if an error occured
	DRReturn loadFromFile();
	//! \brief load pixeldata from system memory to 3D-Renderer (OpenGL), must be called from Main Thread (OpenGL)
	//! \return DR_OK if okay
	//! \return DR_NOT_ERROR if there weren't pixel data in system memory
	DRReturn pixelsCopyToRenderer();
    
        DRTextureBufferPtr      mTexturID;
        GLuint                  mPboID; // OpenGL Pixel Buffer Object ID
        DRString                mFilename;
        short                   mLoadingState; // 0 = empty, 1 = textur im arbeitsspeicher, 2 = Textur bei OpenGL, 3 = empty Textur 
	DRIImage*               mImage;
        DRVector2i              mSize;
private:

};

typedef DRResourcePtr<DRTexture> DRTexturePtr;

#endif //__DR_ENGINE2_TEXTURE__ 