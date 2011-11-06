/* 
 * File:   GenerateNoisePlanet.h
 * Author: Dario
 *
 * Created on 6. November 2011, 14:58
 */

#ifndef __SC_GENERATE_NOISE_PLANET_H
#define	__SC_GENERATE_NOISE_PLANET_H

#include <noise/noise.h>

class GenerateNoisePlanet
{
public:
    double getValue(DRVector3 point);
private:
     // 1: [Continent module]: This Perlin-noise module generates the continents.
     //    This noise module has a high number of octaves so that detail is
     //    visible at high zoom levels.
     noise::module::Perlin baseContinentDef_pe0;
};

#endif	/* __SC_GENERATE_NOISE_PLANET_H */

