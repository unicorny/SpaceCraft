
#ifndef __SC_MAIN_
#define __SC_MAIN_

#include "Engine2Main.h"
//includes which will be later go into Engine
//#include "DRGeometrieHeightfield.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits.h>

// Units
#include "Unit.h"
#include "Vector3Unit.h"

// Tools
//#include "ShaderProgram.h"
//#include "GenerateNoisePlanet.h"
#include "Camera.h"

// View
#include "Block.h"
#include "BlockLoader.h"
#include "RenderBlock.h"
#include "RenderBlockLoader.h"
#include "SektorChunk.h"
#include "RenderSektor.h"
#include "RenderSolarSystem.h"
#include "RenderStellarBody.h"
#include "RenderPlanet.h"
#include "RenderSubPlanet.h"

// Modell
#include "StellarBody.h"
#include "Planet.h"

//#include "GlobalRenderer.h"

#include "Sektor.h"
#include "SubPlanetSektor.h"
#include "PlanetSektor.h"
#include "SolarSystemSektor.h"
#include "RootSektor.h"
//#include "Server.h"
#include "Player.h"

struct ControlMode
{
    ControlMode() {}
    ControlMode(Unit value) : mValue(value) {}
    ControlMode(double value, UnitTypes type) : mValue(value, type) {}
    Unit mValue;
};

extern Player g_Player;
extern RenderBlockLoader g_RenderBlockLoader;

#endif //__SC_MAIN_