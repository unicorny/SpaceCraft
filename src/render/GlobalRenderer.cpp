#include "GlobalRenderer.h"

GlobalRenderer::GlobalRenderer()
: m_bInitialized(false), mQuadratic(NULL), mFrameBufferID(0),
  mTextureRenderStepSize(0), mTextureRenderMaxResolution(0)
{
    
}

GlobalRenderer& GlobalRenderer::Instance()
{
    static GlobalRenderer TheOneAndOnly;
    return TheOneAndOnly;
}

DRReturn GlobalRenderer::init(const char* configFilename)
{
    mQuadratic = gluNewQuadric(); // erzeugt einen Zeiger auf das Quadric Obkect ( NEU ) 
    if(!mQuadratic) LOG_ERROR("error by call gluNewQuadric", DR_ERROR);
    gluQuadricNormals(mQuadratic, GLU_SMOOTH); // erzeugt Normalen ( NEU ) 
    gluQuadricTexture(mQuadratic, GL_TRUE); // erzeugt Textur Koordinaten ( NEU )
    
    //render to texture
     glGenFramebuffersEXT(1, &mFrameBufferID);
    
    if(mPlanetShader.init("data/shader/planet2.vert", "data/shader/planet2.frag"))
        LOG_ERROR("Fehler beim laden des Shaders", DR_ERROR);
    
    DRIni cfg(configFilename);
    mTextureRenderStepSize = cfg.getInt("RenderToTexte", "StepSize");
    mTextureRenderMaxResolution = cfg.getInt("RenderToTexte", "Resolution");
    
    m_bInitialized= true;
    return DR_OK;
}

void GlobalRenderer::addRenderTask(RenderInStepsToTexture* newRenderTask, bool preview/* = false*/)
{
    if(preview)
        mPreviewRenderTasks.push(newRenderTask);
    else
        mRenderTasks.push(newRenderTask);
}

DRReturn GlobalRenderer::renderTasks()
{
    Uint32 start = SDL_GetTicks();
    RenderInStepsToTexture* current = NULL;
        
    if(!mPreviewRenderTasks.empty())
    {
        current = mPreviewRenderTasks.front();
        if(setupFrameBuffer(current->getTextur())) //setup framebuffer with new texture
            LOG_ERROR("Fehler bei setupFrameBuffer 1", DR_ERROR);
        if(current->step()) LOG_ERROR("Fehler bei Step a PreviewRenderTask", DR_ERROR);
    
        if(current->isFinished())
            mPreviewRenderTasks.pop(); //remove finished Task
    }
    if(!mRenderTasks.empty())
    {
        current = mRenderTasks.front();
        if(setupFrameBuffer(current->getTextur())) //setup framebuffer with new texture
            LOG_ERROR("Fehler bei setupFrameBuffer 2", DR_ERROR);
        if(current->step()) LOG_ERROR("Fehler bei Step a RenderTask", DR_ERROR);
    
        if(current->isFinished())
            mRenderTasks.pop(); //remove finished Task
    }    
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);   
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    
    //printf("\r time used: %f", static_cast<float>(SDL_GetTicks()-start)/1000.0f);
    return DR_OK;
}

DRReturn GlobalRenderer::setupFrameBuffer(Texture* texture)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBufferID);
    //create texture
    //bind to the new texture ID
    texture->bind();
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, texture->getId(), 0);
    GLenum ret = GL_FRAMEBUFFER_COMPLETE_EXT;
    ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(ret != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        DRLog.writeToLog("Fehler bei Check Framebuffer Status: %s", getFrameBufferEnumName(ret));
        LOG_ERROR("Fehler bei setupFrameBuffer", DR_ERROR);
    }
   // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    return DR_OK;
}


void GlobalRenderer::exit()
{
    if(mFrameBufferID)
        glDeleteFramebuffersEXT(1, &mFrameBufferID);
    mFrameBufferID = 0;
    gluDeleteQuadric(mQuadratic); 
    m_bInitialized = false;    
}

const char* GlobalRenderer::getFrameBufferEnumName(GLenum name)
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