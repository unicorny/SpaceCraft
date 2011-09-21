/* 
 * File:   Planet.h
 * Author: Dario
 *
 * Created on 20. September 2011, 21:16
 */

#ifndef __SPACE_CRAFT_PLANET_H
#define	__SPACE_CRAFT_PLANET_H

class Planet : public StellarBody
{
public:
    Planet(Unit radius, Unit position[3], int seed, Sektor* parent = NULL);
    ~Planet();
private:
};

#endif	/* __SPACE_CRAFT_PLANET_H */

