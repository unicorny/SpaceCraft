#ifndef __SC_TEXTURE__
#define __SC_TEXTURE__

#define TEXTURE_WRAPPING_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE

class Texture
{
public:
     Texture(const char* filename, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
	 // create an empty texture
	 //! \param dataSize size of data in bytes
	 Texture(GLuint width, GLuint height, GLuint bpp, GLuint format, void* data = NULL, GLint dataSize = 0);
    ~Texture();

	// load texture
	//! \brief load pixel data from File into system memory, can be called from another thread
	//! \return DR_OK if okay (set mLoadingState to 1)
	//! \return DR_ERROR if an error occured
	DRReturn loadFromFile();
	//! \brief load pixeldata from system memory to 3D-Renderer (OpenGL), must be called from Main Thread (OpenGL)
	//! \return DR_OK if okay
	//! \return DR_NOT_ERROR if there weren't pixel data in system memory
	DRReturn pixelsCopyToRenderer();
	__inline__ bool isLoadingFinished() {return mLoadingState == 2;}
	__inline__ bool isTextureEmpty() {return mLoadingState == 3;}

	void bind();
	__inline__ static void unbind() {glBindTexture(GL_TEXTURE_2D, 0);}

	DRReturn getPixelsToSave(const char* path);

	//Parameter
	void setWrappingMode(GLint mode);
	Eigen::Vector2i getResolution();

	//Debug
	__inline__ GLuint getId() {return mTexturID;}

protected:
    GLuint      mTexturID;
	GLuint		mPboID; // OpenGL Pixel Buffer Object ID
	GLuint		mPboSaveID;// OpenGL Pixel Buffer Object to save texture
    DRString    mFilename;
    short       mLoadingState; // 0 = empty, 1 = textur im arbeitsspeicher, 2 = Textur bei OpenGL, 3 = empty Textur 
	short		mSavingState; //
	DRIImage*	mImage;
private:
    
    
};
#endif //__SC_TEXTURE__