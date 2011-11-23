#include "main.h"

PlanetHeightValues::PlanetHeightValues(GenerateNoisePlanet* noisePlanet)
: mNoisePlanet(noisePlanet)
{
    
}

PlanetHeightValues::~PlanetHeightValues()
{

}

float PlanetHeightValues::getHeightValue(DRVector3& position)
{
    return mNoisePlanet->getValue(position);//*0.00128441521;
}

DRColor PlanetHeightValues::getColorValue(const float height)
{
    noise::utils::Color c = mColor.GetColor(height);///0.00128441521);
    return DRColor(c.red, c.green, c.blue, c.alpha);
}

// *******************************************************************************
RenderPlanet::RenderPlanet(GenerateNoisePlanet* noiseGenerator)
: RenderSektor(), mHeights(NULL), mNoiseGenerator(noiseGenerator), mPlanetTexture(NULL),
  mUpdateTextureThread(NULL), mUpdateTextureThreadSemaphore(NULL), mUpdateTextureMutex(NULL),
  mTextureTempPoints(NULL), mTextureSize(DRVector2(2.0f))
{
    //mNoiseGenerator = new GenerateNoisePlanet();
    if(!mNoiseGenerator) return;
    mHeights = new PlanetHeightValues(mNoiseGenerator);
    
    double seaLevelInMeters = mNoiseGenerator->getSeaLevelInMetres();
    
    mHeights->ClearGradientPoints ();
    mHeights->AddGradientPoint (-2.0 + seaLevelInMeters, noise::utils::Color (  0,   0,   0, 255));
    mHeights->AddGradientPoint (    -0.03125 + seaLevelInMeters, noise::utils::Color (  6,  58, 127, 255));
    mHeights->AddGradientPoint (    -0.0001220703 + seaLevelInMeters, noise::utils::Color ( 14, 112, 192, 255));
    mHeights->AddGradientPoint (     0.0 + seaLevelInMeters, noise::utils::Color ( 70, 120,  60, 255));
    mHeights->AddGradientPoint (  0.125 + seaLevelInMeters, noise::utils::Color (110, 140,  75, 255));
    mHeights->AddGradientPoint (  0.25 + seaLevelInMeters, noise::utils::Color (160, 140, 111, 255));
    mHeights->AddGradientPoint (  0.375 + seaLevelInMeters, noise::utils::Color (184, 163, 141, 255));
    mHeights->AddGradientPoint (  0.5 + seaLevelInMeters, noise::utils::Color (255, 255, 255, 255));
    mHeights->AddGradientPoint (  0.75 + seaLevelInMeters, noise::utils::Color (128, 255, 255, 255));
    mHeights->AddGradientPoint ( 2.0 + seaLevelInMeters, noise::utils::Color (  0,   0, 255, 255));
    
    mPlanetTexture = new DRTextur;
    
    mUpdateTextureThreadSemaphore = SDL_CreateSemaphore(0);
    mUpdateTextureMutex = SDL_CreateMutex();
    
    updateTexture(DRVector2(2048.0f, 2048.0f), TextureGenerateThread, true);
}

RenderPlanet::~RenderPlanet()
{
    if(mUpdateTextureThread)
    {
        int returnValue = 0;
        SDL_WaitThread(mUpdateTextureThread, &returnValue);
        if(returnValue)
        {
            LOG_WARNING("Fehler in Thread occured");
            DRLog.writeToLog("Thread return with error: %d", returnValue);            
        }
        //SDL_KillThread(mUpdateThread);
        mUpdateTextureThread = NULL;
        SDL_DestroySemaphore(mUpdateTextureThreadSemaphore);
        SDL_DestroyMutex(mUpdateTextureMutex);
    }
    
    DR_SAVE_DELETE(mHeights);
    DR_SAVE_DELETE(mPlanetTexture);
    DR_SAVE_DELETE_ARRAY(mTextureTempPoints);
    mNoiseGenerator = NULL;
}

DRReturn RenderPlanet::render(float fTime, Camera* cam)
{
    if(SDL_SemTryWait(mUpdateTextureThreadSemaphore) == 0)
	{
        SDL_mutexP(mUpdateTextureMutex);
        if(mTextureTempPoints)
            if(mPlanetTexture->loadFromMemory(mTextureTempPoints, mTextureSize, GL_NEAREST, GL_NEAREST))
                LOG_WARNING("Fehler bei texture load from memory");
        SDL_mutexV(mUpdateTextureMutex);

        int returnValue = 0;
        SDL_WaitThread(mUpdateTextureThread, &returnValue);
        if(returnValue)
        {
            LOG_WARNING("Fehler in Thread occured");
            DRLog.writeToLog("Thread return with error: %d", returnValue);            
        }
        mUpdateTextureThread = NULL;
    }
    
    uint quadricDetails = 32;
    if(mDetailLevel > 9) quadricDetails = 256; // 10
    else if(mDetailLevel > 6) quadricDetails = 128; //7
    else if(mDetailLevel > 2) quadricDetails = 64; // 3
    else if(mDetailLevel > 0.0f)  quadricDetails = 32; // 1
    else quadricDetails = 8;
    
    updateTexture(DRVector2(128.0f, 256.0f), TextureGenerateThread);
    //glTranslatef(0.0f, 0.0f, -4.0f);
    mPlanetTexture->bind();
    gluSphere(GlobalRenderer::Instance().getQuadric(), 1.0f, quadricDetails*2, quadricDetails);
    
    if(DRGrafikError("[RenderPlanet::render]")) return DR_ERROR;
    return DR_OK;
}

int RenderPlanet::TextureGenerateThread(void* data)
{
    RenderPlanet* t = (RenderPlanet*)data;
    if(!t->mNoiseGenerator) LOG_ERROR("kein Noise Generator", -1);
    
    SDL_mutexP(t->mUpdateTextureMutex);
    int size = t->mTextureSize.x*t->mTextureSize.y;
    
    // First, create a spherical-noise-map builder.
    noise::utils::NoiseMapBuilderSphere planet;
    noise::utils::NoiseMap elevGrid;

    // Pass in the boundaries of the elevation grid to extract.
    planet.SetBounds (-90, 90, -180, 180);
    planet.SetDestSize (t->mTextureSize.x, t->mTextureSize.y);

    // Build the elevation grid with the output values from the final-planet
    // group.
    planet.SetSourceModule (*t->mNoiseGenerator->getOutModul());
    planet.SetDestNoiseMap (elevGrid);
    planet.Build ();
    
    double seaLevelInMeters = t->mNoiseGenerator->getSeaLevelInMetres();
    noise::utils::Image destImage;
    noise::utils::RendererImage imageRenderer;
    imageRenderer.SetSourceNoiseMap (elevGrid);
    imageRenderer.SetDestImage (destImage);
    imageRenderer.ClearGradient ();
    imageRenderer.AddGradientPoint (-2.0 + seaLevelInMeters, noise::utils::Color (  0,   0,   0, 255));
    imageRenderer.AddGradientPoint (    -0.03125 + seaLevelInMeters, noise::utils::Color (  6,  58, 127, 255));
    imageRenderer.AddGradientPoint (    -0.0001220703 + seaLevelInMeters, noise::utils::Color ( 14, 112, 192, 255));
    imageRenderer.AddGradientPoint (     0.0 + seaLevelInMeters, noise::utils::Color ( 70, 120,  60, 255));
    imageRenderer.AddGradientPoint (  0.125 + seaLevelInMeters, noise::utils::Color (110, 140,  75, 255));
    imageRenderer.AddGradientPoint (  0.25 + seaLevelInMeters, noise::utils::Color (160, 140, 111, 255));
    imageRenderer.AddGradientPoint (  0.375 + seaLevelInMeters, noise::utils::Color (184, 163, 141, 255));
    imageRenderer.AddGradientPoint (  0.5 + seaLevelInMeters, noise::utils::Color (255, 255, 255, 255));
    imageRenderer.AddGradientPoint (  0.75 + seaLevelInMeters, noise::utils::Color (128, 255, 255, 255));
    imageRenderer.AddGradientPoint ( 2.0 + seaLevelInMeters, noise::utils::Color (  0,   0, 255, 255));
    imageRenderer.EnableLight (true);
    imageRenderer.SetLightContrast (2.0 );
    imageRenderer.SetLightIntensity (2.0);
    imageRenderer.SetLightElev (90.0);
    imageRenderer.SetLightAzimuth (135.0);
    imageRenderer.Render ();
    
	DR_SAVE_DELETE_ARRAY(t->mTextureTempPoints);
    t->mTextureTempPoints = new DRColor[size];
    
    for(int i = 0; i < size; i++)
    {
        //DRVector3 point = DRRandom::rVector3(1.0f);
		noise::utils::Color h = destImage.GetConstSlabPtr()[i];
        t->mTextureTempPoints[i] = DRColor(h.red, h.green, h.blue, h.alpha);
    }
    SDL_mutexV(t->mUpdateTextureMutex);                

	SDL_SemPost(t->mUpdateTextureThreadSemaphore);
	return 0;
}

DRReturn RenderPlanet::updateTexture(DRVector2 newSize, int (*function)(void*), bool waitToComplete/* = false*/)
{
    int returnValue = 0;
	if(mUpdateTextureThread && waitToComplete)
	{
        SDL_WaitThread(mUpdateTextureThread, &returnValue);
        if(returnValue)
        {
            LOG_WARNING("Fehler in Thread occured");
            DRLog.writeToLog("Thread return with error: %d", returnValue);            
        }
        mUpdateTextureThread = NULL;
	}
	else if(mUpdateTextureThread) return DR_OK;
    
    double fractpart, intpart;
    fractpart = modf (newSize.x, &intpart);
    int ipart = (int)intpart;
    // positive x is a power of two \scriptstyle\Leftrightarrow (x & (x − 1)) equals zero. 
    // Quelle: http://en.wikipedia.org/wiki/Power_of_two#Fast_algorithm_to_check_if_a_positive_number_is_a_power_of_two
    if(fractpart > 0.0f || (ipart & (ipart - 1)) != 0) 
        LOG_ERROR("newSize isn't a power of 2", DR_ERROR);

    if(newSize.x > mTextureSize.x*2.0f)
        mTextureSize.x *= 2.0f;
    else 
        mTextureSize.x = newSize.x;
    mTextureSize.y = mTextureSize.x*2.0f;

    
#if SDL_VERSION_ATLEAST(1,3,0)
	mUpdateTextureThread = SDL_CreateThread(function, "RePlTex", this);
#else
	mUpdateTextureThread = SDL_CreateThread(function, this);
#endif

	if(waitToComplete)
	{
        SDL_WaitThread(mUpdateTextureThread, &returnValue);
        if(returnValue)
        {
            LOG_WARNING("Fehler in Thread occured");
            DRLog.writeToLog("Thread return with error: %d", returnValue);            
        }
		mUpdateTextureThread = NULL;
        SDL_SemWait(mUpdateTextureThreadSemaphore);
        // things to do after ending
        if(mPlanetTexture->loadFromMemory(mTextureTempPoints, mTextureSize))
            LOG_ERROR("Fehler bei load texture from memory", DR_ERROR);
	}
		
	return DR_OK;
}