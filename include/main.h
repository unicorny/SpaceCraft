#include "Engine2Main.h"

#include <iostream>
#include <sstream>


// Tools
#include "ShaderProgram.h"
#include "Unit.h"
#include "Vector3Unit.h"
#include "GenerateNoisePlanet.h"
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

// Modell
#include "StellarBody.h"
#include "Planet.h"

#include "GlobalRenderer.h"

#include "Sektor.h"
#include "PlanetSektor.h"
#include "SolarSystemSektor.h"
#include "RootSektor.h"
#include "Server.h"
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