/* 
 * File:   RenderPlanet.h
 * Author: Dario
 *
 * Created on 6. November 2011, 14:48
 */

#ifndef __SC_RENDER_PLANET_H
#define	__SC_RENDER_PLANET_H

#include "noiseutils.h"

class PlanetHeightValues: public DRHeightValueStorage
{
public:
   PlanetHeightValues(GenerateNoisePlanet* noisePlanet = NULL);
   virtual ~PlanetHeightValues();
   virtual float getHeightValue(DRVector3& position);
   virtual DRColor getColorValue(const float height);
   
   __inline__ void setNoisePlanet(GenerateNoisePlanet* noisePlanet) {mNoisePlanet = noisePlanet;}
   __inline__ void AddGradientPoint (double gradientPos, const noise::utils::Color& gradientColor) {mColor.AddGradientPoint(gradientPos, gradientColor);}
   __inline__ void ClearGradientPoints() {mColor.Clear();}
   
private:
	//! \return true if value could be found, else false
    bool findValue(const DRVector3& position, float& value);
	void addValue(const DRVector3& position, float value);
    
    noise::utils::GradientColor mColor;
    GenerateNoisePlanet* mNoisePlanet;
    
    typedef std::pair<float, float> ZMAP_PAIR;
	typedef std::map<float, float> ZMAP;
    typedef std::pair<float, ZMAP*> YMAP_PAIR;
	typedef std::map<float, ZMAP*> YMAP;
    typedef std::pair<float, YMAP*> XMAP_PAIR;
    std::map<float, YMAP*> mXMap;
    
};

class RenderPlanet : public RenderSektor
{
public:
    RenderPlanet() : RenderSektor() {};
    virtual ~RenderPlanet();
    
    virtual DRReturn render(float fTime, Camera* cam);
protected:
    
private:    
};


#endif	/* __SC_RENDER_PLANET_H */

