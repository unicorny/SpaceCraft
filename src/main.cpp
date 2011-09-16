#include "main.h"

// Globale Variablen
Player g_Player;
RenderBlockLoader g_RenderBlockLoader;
Camera g_cam(DRVector3(0.0f, -5.0f, 0.0f), DRVector3(0.0f));
DRFont* g_Font = NULL;
DRTextur* g_tex = NULL;
int blockCount = 100;
//DRGeometrieIcoSphere g_geo;

GLint sphereList = 0;

int rekursionTest(int zahl)
{
    int z[20];
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
    
    DRLog.writeMatrixToLog(m1);
    DRLog.writeMatrixToLog(m2);
    DRLog.writeMatrixToLog(m3);
    
    DRLog.writeToLog("RekursionTest: %d", rekursionTest(0));
    
    
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
}
DRReturn generateSphere(DRReal radius);
DRReturn load()
{
    if(EnInit_Simple())
        return DR_ERROR;
    DRFileManager::Instance().addOrdner("data/blockView");
    test();
        
    DRRandom r;
    if(g_Player.init())
        LOG_ERROR("Fehler bei Player::init", DR_ERROR);
       
    if(g_RenderBlockLoader.init())
        LOG_ERROR("Fehler bei RenderBloockLoader::init", DR_ERROR);
     
    //if(EnInit_OpenGL(1.0f, DRVideoConfig(800, 600), "Space Craft - Techdemo"))
    if(EnInit_INI("data/config.ini"))
        LOG_ERROR("Fehler bei init OpenGL", DR_ERROR);    

    glClearColor(0.1, 0.2, 0.0, 0);
    g_Font = new DRFont();
    g_Font->init("data/MalgunGothic.tga", "data/MalgunGothic.tbf");
    
  //  glShadeModel(GL_SMOOTH);
  //  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
   // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    //Reseten der Matrixen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(40.0f, (GLfloat)XWIDTH/(GLfloat)YHEIGHT, 0.1f, 300.0f);
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
    
    Uint32 start = SDL_GetTicks();
    generateSphere(5.0f);
    
    DRLog.writeToLog("%.0f Sekunden fuer Planeten laden/generieren", ((float)SDL_GetTicks()-start)/1000.0f);

    return DR_OK;
}

void ende()
{
    DR_SAVE_DELETE(g_tex);
    glDeleteLists(sphereList, 1);
    DR_SAVE_DELETE(g_Font);
    g_RenderBlockLoader.exit();
    g_Player.exit();
    EnExit();
}

DRReturn move(float fTime)
{
    float fRotSpeed = 2.0f;
    float fSpeed = 20.0f;
    //Kamera
    Uint8 *keystate = SDL_GetKeyState(NULL);
    int mouseMove_x = 0, mouseMove_y = 0;
    // holen der Maus bewegung seit letztem frame und der bitmaks welche Tasten gedrückt sind
    Uint8 mousePressed = SDL_GetRelativeMouseState(&mouseMove_x, &mouseMove_y);

     // die Kamera wird rotiert, gesteuert durch die Tasten w, s (x Achse, hoch/runter), <-, -> (y Achse links/rechts), e und q (z Achse seitlich)
    g_cam.rotateRel(DRVector3(keystate[SDLK_w]-keystate[SDLK_s], keystate[SDLK_LEFT]-keystate[SDLK_RIGHT], keystate[SDLK_e]-keystate[SDLK_q])*fTime);
    // wenn die rechte maustaste gedrückt ist
    if((mousePressed & 4) == 4)
        // wird die Kamera auch abhängig von der Mausposition gedreht
    g_cam.rotateRel(DRVector3(-mouseMove_y, -mouseMove_x, 0.0f)*fTime*fRotSpeed);
    // kamera bewegung abhängig von den Tasten a, d (links/rechts), bild up, bild down (hoch/runter), Pfeil hoch und Pfeil runter (vorwärts/rückwärts)
    g_cam.translateRel(DRVector3(keystate[SDLK_a]-keystate[SDLK_d], keystate[SDLK_PAGEDOWN]-keystate[SDLK_PAGEUP], keystate[SDLK_UP]-keystate[SDLK_DOWN])*fTime*fSpeed);

        
    //if(EnIsButtonPressed(SDLK_z)) blockCount++;
    if(keystate[SDLK_z]) blockCount++;

    return DR_OK;
}
const float tao = 1.61803399;

DRReturn generateSphere(DRReal radius)
{
    float percent = 1.0f;
    const int iterator = 10000;
    
    const int totalSegments = 1000;
    const int currentSegments = (int)((float)totalSegments*percent);
        
    const int vertexCount = currentSegments*currentSegments;
    const int indexCount =  2*currentSegments*currentSegments-2*currentSegments; //2*currentSegments*currentSegments;//2*currentSegments-1+2*currentSegments*currentSegments;
//    const int iSegments = 200;
//    const int segs = 200;
    printf("vertexCount: %d, indexCount: %d, currentSegments: %d\n", vertexCount, indexCount, currentSegments);
    
    //DRGeometrieIcoSphere geo2;        
    DRGeometrieIcoSphere geo;        
    //DRGeometrieSphere geo;    
    geo.initIcoSphere(2);
    //geo.initSphere(totalSegments);
    //geo.makeSphericalLandscape(iterator, 7157);
    
    
    //if(geo.initSphere(totalSegments))
      //  LOG_ERROR("Fehler bei SphereInit", DR_ERROR);
    
    DRVector3* points = geo.getVertexPointer();// new DRVector3[vertexCount];
    DRColor* color = geo.getColorPointer();// new DRColor[vertexCount];
    DRColor* heighMap = new DRColor[vertexCount];
    GLuint*  indices = geo.getIndexPointer();    
    
    //*/
            
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDisable(GL_CULL_FACE);
    
    //glBegin(GL_LINE_LOOP);
  
    /*
    const char* path = "data/planet.png";
  
    DRIImage* image = DRIImage::newImage();
    DRReturn ret = image->loadFromFile(path);
    if(!ret && image->getWidth()*image->getHeight() == vertexCount)
    {
        image->getPixel(color);
        int size = image->getWidth()*image->getHeight();
        printf("size: %u\n", size);        
    }
    else
    {
        geo.makeSphericalLandscape(iterator, 1119);
        float max = 0.0f, min = 1.0f;
        for(int i = 0; i < vertexCount; i++)
        {
            float l = 1.0f - points[i].length();
            if(l > max) max = l;
            if(l < min) min = l;
        }
        //einfärben
        DRLog.writeToLog("min: %f, max: %f", min, max);
        for(int j = 0; j < vertexCount; j++)
        {
        //    DRLog.writeToLog("\n---------- j:%d -------------", j);
            float d = 1.0f - points[j].length();
            heighMap[j] = DRColor((DRReal)(fabs((d+min))/(max+fabs(min))));
    //        if(j < 10)
      //          DRLog.writeToLog("heighMapValue: %f, d: %f", fabs((d+min))/(max+fabs(min)), d);
            d *= -1.0f;
            //if(j < 10)
          //      DRLog.writeToLog("d: %f", d);
            if(d < 0)
            {
                if(d == -1.0f)
                    color[j] = color[(int)fabs(vertexCount-currentSegments-j)].interpolate(color[j-currentSegments], 0.5f);
                else
                    color[j] = DRColor(0.0f, (1.0f-(d/min))/10.0f, 1.0f-(d/min));
            }
            else if(d > 0)
            {
                if(d > max/2)
                    color[j] = DRColor((d/max), (d/max)/2.0f, 0.1f);
                else if(d < max-max/10)
                    color[j] = DRColor((d/max)/2.0f, (d/max), 0.1f);
                else
                    color[j] = DRColor(d/max);
            }
            //DRLog.writeVector3ToLog(points[j]);
            //DRLog.writeColorToLog(color[j]);
           // points[j] = points[j].normalize();
        }
        //DRLog.writeToLog("\n-------- ende -------------");
        printf("%d Pixel in ein %d großes Bild\n", vertexCount, currentSegments * (currentSegments+2));
        image->setWidth(currentSegments);
        image->setHeight(currentSegments);
        image->setImageFormat(-1);
        image->setPixel(color);
        image->saveIntoFile(path);    
        
        image->setPixel(heighMap);
        image->saveIntoFile("data/heighmap.png");
    } 
    
    g_tex = new DRTextur(image);
    DRIImage::deleteImage(image);    
   // */
    
    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);     
    
    glPushMatrix();          
    glScalef(radius, radius, radius);
    glColor3f(1.0f, 0.0f, 0.0f);
    geo.render();
    
    glPopMatrix();
    
    glEndList();
    
 //   DR_SAVE_DELETE_ARRAY(points);
//    DR_SAVE_DELETE_ARRAY(color);
//    DR_SAVE_DELETE_ARRAY(indices);
    DR_SAVE_DELETE_ARRAY(heighMap);
    
//    g_tex = new DRTextur(path, GL_NEAREST, GL_NEAREST);
   
    
    return DR_OK;
}

DRReturn render(float fTime)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();             //a clear the matrix
    g_cam.setKameraMatrix();
    
    //light
    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
      //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glEnable(GL_LIGHT0);
    
    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glEnable(GL_LIGHT1);       
    
    if(g_tex)
        g_tex->bind();
      
    //glColor3f(0.2f, 0.5f, 0.1f);
    glColor3f(1.0f, 1.0f, 1.0f);
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
    RenderBlock* rb =  g_RenderBlockLoader.getRenderBlock("dirt");
    rb->render();
    
    glTranslatef(0.0f, -5.0f, 0.0f);
    rb = g_RenderBlockLoader.getRenderBlock("dirG");
    rb->render();
    glTranslatef(1.0f, 0.0f, 0.0f);
    rb->render();
    glTranslatef(0.0f, 2.0f, 0.0f);
    g_RenderBlockLoader.getRenderBlock("benc")->render();
    
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_LIGHTING);
    glPushMatrix();
    
    glTranslatef(0.0, 0.0f, -15.0f);
    //renderSphere(5.0f);
    
    static float sphereRotate = 0;
    glRotatef(sphereRotate, 0.0f, 1.0f, 0.0f);
    
    if(sphereList)
        glCallList(sphereList);
    //g_geo.render();
    
    sphereRotate += fTime*10.0f;
      
    
    glPopMatrix();
    //*/
    static u32 start = 0;
    float dir[] = {1.0f, 1.0f};
    int y = 0;
    const int length = 250;
    for(int i = 0; i < blockCount; i++)
    {   
        
        if(!(i % 10))
        {
            glTranslatef(0.0f, 1.0f, 0.0f);
            rb->render();
            glTranslatef(0.0f, -1.0f, 0.0f);
        }
        if(!(i % length))
        {
            if(!(y % length))
            {
                glTranslatef(0.0f, 1.0f, 0.0f);
                dir[1] *= -1.0f;
                dir[0] *= -1.0f;
            }
            else
            {
                glTranslatef(0.0f, 0.0f, 1.0f*dir[1]);
                dir[0] *= -1.0f;
             
            }
            y++;
            
        }
        else
        {
            glTranslatef(1.0f*dir[0], 0.0f, 0.0f);
        }
        rb->render();
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
    
    g_Font->end();
    
    start = SDL_GetTicks();
    

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
