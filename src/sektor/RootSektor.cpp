#include "main.h"

RootSektor::RootSektor(SektorID id)
: Sektor(Vector3Unit(1.0), Unit(0.0, M), id, NULL)
{
    mType = SEKTOR_ROOT;
}

RootSektor::~RootSektor()
{
    
}

void RootSektor::addSektor(Sektor* sektor, SektorID id)
{
    mChilds.insert(SEKTOR_ENTRY(id, sektor));
}

DRReturn RootSektor::move(float fTime, Camera* cam)
{
    mLastRelativeCameraPosition = cam->getSektorPositionAtSektor(this);
    return DR_OK;
}

DRReturn RootSektor::render(float fTime, Camera* cam)
{
    //Reseten der Matrixen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.01f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix

    glLoadIdentity();                    // Reset (init) the modelview matrix
    cam->setKameraMatrixRotation();
    
    return DR_OK;
}