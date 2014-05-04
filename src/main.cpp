#include "main.h"
#include "ShaderManager.h"
#include "DRGeometrieManager.h"
#include "DRTextureManager.h"
#include "GlobalRenderer.h"
#include "RenderBlockLoader.h"
#include "Camera.h"
#include "Player.h"

// for test only
#include "RenderNoisePlanetToTexture.h"
#include "RenderTextureToTexture.h"
#include "RenderSubPlanet.h"
#include "SubPlanetSektor.h"

struct ControlMode
{
    ControlMode() {}
    ControlMode(Unit value, float time) : mValue(value), mTime(time) {}
    ControlMode(double value, UnitTypes type, float time) : mValue(value, type), mTime(time) {}
    Unit mValue;
    float mTime;
};

// Globale Variablen
Player g_Player;
RenderBlockLoader g_RenderBlockLoader;
Camera* g_cam = NULL;
std::list<Camera*> g_Cameras;
DRFont* g_Font = NULL;
DRTexturePtr g_tex;
DRTexturePtr g_terrain;
int blockCount = 0;
#define MAX_CONTROL_MODES 9
ControlMode gControlModes[MAX_CONTROL_MODES];
int gCurrentControlMode = 0;
// Debug
bool            wireframe = false;
Unit            g_distanceToGround;



int rekursionTest(int zahl)
{
//    int z[20];
    if(zahl > 10000) return zahl;
    return rekursionTest(++zahl);
}

void test()
{
    //tests
    DRMatrix m1(DRMatrix::identity());
    float mat[] = {1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f};
    if(memcmp(m1, mat, sizeof(float)*16) != 0)
        LOG_WARNING("matrix identity isn't valid");
    DRMatrix m2 = m1.rotationX(30.0f);
    DRMatrix m3 = DRMatrix::axis(DRVector3(1.0f, 0.0f, 0.0f),
                                 DRVector3(0.0f, 1.0f, 0.0f),
                                 DRVector3(0.0f, 0.0f, 1.0));
    if(memcmp(m1, m3, sizeof(float)*16) != 0)
        LOG_WARNING("matrix axis isn't valid");
    
    DREngineLog.writeMatrixToLog(m1);
    DREngineLog.writeMatrixToLog(m2);
    DREngineLog.writeMatrixToLog(m3);
    
    DRVector3 rot1(1.0f, 0.0f, 0.0f);
    m2 = DRMatrix::rotationY(90.0f);
    rot1 = rot1.transformCoords(m2);
    DREngineLog.writeVector3ToLog(rot1, "1/0/0 90 Grad um y-Achse rotiert");
    rot1 = rot1.transformCoords(m2.invert());
    DREngineLog.writeVector3ToLog(rot1, "zurueckrotiert, 1/0/0 erwartet!");
    
    DREngineLog.writeToLog("RekursionTest: %d", rekursionTest(0));
    
    
    //Speicher test
/*  LOG_INFO("Speichertest");
    std::list<void*> pointer;
    void* t = NULL;
    u32 count = 0;
    do
    {
        t = malloc(16384);
        if(t) pointer.push_back(t);
        count++;
        if(count > 192073)
            break;
    } while(t);
    
    DRLog.writeToLog("count: %d, %u kByte wurden reserviert!", count, count*16384/1024);
    
    for(std::list<void*>::iterator it = pointer.begin(); it != pointer.end(); it++)
        free(*it);
    pointer.clear();
   //* */
    
    // Unit test
    printf("\n");
    Unit parsec(1.0, PARSEC);
    Unit lj = parsec.convertTo(LIGHTYEAR);
    DREngineLog.writeToLog("%s -> %s", parsec.print().data(), lj.print().data());    
    lj = Unit(1.0, LIGHTYEAR);
    parsec = lj.convertTo(PARSEC);
    DREngineLog.writeToLog("%s -> %s", lj.print().data(), parsec.print().data());    
    Unit ae = lj.convertTo(AE);
    DREngineLog.writeToLog("%s -> %s", lj.print().data(), ae.print().data());    
    ae = parsec.convertTo(AE);
    DREngineLog.writeToLog("%s -> %s", parsec.print().data(), ae.print().data());    
    parsec = ae.convertTo(PARSEC);
    DREngineLog.writeToLog("%s -> %s", ae.print().data(), parsec.print().data());    
    Unit m = parsec.convertTo(M);
    DREngineLog.writeToLog("%s -> %s", parsec.print().data(), m.print().data());    
    Unit kpc(1.0, KILOPARSEC);
    m = kpc.convertTo(M);
    DREngineLog.writeToLog("%s -> %s", kpc.print().data(), m.print().data());    
    m = Unit(1.0, M);
    kpc = m.convertTo(KILOPARSEC);
    DREngineLog.writeToLog("%s -> %s", m.print().data(), kpc.print().data());    
    printf("\n");
    
    Unit aes(0.005, AE);
    DREngineLog.writeToLog("%s -> %s", aes.print().data(), aes.convertTo(KM).print().data());
    
    //Vector Unit Test
    Vector3Unit u1(100, 200, 70, M), u2(1, 0, 0, KILOPARSEC), u3(100, 20, 17, LIGHTYEAR);
    u1.print("u1");
    u2.print("u2");
    u3.print("u3");
    
    u1 *= Unit(20, KM);
    u1.print("u1* 20 km");
    
    Vector3Unit(u1 + u2).print("u1+u2");
    Vector3Unit(u2+u3).print("u2+u3");
    Vector3Unit(u1*Unit(1, LIGHTYEAR)).print("u1*1 Lichtjahr");
    
    DRVector3 v(1.0f, 7.0f, 2.0f);
    DREngineLog.writeVector3ToLog(v, "init");
    v = v.normalize();
    DREngineLog.writeVector3ToLog(v, "normalized");
    v *= 7.0f;
    DREngineLog.writeVector3ToLog(v, "multiplikator");
    
    // ----------------------------------  ReferenzHolder Test --------------------------------
    
    DREngineLog.writeToLog("DRIndexReferenzHolder test");
    DRIndexReferenzHolder referenzHolder(10);
    uint tests[10];
    tests[0] = referenzHolder.getFree();
    referenzHolder.add(tests[0]);
    tests[1] = referenzHolder.getFree();
    
    DREngineLog.writeToLog("index1 (0): %d, index2 (1): %d", tests[0], tests[1]);
    referenzHolder.remove(tests[0]);
    tests[2] = referenzHolder.getFree();
    referenzHolder.remove(tests[1]);
    tests[3] = referenzHolder.getFree();
    DREngineLog.writeToLog("index3 (2): %d, index4 (1): %d", tests[2], tests[3]);
    for(int i = 0; i < 5; i++)
        tests[4+i] = referenzHolder.getFree();
    referenzHolder.remove(tests[7]);
    tests[9] = referenzHolder.getFree();
    DREngineLog.writeToLog("index10: (6): %d", tests[9]);
    
    DRTextureManager::Instance().test();
    
    // Random Test
    
}

void sizeOfClasses()
{
    DREngineLog.writeToLog("--------  Klassen-Objekt groessen (in Bytes): -----------");
    DREngineLog.writeToLog("char: %d", sizeof(char));
    DREngineLog.writeToLog("short: %d", sizeof(short));
	DREngineLog.writeToLog("int: %d", sizeof(int));
    DREngineLog.writeToLog("long: %d", sizeof(long));
    DREngineLog.writeToLog("void*: %d", sizeof(void*));
    DREngineLog.writeToLog("float: %d", sizeof(float));
    DREngineLog.writeToLog("double: %d", sizeof(double));
    
    DREngineLog.writeToLog("Camera: %d", sizeof(Camera));
    DREngineLog.writeToLog("DRGeometrieIcoSphere: %d", sizeof(DRGeometrieIcoSphere));
    DREngineLog.writeToLog("DRGeometrieHeightfield: %d", sizeof(DRGeometrieHeightfield));
	DREngineLog.writeToLog("DRMatrix: %d", sizeof(DRMatrix));
    DREngineLog.writeToLog("DRSaveTexture: %d", sizeof(DRSaveTexture));
    DREngineLog.writeToLog("DRTexture: %d", sizeof(DRTexture));
	DREngineLog.writeToLog("DRTextureManager: %d", sizeof(DRTextureManager));
    DREngineLog.writeToLog("DRVector3: %d", sizeof(DRVector3));
    DREngineLog.writeToLog("HeightMapTexture: %d", sizeof(HeightMapTexture));
    DREngineLog.writeToLog("PlanetSektor: %d", sizeof(PlanetSektor));
    DREngineLog.writeToLog("RenderSektor: %d", sizeof(RenderSektor));
    DREngineLog.writeToLog("RenderPlanet: %d", sizeof(RenderPlanet));
    DREngineLog.writeToLog("RenderSubPlanet: %d", sizeof(RenderSubPlanet));
    DREngineLog.writeToLog("RenderInStepsToTexture: %d", sizeof(RenderInStepsToTexture));
    DREngineLog.writeToLog("RenderNoisePlanetToTexture: %d", sizeof(RenderNoisePlanetToTexture));
    DREngineLog.writeToLog("RenderTextureToTexture: %d", sizeof(RenderTextureToTexture));
    DREngineLog.writeToLog("Sektor: %d", sizeof(Sektor));
    DREngineLog.writeToLog("SektorObject: %d", sizeof(SektorObject));
    DREngineLog.writeToLog("SubPlanetSektor: %d", sizeof(SubPlanetSektor));
    DREngineLog.writeToLog("Unit: %d", sizeof(Unit));
    DREngineLog.writeToLog("UnitTypes: %d", sizeof(UnitTypes));
    DREngineLog.writeToLog("Vector3Unit: %d", sizeof(Vector3Unit));
    DREngineLog.writeToLog("------- Klassen-Objekt groessen Ende ----------");
}

void initPermTexture(/*GLuint *texID*/)
{
  char *pixels;
  int i,j;

  int perm[256]= {151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

  int grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
                     {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
                     {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
                     {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16

  //glGenTextures(1, texID); // Generate a unique texture ID
  //glBindTexture(GL_TEXTURE_2D, *texID); // Bind the texture to texture unit 0

  pixels = (char*)malloc( 256*256*4 );
  for(i = 0; i<256; i++)
    for(j = 0; j<256; j++) {
      int offset = (i*256+j)*4;
      char value = perm[(j+perm[i]) & 0xFF];
      pixels[offset] = grad3[value & 0x0F][0] * 64 + 64;   // Gradient x
      pixels[offset+1] = grad3[value & 0x0F][1] * 64 + 64; // Gradient y
      pixels[offset+2] = grad3[value & 0x0F][2] * 64 + 64; // Gradient z
      pixels[offset+3] = value;                     // Permuted index
    }
  DRIImage* image = DRIImage::newImage();
  image->setImageFormat(-1);
  image->setSize(DRVector2i(256));
  image->setPixel(reinterpret_cast<u8*>(pixels));
  image->saveIntoFile("data/permTexture.tga");
  DRIImage::deleteImage(image);
  /*/
  // GLFW texture loading functions won't work here - we need GL_NEAREST lookup.
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  */
  free(pixels);
}

DRReturn load()
{
    if(EnInit_Simple())
        return DR_ERROR;
    DRFileManager::Instance().addOrdner("data/blockView");
    DRFileManager::Instance().addOrdner("data/shader");
    test();
    sizeOfClasses();
    
    //Steuerung
    u16 i = 0;
    gControlModes[i++] = ControlMode(Unit(20, M), 120.0f);
    gControlModes[i++] = ControlMode(Unit(0.100, KM), 100.0f);
    gControlModes[i++] = ControlMode(Unit(1, KM), 90.0f);
    gControlModes[i++] = ControlMode(Unit(10, KM), 80.0f);
    gControlModes[i++] = ControlMode(Unit(100, KM), 60.0f);
    gControlModes[i++] = ControlMode(Unit(1000, KM), 30.0f);
    gControlModes[i++] = ControlMode(Unit(20000, KM), 10.0f);
    gControlModes[i++] = ControlMode(Unit(400000, KM), 4.0f);
    gControlModes[i++] = ControlMode(Unit(0.1, AE), 1.0f);
    gControlModes[i++] = ControlMode(Unit(1, AE), 1.0f);
    //gControlModes[i++] = ControlMode(Unit(10, AE), 1.0f);
    //gControlModes[i++] = ControlMode(Unit(500, AE), 1.0f);
     
    //if(EnInit_OpenGL(1.0f, DRVideoConfig(800, 600), "Space Craft - Techdemo"))
    if(EnInit_INI("data/config.ini"))
        LOG_ERROR("Fehler bei init OpenGL", DR_ERROR);       
	LOG_WARNING_SDL();
    
    //check maximal multitextures
    int textureCount = 0;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &textureCount);
    if(textureCount < 4) LOG_ERROR("Multitexturing mit mindestens 4 Texturen wird nicht unterstuetzt!", DR_ERROR);
    DREngineLog.writeToLog("Multitexturing supported up to: %d textures\n", textureCount);
    
    g_Font = new DRFont();
    g_Font->init("data/MalgunGothic.tga", "data/MalgunGothic.tbf");

    DREngineLog.writeToLog("CPU-Count: %d", g_CPU_Count);   
    
    
    
  //  glShadeModel(GL_SMOOTH);
  //  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
   // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    //Reseten der Matrixen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(40.0f, (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.1f, 2000.0f);
    glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix

    glLoadIdentity();                    // Reset (init) the modelview matrix


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);             // Enable smooth shading
    glClearDepth(1.0f);                  // Clear depth buffer
    glEnable(GL_DEPTH_TEST);             // Enables depth test
    glDepthFunc(GL_LEQUAL);              // Type of depth test to perform

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glLineWidth(1.0f);
    glEnable(GL_NORMALIZE);

    // Enhances image quality
    glColor3f(0.5f, 0.5f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    //glEnable(GL_LIGHTING);
    glDisable(GL_FOG);
    
    initPermTexture();
    
	if(GlobalRenderer::getSingleton().init("data/config.ini"))
		LOG_ERROR("error by init GlobalRenderer", DR_ERROR);
    
    if(ShaderManager::getSingleton().init())
        LOG_ERROR("error by init ShaderManager", DR_ERROR);
    if(DRGeometrieManager::getSingleton().init())
        LOG_ERROR("error by init DRGeometrieManager", DR_ERROR);

    if(g_Player.init())
        LOG_ERROR("Fehler bei Player::init", DR_ERROR);
    g_cam = g_Player.getCamera();
    g_Cameras.push_back(g_cam);
       
    if(g_RenderBlockLoader.init())
        LOG_ERROR("Fehler bei RenderBloockLoader::init", DR_ERROR);
    
    Uint32 start = SDL_GetTicks();
    //g_terrain = new DRTextur("data/terrainsurface.bmp", GL_NEAREST, GL_NEAREST);
    DREngineLog.writeToLog("%.0f Sekunden fuer Planeten laden/generieren", ((float)SDL_GetTicks()-start)/1000.0f);

    return DR_OK;
}

void ende()
{
    g_Cameras.pop_back();
    g_Player.exit();
    g_tex.release();    
    DR_SAVE_DELETE(g_Font);
    g_terrain.release();
	GlobalRenderer::getSingleton().exit();
    ShaderManager::getSingleton().exit();
    DRGeometrieManager::getSingleton().exit();
    g_RenderBlockLoader.exit();
    Server::freeAllServer();	
    EnExit();
}

DRReturn move(float fTime)
{
    float fRotSpeed = 2.0f;
    float fSpeed = 20.0f;
	int numKeys = 0;
    //Kamera
#if SDL_VERSION_ATLEAST(1,3,0)
	const Uint8 *keystate = SDL_GetKeyboardState(&numKeys);
#else
	Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
	
	/*for(uint i = 0; i < numKeys; i++)
	{
		if(keystate[i]) DRLog.writeToLog("%d button pressed: %d", i, (int)keystate[i]);
	}
	//*/
//	return DR_OK;
    
    int mouseMove_x = 0, mouseMove_y = 0;
    // holen der Maus bewegung seit letztem frame und der bitmaks welche Tasten gedrückt sind
    Uint8 mousePressed = SDL_GetRelativeMouseState(&mouseMove_x, &mouseMove_y);

    //if(gCurrentControlMode != 0 )
    //{
         // die Kamera wird rotiert, gesteuert durch die Tasten w, s (x Achse, hoch/runter), <-, -> (y Achse links/rechts), e und q (z Achse seitlich)
    g_cam->rotateRel(DRVector3(keystate[SDL_SCANCODE_S]-keystate[SDL_SCANCODE_W],
                               keystate[SDL_SCANCODE_RIGHT]-keystate[SDL_SCANCODE_LEFT],
                               keystate[SDL_SCANCODE_Q]-keystate[SDL_SCANCODE_E])*fTime);
        // wenn die rechte maustaste gedrückt ist
        if((mousePressed & 4) == 4)
            // wird die Kamera auch abhängig von der Mausposition gedreht
        g_cam->rotateRel(DRVector3(-mouseMove_y, -mouseMove_x, 0.0f)*fTime*fRotSpeed);
    //}

    //if(gControlModes[gCurrentControlMode].mValue.getType() == M)
      //  g_cam->translateRel(DRVector3(keystate[SDLK_d]-keystate[SDLK_a], keystate[SDLK_PAGEUP]-keystate[SDLK_PAGEDOWN], keystate[SDLK_DOWN]-keystate[SDLK_UP])*fTime*gControlModes[gCurrentControlMode].mValue);
    //else
        g_cam->translateRel_SektorPosition(DRVector3(keystate[SDL_SCANCODE_D]-keystate[SDL_SCANCODE_A],
                                                     keystate[SDLK_PAGEUP]-keystate[SDLK_PAGEDOWN],
                                                     keystate[SDLK_DOWN]-keystate[SDLK_UP])
                                                     *fTime*gControlModes[gCurrentControlMode].mValue, gControlModes[gCurrentControlMode].mValue.getType());    
    
    //set control mode
    if(EnIsButtonPressed(SDL_SCANCODE_1)) gCurrentControlMode = 0;
    else if(EnIsButtonPressed(SDL_SCANCODE_2)) gCurrentControlMode = 1;
    else if(EnIsButtonPressed(SDL_SCANCODE_3)) gCurrentControlMode = 2;
    else if(EnIsButtonPressed(SDL_SCANCODE_4)) gCurrentControlMode = 3;
    else if(EnIsButtonPressed(SDL_SCANCODE_5)) gCurrentControlMode = 4;
    else if(EnIsButtonPressed(SDL_SCANCODE_6)) gCurrentControlMode = 5;
    else if(EnIsButtonPressed(SDL_SCANCODE_7)) gCurrentControlMode = 6;
    else if(EnIsButtonPressed(SDL_SCANCODE_8)) gCurrentControlMode = 7;
    else if(EnIsButtonPressed(SDL_SCANCODE_9)) gCurrentControlMode = 8;
    
    GlobalRenderer::Instance().setTimeForInactiveChild(gControlModes[gCurrentControlMode].mTime);
    g_Player.setCurrentSpeed(gControlModes[gCurrentControlMode].mValue);
     
    // R-Taste
    if(EnIsButtonPressed(SDL_SCANCODE_R)) wireframe = !wireframe;
    
    //if(EnIsButtonPressed(SDLK_z)) blockCount++;
    if(keystate[SDL_SCANCODE_Z]) blockCount++;
    
    if(fTime == 0.0f) fTime = 0.00166f;

    //if(g_Player.getSektor()->moveAll(fTime, g_cam))
    if(g_Player.getSektor()->updateVisibilityAll(g_Cameras))
        LOG_ERROR("Fehler bei update Visibility in sektors", DR_ERROR);
    if(g_Player.getSektor()->moveAll(fTime, g_Player.getCamera()))
        LOG_ERROR("Fehler bei move sektor", DR_ERROR);

    return DR_OK;
}


DRReturn render(float fTime)
{
    int w = 0, h = 0;
    SDL_GL_GetDrawableSize(g_pSDLWindow, &w, &h);
    glViewport(0, 0, w, h);
    
    glClearColor(0.1, 0.2, 0.0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    //if(g_terrain)
      //  g_terrain->bind();
    //if(g_Player.getSektor()->renderAll(fTime, g_Player.getCamera()))
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if(g_Player.getSektor()->renderAll(fTime, g_Player.getCamera()))
        LOG_ERROR("Fehler bei render sektor", DR_ERROR);
    ShaderProgram::unbind();
    glDisable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    
    glClear (GL_DEPTH_BUFFER_BIT);    
    glColor3f(1.0f, 1.0f, 1.0f);    
    
    //Reseten der Matrixen
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluPerspective(g_Player.getCameraFOV(), (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.1f, 2000.0f);
    glMultMatrixf(DRMatrix::perspective_projection(g_Player.getCameraFOV(), (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.1f, 2000.0f));
    DRFrustumCulling cull(g_cam, g_Player.getCameraFOV(), (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);          // Select the modelview matrix

    glLoadIdentity();                    // Reset (init) the modelview matrix
    DRVector3 translate(0.0f);
    g_cam->setCameraMatrix();
    glEnable(GL_DEPTH_TEST);             // Enables depth test
        
    //light
    //Add ambient light
    GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    //Add positioned light
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glDisable(GL_LIGHT0);
    
    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glEnable(GL_LIGHT1);       
    
    if(g_tex.getResourcePtrHolder())
        g_tex->bind();
      
    //glColor3f(0.2f, 0.5f, 0.1f);
 /*   glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0, 0.0f);
        glVertex3f( 50.0f, 0.0f, -50.0f);
        glTexCoord2f(0.0, 0.0f);
        glVertex3f(-50.0f, 0.0f, -50.0f);
        glTexCoord2f(0.0, 1.0f);
        glVertex3f(-50.0f, 0.0f,  50.0f);
        glTexCoord2f(1.0, 1.0f);
        glVertex3f( 50.0f, 0.0f,  50.0f);
    glEnd();
    
    //glDisable(GL_TEXTURE_2D);
    
    glTranslatef(0.0f, 2.0f, 0.0f);
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    glTranslatef(0.0f, -2.0f, 0.0f);
    
  //  printf("bevore renderBlock\n");
    
    glTranslatef(0.0f, 10.0f, 0.0f);
    translate.y += 10.0f;
	//*/
    RenderBlock* rb =  g_RenderBlockLoader.getRenderBlock("dirt");
	/*
    rb->render();
    
    glTranslatef(0.0f, -5.0f, 0.0f);
    translate.y -= 5.0f;
    rb = g_RenderBlockLoader.getRenderBlock("dirG");
    rb->render();
    glTranslatef(1.0f, 0.0f, 0.0f);
    translate.x += 1.0f;
    rb->render();
    glTranslatef(0.0f, 2.0f, 0.0f);
    translate.y += 2.0f;
    
	DRFrustumPosition res = cull.isBoxInFrustum(DRVector3(-0.5f), DRVector3(0.5f), DRMatrix::translation(translate));
    if(res != OUTSIDE)    
        g_RenderBlockLoader.getRenderBlock("benc")->render();
    
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_LIGHTING);
    
    //*/
    
    static u32 start = 0;
    float dir[] = {1.0f, 1.0f};
    int y = 0;
    const int length = 250;
    int clipCount = 0;
    int renderCount = 0;
    
    for(int i = 0; i < blockCount; i++)
    {           
        if(!(i % 10))
        {
            glTranslatef(0.0f, 1.0f, 0.0f);
            translate.y += 1.0f;
            
            if(cull.isSphereInFrustum(translate, 0.6f) != OUTSIDE)
            {
                rb->render();
                renderCount++;
            }
            else clipCount++;
            
            glTranslatef(0.0f, -1.0f, 0.0f);
            translate.y -= 1.0f;
        }
        if(!(i % length))
        {
            if(!(y % length))
            {
                glTranslatef(0.0f, 1.0f, 0.0f);
                translate.y += 1.0f;
                dir[1] *= -1.0f;
                dir[0] *= -1.0f;
            }
            else
            {
                glTranslatef(0.0f, 0.0f, 1.0f*dir[1]);
                translate.z += 1.0f*dir[1];
                dir[0] *= -1.0f;
             
            }
            y++;
        }
        else
        {
            glTranslatef(1.0f*dir[0], 0.0f, 0.0f);
            translate.x += 1.0f*dir[0];
        }
        if(cull.isSphereInFrustum(translate, 0.6f) != OUTSIDE)
        {
                rb->render();
                renderCount++;
        }
        else clipCount++;
    }
    u32 end = SDL_GetTicks();
          
    //FPS
    g_Font->begin();
        
    DRText text(g_Font);
    text.setFlags(DR_FF_RELATIVE | DR_FF_RELATIVSCALING);
    text.setText("FPS: %.0f", 1.0f/fTime);
    text.setPosition(DRVector2(0.0f, 0.0f));
    text.setColor12(DRColor(0.8f, 0.5f, 0.1f));
    text.setScaling(DRVector2(1.0f));
    text.drawText();    
    
    text.setText("Count: %d", blockCount);
    text.setPosition(DRVector2(0.0f, 0.04f));
    text.drawText();
    
    text.setText("milliseconds: %u", end-start);
    text.setPosition(DRVector2(0.1f, 0.0f));
    text.drawText();
    
    text.setText("Steuerung: %d - %s/s", gCurrentControlMode+1, gControlModes[gCurrentControlMode].mValue.print().data());
    text.setPosition(DRVector2(0.0f, 0.08f));
    text.drawText();
    
    text.setText("Hoehe: %s", g_distanceToGround.print().data());
    text.setPosition(DRVector2(0.0f, 0.12f));
    text.drawText();

	GlobalRenderer& gb = GlobalRenderer::Instance();
    DRTextureManager& tx = DRTextureManager::Instance();
    text.setColor12(DRColor(1.0f, 1.0f, 1.0f));
    text.setScaling(DRVector2(0.8f));
	text.setText("Grafik Memory: %.0f MByte", static_cast<double>(tx.getGrafikMemoryTexture()+gb.getGrafikMemoryGeometrie())/(1024.0f*1024.0));
	text.setPosition(DRVector2(0.8f, 0.0f));
	text.drawText();
    
	text.setText("Texture: %.0f MByte", static_cast<double>(tx.getGrafikMemoryTexture())/(1024.0f*1024.0));
	text.setPosition(DRVector2(0.8f, 0.04f));
	text.drawText();

	text.setText("Geometrie: %.0f MByte",static_cast<double>(gb.getGrafikMemoryGeometrie())/(1024.0f*1024.0));
	text.setPosition(DRVector2(0.8f, 0.08f));
	text.drawText();
    
    g_Font->end();
   
    start = SDL_GetTicks();    
    if(GlobalRenderer::Instance().renderTasks())
        LOG_ERROR("Fehler bei calling GlobalRenderer::renderTasks", DR_ERROR);

    return DR_OK;
}

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[])
{
    
    printf("Bitte warten, waehrend SpaceCraft geladen wird\n");
    if(load())
    {
        printf("Es trat ein Fehler bei load auf, das Programm wird beendet!\n");
        return -1;
    }

    if(EnGameLoop(move, render))
    {
        printf("Fehler in der GameLoop\n");
        return -1;
    }
   // * */
    ende();
    printf("Auf wiedersehen ;)\n");
    return 42;
}
