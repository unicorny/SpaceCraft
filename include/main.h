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

class RenderBlockLoader;
class Player;

struct ControlMode;

extern Player g_Player;
extern RenderBlockLoader g_RenderBlockLoader;