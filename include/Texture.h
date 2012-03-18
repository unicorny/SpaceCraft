#ifndef __SC_TEXTURE__
#define __SC_TEXTURE__

#define TEXTURE_WRAPPING_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE

#include "main.h"

/**!
 * TODO: 
 *  - Konstruktor privat, TextureManager friend 
 *  - load from file get memory from texture manager
 *  - remove DRTexture 
 *  - remove global renderer grafik mem
 *  - separate Texture Save class 
 */

class Texture : public DRIResource
{
    friend class DRTextureManager;
public:
	// create an empty texture
        //! \param dataSize size of data in bytes
        Texture(DRVector2i size, GLuint format, GLubyte* data = NULL, GLint dataSize = 0); 
        ~Texture();
	
	__inline__ bool isLoadingFinished() {return mLoadingState == 2;}
	__inline__ bool isTextureEmpty() {return mLoadingState == 3;}
	__inline__ bool isTextureReadyToSave() {return mSavingState == 2;}
	__inline__ bool isTextureSaved() {return mSavingState == 3;}
	__inline__ bool isLoadingError() {return mLoadingState == -1;}
	__inline__ void setErrorByLoading() {mLoadingState = -1;}

	void bind();
	__inline__ static void unbind() {glBindTexture(GL_TEXTURE_2D, 0);}

	DRReturn getPixelsToSave(const char* path);
	DRReturn putPixelsToImage();
	DRReturn saveImage();

	//Parameter
	void setWrappingMode(GLint mode);
        void setFilter(GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
	DRVector2i getResolution();
        
        virtual const char* getResourceType() const {return "Texture";}
        virtual bool less_than(DRIResource& tex) const
        {
            return mTexturID <  dynamic_cast<Texture&>(tex).mTexturID;
        }

	//Debug
	__inline__ GLuint getId() {return mTexturID;}

protected:
        Texture(const char* filename);
        Texture(GLuint textureID, GLubyte* data = NULL, GLint dataSize = 0);
        
        // load texture
	//! \brief load pixel data from File into system memory, can be called from another thread
	//! \return DR_OK if okay (set mLoadingState to 1)
	//! \return DR_ERROR if an error occured
	DRReturn loadFromFile();
	//! \brief load pixeldata from system memory to 3D-Renderer (OpenGL), must be called from Main Thread (OpenGL)
	//! \return DR_OK if okay
	//! \return DR_NOT_ERROR if there weren't pixel data in system memory
	DRReturn pixelsCopyToRenderer();
    
        GLuint      mTexturID;
        GLuint	    mPboID; // OpenGL Pixel Buffer Object ID
	GLuint	    mPboSaveID;// OpenGL Pixel Buffer Object to save texture
        DRString    mFilename;
        short       mLoadingState; // 0 = empty, 1 = textur im arbeitsspeicher, 2 = Textur bei OpenGL, 3 = empty Textur 
	short	    mSavingState; //
	GLuint	    mSavingCursor;
	u8*	    mSavingBuffer;
	DRIImage*   mImage;
        DRVector2i  mSize;
private:

};

typedef DRResourcePtr<Texture> TexturePtr;

#endif //__SC_TEXTURE__ 