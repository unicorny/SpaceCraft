#include "main.h"

SolarSystemSektor::SolarSystemSektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent)
: Sektor(position, radius, id, parent)
{
    mType = SOLAR_SYSTEM;
    
    //neuen Planeten erstellen
    Vector3Unit planetPosition(DRRandom::rVector3(1.0f), AE);
    planetPosition = planetPosition.normalize();
    planetPosition = planetPosition * Unit(0.2f, AE);
   // position.print("Planeten position");
    
    noise::module::Perlin p;
    
    if(id.count) p.SetSeed(id.count);
    //int seed = rand();
    int seed = (int)p.GetValue(id.x, id.y, id.z);
    Unit planetRadius(DRRandom::rDouble(72000, 1000), KM);
    //mCurrentSektor->setStellarBody(new Planet(radius, position, seed, mCurrentSektor));    
    Sektor* temp = new PlanetSektor(planetPosition, planetRadius, seed, this);
    mChilds.insert(SEKTOR_ENTRY(seed, temp));
}

SolarSystemSektor::~SolarSystemSektor()
{
    
}

DRReturn SolarSystemSektor::move(float fTime, Camera* cam)
{
    if(fTime == 0.0f)
    {
        Sektor* temp = mChilds.begin()->second;
        DRLog.writeToLog("new Game, set camera");
        cam->lookAt(temp->getPosition().getVector3());
    }
    
    return DR_OK;
}

DRReturn SolarSystemSektor::render(float fTime, Camera* cam)
{
    //Reseten der Matrixen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.001f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix

    glLoadIdentity();                    // Reset (init) the modelview matrix
    cam->setKameraMatrixRotation();
    
    return DR_OK;
}