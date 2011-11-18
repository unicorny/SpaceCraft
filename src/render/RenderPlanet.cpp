#include "main.h"

using namespace std;

PlanetHeightValues::PlanetHeightValues(GenerateNoisePlanet* noisePlanet): mNoisePlanet(noisePlanet)
{
    
}

PlanetHeightValues::~PlanetHeightValues()
{
	for(map<float, YMAP*>::iterator it = mXMap.begin(); it != mXMap.end(); it++)
	{
		YMAP* ymap = it->second;
		for(map<float, ZMAP*>::iterator it2 = ymap->begin(); it2 != ymap->end(); it2++)
		{
			ZMAP* zmap = it2->second;
			DR_SAVE_DELETE(zmap);
		}
		ymap->clear();
		DR_SAVE_DELETE(ymap);
	}
	mXMap.clear();
}

float PlanetHeightValues::getHeightValue(DRVector3& position)
{
	float temp = 0.0;
	if(findValue(position, temp)) return temp;
    else if(mNoisePlanet)
	{
		temp = mNoisePlanet->getValue(position)*0.00128441521;
		addValue(position, temp);
        return temp;
	}
    else 
	{
        return 0.0f;
	}
}

DRColor PlanetHeightValues::getColorValue(const float height)
{
    noise::utils::Color c = mColor.GetColor(height/0.00128441521);
    return DRColor(c.red, c.green, c.blue, c.alpha);
}

bool PlanetHeightValues::findValue(const DRVector3& position, float& value)
{
	map<float, YMAP*>::iterator it;
    it = mXMap.find(position.x);
	if(it == mXMap.end()) return false;

	map<float, ZMAP*>::iterator it2;
	it2 = it->second->find(position.y);
	if(it2 == it->second->end()) return false;

	map<float, float>::iterator it3;
	it3 = it2->second->find(position.z);
	if(it3 == it2->second->end()) return false;

	value = it3->second;
	return true;
}

void PlanetHeightValues::addValue(const DRVector3& position, float value)
{
	map<float, YMAP*>::iterator it;
	it = mXMap.find(position.x);
	if(it == mXMap.end())
	{
		ZMAP* zmap = new ZMAP;
		zmap->insert(ZMAP_PAIR(position.z, value));
		YMAP* ymap = new YMAP;
		ymap->insert(YMAP_PAIR(position.y, zmap));
		mXMap.insert(XMAP_PAIR(position.x, ymap));
	}
	else
	{
		map<float, ZMAP*>::iterator it2;
		it2 = it->second->find(position.y);
		if(it2 == it->second->end())
		{
			ZMAP* zmap = new ZMAP;
			zmap->insert(ZMAP_PAIR(position.z, value));
			it->second->insert(YMAP_PAIR(position.y, zmap));
		}
		else
		{
			it2->second->insert(ZMAP_PAIR(position.z, value));
		}
	}
}
// *******************************************************************************
RenderPlanet::~RenderPlanet()
{

}

DRReturn RenderPlanet::render(float fTime, Camera* cam)
{
    //glTranslatef(0.0f, 0.0f, -4.0f);
    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, 32, 32);
    
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}
