
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
    
    //! \brief set size of render target and texture
    DRReturn setup(DRVector2 size);
    //! \brief bind render target as texture to opengl
    DRReturn bindTexture();
    //! \brief bind render target to get the stuff from the pipline
    DRReturn bindToRender();
    
    //! \brief render a quad with size and shader to texture
    //! \param shader describe the effect
    //! \param size texture size
    //! \param texture target texture
    //DRReturn shaderToTexture(ShaderProgram* shader, DRVector2 size, DRTextur* texture);
    
    //DRTextur* getTexture();
    
protected:
    GLuint      mTextureID;
};

#endif //__SC_RENDER_TO_TEXTURE_