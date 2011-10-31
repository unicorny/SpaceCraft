#include "Engine2Main.h"

#include <iostream>
#include <sstream>


// Tools
#include "ShaderProgram.h"
#include "Unit.h"
#include "Vector3Unit.h"

// View
#include "Block.h"
#include "BlockLoader.h"
#include "RenderBlock.h"
#include "RenderBlockLoader.h"
#include "SektorChunk.h"

// Modell
#include "StellarBody.h"
#include "Planet.h"

#include "GlobalRenderer.h"

#include "Camera.h"
#include "Sektor.h"
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