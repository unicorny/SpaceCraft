#include "Texture.h"
#include "GlobalRenderer.h"


Texture::Texture(const char* filename, GLint glMinFilter /* = GL_LINEAR */, GLint glMagFilter /* = GL_LINEAR */)
: mTexturID(0), mPboID(0), mFilename(filename), mLoadingState(0), 
  mSavingState(0), mSavingCursor(0), mSavingBuffer(NULL), mImage(NULL),
  mTextureWidth(0), mTextureHeight(0)
{
	glGenTextures(1, &mTexturID);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, mTexturID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);

	glGenBuffersARB(1, &mPboID);
}

Texture::Texture(GLuint width, GLuint height, GLuint bpp, GLuint format, void* data, GLint dataSize)
: mTexturID(0), mPboID(0), mFilename(""), mLoadingState(0),
  mSavingState(0), mSavingCursor(0), mSavingBuffer(NULL), mImage(NULL),
  mTextureWidth(width), mTextureHeight(height)
{
	glGenTextures(1, &mTexturID);
	glBindTexture(GL_TEXTURE_2D, mTexturID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint format2 = GL_RGB;
	if(format == 4) format2 = GL_RGBA;

	if(data && dataSize)
	{
		glGenBuffersARB(1, &mPboID);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);	

		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 
			data, GL_STREAM_DRAW_ARB);	
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
		format2, bpp, NULL);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	mLoadingState = 3;
}

Texture::~Texture()
{
	if(mTexturID)
	{
		glDeleteTextures(1, &mTexturID);
		mTexturID = 0;
	}	
	if(mPboID)
	{
		glDeleteBuffersARB(1, &mPboID);
		mPboID = 0;
	}
	if(mPboSaveID)
	{
		glDeleteBuffersARB(1, &mPboSaveID);
		mPboSaveID = 0;
	}
	if(mImage)
	{
		DRIImage::deleteImage(mImage);
		mImage = NULL;
	}
	if(mSavingBuffer)
	{
		DR_SAVE_DELETE_ARRAY(mSavingBuffer);
	}
}

DRReturn Texture::loadFromFile()
{
	mImage = DRIImage::newImage();
	const char* path = DRFileManager::Instance().getWholePfad(mFilename.data());
	if(!path)
	{
		if(mImage->loadFromFile(mFilename.data()))
			LOG_ERROR("Fehler beim Textur laden", DR_ERROR);
	}
	else
	{
		if(mImage->loadFromFile(DRString(DRString(path)+"/"+mFilename).data()))
			LOG_ERROR("Fehler2 beim Textur laden", DR_ERROR);
	}
	mTextureWidth = mImage->getWidth();
	mTextureHeight = mImage->getHeight();
	mLoadingState = 1;
	return DR_OK;
}

DRReturn Texture::pixelsCopyToRenderer()
{
	if(!mImage) LOG_ERROR("fehler, mImage is zero", DR_NOT_ERROR);
	if(isLoadingError()) LOG_ERROR("error by loading", DR_ERROR);
	glBindTexture(GL_TEXTURE_2D, mTexturID);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
	GLenum format = mImage->getImageFormat();
	unsigned numComponents = 0;
	if(format == GL_RGB) numComponents = 3;
	else if(format == GL_RGBA) numComponents = 4;
	else LOG_ERROR("ungueltiges Format", DR_ERROR);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mImage->getWidth()*mImage->getHeight()*numComponents, 
					mImage->getPixel(), GL_STREAM_DRAW_ARB);

	glTexImage2D(GL_TEXTURE_2D, 0, numComponents, mImage->getWidth(), mImage->getHeight(),
		0, format, GL_UNSIGNED_BYTE, NULL);

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	if(DRGrafikError("Error by copying pixels to OpenGL")) return DR_ERROR;

	DRIImage::deleteImage(mImage); mImage = NULL;
	printf("texture load\n");
	
	mLoadingState = 2;

	return DR_OK;
}

DRReturn Texture::getPixelsToSave(const char* path)
{
	
	if(!mPboSaveID)
	{
		glGenBuffersARB(1, &mPboSaveID);
	}
	mFilename = path;
	bind();
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, mPboSaveID);
	Eigen::Vector2i size = Eigen::Vector2i(mTextureWidth, mTextureHeight);
	printf("size: %d, %d\n", size(0), size(1));
	mSavingBuffer = new u8[size(0)*size(1)*4];
	mSavingCursor = 0;
	glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, size(0)*size(1)*4*sizeof(u8), NULL, GL_STREAM_READ_ARB);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	//glReadPixels(0, 0, size(0), size(1), GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	DRGrafikError("glGetTexImage");
		
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
	
	if(DRGrafikError("Texture::getPixelsToSave(): error by asynchronously saving an image!"))
		LOG_ERROR("Fehler bei save image", DR_ERROR);
	mSavingState = 1;

	return DR_OK;
}

DRReturn Texture::putPixelsToImage()
{
	if(!mImage) mImage = DRIImage::newImage();
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, mPboSaveID);
	GLubyte* ptr = static_cast<GLubyte*>(glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB));
	DRGrafikError("map Buffer");
	//if(!ptr) LOG_ERROR("glMapBuffer return ZERO Pointer", DR_ERROR);
	Eigen::Vector2i size = Eigen::Vector2i(mTextureWidth, mTextureHeight);
	if(ptr)
	{
		GLuint stepSize = GlobalRenderer::Instance().getTextureRenderStepSize()*2*4;
		stepSize *= stepSize;
		GLuint bufferSize = size(0)*size(1)*4*sizeof(u8);
		printf("\rstepSize KByte: %d, KBytes left: %d", stepSize/1024, (bufferSize-mSavingCursor)/1024);
		if(mSavingCursor+stepSize >= bufferSize)
		{
			memcpy(&mSavingBuffer[mSavingCursor], &ptr[mSavingCursor], bufferSize-mSavingCursor);
			mSavingState = 2;
		}
		else
		{
			memcpy(&mSavingBuffer[mSavingCursor], &ptr[mSavingCursor], stepSize);
			mSavingCursor += stepSize;
		}

		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
	}

	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);

	if(DRGrafikError("Texture::getPixelsToSave(): error by asynchronously saving an image!"))
		LOG_ERROR("Fehler bei save image", DR_ERROR);
	return DR_OK;
}

DRReturn Texture::saveImage()
{
	if(!mImage) return DR_ZERO_POINTER;
	if(mSavingState < 2) LOG_ERROR("Image data not ready!", DR_ERROR);
	if(mSavingState > 2) LOG_ERROR("ImageData already saved!", DR_OK);

	Eigen::Vector2i size = Eigen::Vector2i(mTextureWidth, mTextureHeight);
	if(size(0) <= 0) LOG_ERROR("error getSize return zero", DR_ERROR);
	if(mSavingBuffer)
	{
		mImage->setSize(DRVector2(size(0), size(1)));
		mImage->setImageFormat(-1);
		mImage->setPixel(mSavingBuffer);		
		DR_SAVE_DELETE_ARRAY(mSavingBuffer);
	}

	if(mImage->saveIntoFile(mFilename.data()))
		LOG_ERROR("fehler bei save", DR_ERROR);
	DRIImage::deleteImage(mImage);
	mImage = NULL;
		
	mSavingState = 3;
	printf("image saved\n");
	return DR_OK;
}

void Texture::bind()
{
	if(mLoadingState >= 2)
	{
		glBindTexture(GL_TEXTURE_2D, mTexturID);
	}
}

void Texture::setWrappingMode(GLint mode)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
}

Eigen::Vector2i Texture::getResolution()
{
	return Eigen::Vector2i(mTextureWidth, mTextureHeight);
	if(mLoadingState < 2) return Eigen::Vector2i(-1, -1);
	
	Eigen::Vector2i v;
	glBindTexture(GL_TEXTURE_2D, mTexturID);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &v(0));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &v(1));
	DRGrafikError("getResolution");
	return v;
}