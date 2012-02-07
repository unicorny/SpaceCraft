#include "main.h"

RenderToTexture::RenderToTexture()
: mTextureID(0), mFrameBuffer(0)
{
    if(!glBindFramebufferEXT || !glGenFramebuffersEXT || !glFramebufferTexture2DEXT || !glCheckFramebufferStatusEXT)
        LOG_ERROR_VOID("some fbo extension is missing");
    
    glGenTextures(1, &mTextureID);    
    glGenFramebuffersEXT(1, &mFrameBuffer);
}

RenderToTexture::~RenderToTexture()
{
    if(mTextureID)
        glDeleteTextures(1, &mTextureID);
    mTextureID = 0;
    if(mFrameBuffer)
        glDeleteFramebuffersEXT(1, &mFrameBuffer);
    mFrameBuffer = 0;
}

GLuint RenderToTexture::removeTexture()
{
    GLuint tex = mTextureID; 
    mTextureID = 0; 
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, mTextureID, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    return tex;
}

//! \brief set size of render target and texture
DRReturn RenderToTexture::setup(DRVector2 size, GLint glMinFilter/* = GL_LINEAR*/, GLint glMagFilter/* = GL_LINEAR*/)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);
    //create texture
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
    glTexImage2D(GL_TEXTURE_2D, 0, 4, size.x, size.y, 0,
		GL_RGBA, GL_FLOAT, 0);
    
    mSize = size;   
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, mTextureID, 0);
    GLenum ret = GL_FRAMEBUFFER_COMPLETE_EXT;
    ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(ret != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        DRLog.writeToLog("Fehler bei Check Framebuffer Status: %s", getFrameBufferEnumName(ret));
        LOG_ERROR("...", DR_ERROR);
    }
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    return DR_OK;
}

GLuint RenderToTexture::switchTexture(GLuint newTexture)
{
    GLuint tempTexture = mTextureID;
    mTextureID = newTexture;
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, mTextureID, 0);
    GLenum ret = GL_FRAMEBUFFER_COMPLETE_EXT;
    ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(ret != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        DRLog.writeToLog("Fehler bei Check Framebuffer Status: %s", getFrameBufferEnumName(ret));
        LOG_ERROR("Fehler bei switchTexture", DR_ERROR);
    }
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    return tempTexture;
}
//! \brief bind render target as texture to opengl
DRReturn RenderToTexture::bindTexture()
{
    glEnable(GL_TEXTURE_2D);
    //bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	if(DRGrafikError("RenderToTexture::bindTexture")) return DR_ERROR;
    
    return DR_OK;
}
    
//! \brief bind render target to get the stuff from the pipline
DRReturn RenderToTexture::bindToRender()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);   
    return DR_OK;
}

void RenderToTexture::unbind()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

DRReturn RenderToTexture::saveToImage(const char* path)
{
    DRIImage* image = DRIImage::newImage();
    u8* buffer = (u8*)malloc(mSize.x*mSize.y*4*sizeof(u8));
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    if(DRGrafikError("RenderToTexture::saveToImage")) LOG_ERROR("Fehler bei getting texture Data!", DR_ERROR);
    image->setSize(mSize);
    image->setImageFormat(-1);
    image->setPixel(buffer);
    if(image->saveIntoFile(path))
        LOG_ERROR("fehler bei save", DR_ERROR);
    
    free(buffer);
    delete image;
    
    return DR_OK;
}
    
//! \brief render a quad with size and shader to texture
//! \param shader describe the effect
//! \param size texture size
//! \param texture target texture
//DRReturn RenderToTexture::shaderToTexture(ShaderProgram* shader, DRVector2 size, DRTextur* texture)
//{
    
//}

const char* RenderToTexture::getFrameBufferEnumName(GLenum name)
{
    switch(name)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT: return "GL_FRAMEBUFFER_COMPLETE_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT"; 
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT: return "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
    default: return "-unknown enum-";
    }
    return "-error-";
}