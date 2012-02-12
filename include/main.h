#include "Engine2Main.h"
//includes which will be later go into Engine
//#include "DRGeometrieHeightfield.h"

//#include "DRGeometrieManager.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <vector>
#include <queue>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "Texture.h"
#include "DRTextureManager.h"

// Tools
#include "ShaderProgram.h"
//#include "ShaderManager.h"
#include "Unit.h"
#include "Vector3Unit.h"
#include "GenerateNoisePlanet.h"
#include "Camera.h"


// View
#include "Block.h"
#include "BlockLoader.h"
#include "RenderBlock.h"
#include "RenderBlockLoader.h"
//#include "SektorChunk.h"
#include "RenderToTexture.h"
#include "RenderInStepsToTexture.h"
#include "RenderNoisePlanetToTexture.h"
#include "RenderSektor.h"
#include "RenderSolarSystem.h"
//#include "RenderStellarBody.h"
#include "RenderPlanet.h"
#include "RenderSubPlanet.h"

// Modell
#include "StellarBody.h"
#include "Planet.h"
#include "GlobalRenderer.h"
#include "Sektor.h"
#include "PlanetSektor.h"
#include "SubPlanetSektor.h"
#include "SolarSystemSektor.h"
#include "RootSektor.h"
#include "Server.h"
#include "Player.h"

struct ControlMode;

extern Player g_Player;
extern RenderBlockLoader g_RenderBlockLoader;