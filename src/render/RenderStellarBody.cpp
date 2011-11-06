#include "main.h"

RenderStellarBody::~RenderStellarBody()
{
    DR_SAVE_DELETE(mGeometrie);
}
