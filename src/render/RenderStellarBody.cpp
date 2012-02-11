#include "main.h"
#include "RenderStellarBody.h"

RenderStellarBody::~RenderStellarBody()
{
    DR_SAVE_DELETE(mGeometrie);
}
