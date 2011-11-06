#include "main.h"


GlobalRenderer::GlobalRenderer()
: m_bInitialized(false), mQuadratic(NULL)
{
    
}

GlobalRenderer& GlobalRenderer::Instance()
{
    static GlobalRenderer TheOneAndOnly;
    return TheOneAndOnly;
}

DRReturn GlobalRenderer::init()
{
    mQuadratic = gluNewQuadric(); // erzeugt einen Zeiger auf das Quadric Obkect ( NEU ) 
    if(!mQuadratic) LOG_ERROR("error by call gluNewQuadric", DR_ERROR);
    gluQuadricNormals(mQuadratic, GLU_SMOOTH); // erzeugt Normalen ( NEU ) 
    gluQuadricTexture(mQuadratic, GL_TRUE); // erzeugt Textur Koordinaten ( NEU )
    
    if(mPlanetShader.init("data/shader/planet1.vert", "data/shader/planet1.frag"))
        LOG_ERROR("Fehler beim laden des Shaders", DR_ERROR);
    
    
    m_bInitialized= true;
    return DR_OK;
}

void GlobalRenderer::exit()
{
    gluDeleteQuadric(mQuadratic); 
    m_bInitialized = false;
}