
#ifndef __SC_RENDER_TO_TEXTURE_
#define __SC_RENDER_TO_TEXTURE_

/*!
 * \brief class to easy render something into texture
 * 
 * @author: Dario Rekowsk8i
 * @date: 26.11.2011
 * 
 * 
 */

class RenderToTexture
{
public:
    RenderToTexture();
    
    ~RenderToTexture();
    
    //! \brief set size of render target and texture
    DRReturn setup(DRVector2 size, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    //! \brief bind render target as texture to opengl
    DRReturn bindTexture();
    //! \brief bind render target to get the stuff from the pipline
    DRReturn bindToRender();
    
    void unbind();
    
    //! \brief bind new texture to framebuffer object, leave framebuffer unbound
    //! \return oldTexture
    GLuint switchTexture(GLuint newTexture);
    
    //! \brief render a quad with size and shader to texture
    //! \param shader describe the effect
    //! \param size texture size
    //! \param texture target texture
    //DRReturn shaderToTexture(ShaderProgram* shader, DRVector2 size, DRTextur* texture);
    
    DRReturn saveToImage(const char* path);
    
    __inline__ DRVector2 getSize() {return mSize;}
    
    GLuint removeTexture(); 
    
protected:
    
    static const char* getFrameBufferEnumName(GLenum name);
    
    GLuint      mTextureID;
    GLuint      mFrameBuffer;
    DRVector2   mSize;
};

#endif //__SC_RENDER_TO_TEXTURE_