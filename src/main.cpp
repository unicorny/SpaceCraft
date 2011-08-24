#include "main.h"

// Globale Variablen
Player g_Player;
RenderBlockLoader g_RenderBlockLoader;
Camera g_cam(DRVector3(0.0f, -5.0f, 0.0f), DRVector3(0.0f));
DRFont* g_Font = NULL;
int blockCount = 100;

GLint sphereList = 0;

#define GRADTORAD 0.017453292f
#define RADTOGRAD 57.29577951f
const double PI = 3.1415926535;


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
   * */
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
     
    if(EnInit_OpenGL(1.0f, DRVideoConfig(800, 600), "Space Craft - Techdemo"))
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
    
    DRLog.writeToLog("%f Sekunden für generate Planet", ((float)SDL_GetTicks()-start)/1000.0f);

    return DR_OK;
}

void ende()
{
    glDeleteLists(sphereList, 1);
    DR_SAVE_DELETE(g_Font);
    g_RenderBlockLoader.exit();
    g_Player.exit();
    EnExit();
}

DRReturn move(float fTime)
{

    float fRotSpeed = 1.0f;
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
    g_cam.rotateRel(DRVector3(-mouseMove_y, -mouseMove_x, 0.0f)*fTime*fSpeed);
    // kamera bewegung abhängig von den Tasten a, d (links/rechts), bild up, bild down (hoch/runter), Pfeil hoch und Pfeil runter (vorwärts/rückwärts)
    g_cam.translateRel(DRVector3(keystate[SDLK_a]-keystate[SDLK_d], keystate[SDLK_PAGEDOWN]-keystate[SDLK_PAGEUP], keystate[SDLK_UP]-keystate[SDLK_DOWN])*fTime*fSpeed);

        
    //if(EnIsButtonPressed(SDLK_z)) blockCount++;
    if(keystate[SDLK_z]) blockCount++;

    return DR_OK;
}

DRReturn generateSphere(DRReal radius)
{
    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE); 
    
    glPushMatrix();
    
    glScalef(radius, radius, radius);
    glColor3f(0.0f, 1.0f, 1.0f);
    
    float percent = 0.4f;
    
    const int totalSegments = 200;
    const int currentSegments = (int)((float)totalSegments*percent);
    const int iSegments = 200;
    const int segs = 200;
    DRVector3 points[iSegments*(segs+2)];
    DRColor color[iSegments*(segs+2)];
    GLuint indices[2*iSegments-1+2*iSegments*segs];
    memset(indices, 0, sizeof(GLuint)*iSegments*(segs+2));    
    memset(color, 1, sizeof(DRColor)*iSegments*(segs+2));
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //  glDisable(GL_CULL_FACE);
    
    //glBegin(GL_LINE_LOOP);
    for(int i = 0; i < iSegments; i++)
    {
        DRReal sin = sinf(((PI)/(double)(iSegments-1))*(double)i);
        DRReal cos = cosf(((PI)/(double)(iSegments-1))*(double)i);        

        points[i] = DRVector3(cos, sin, 0.0f);
        color[i] = DRColor(1.0f, 1.0f, 0.0f);
        //glVertex3fv(points[i]);
        //point = point.normalize();
        //DRLog.writeToLog("sin: %f, cos: %f, i: %d", sin, cos, i);
        //DRLog.writeVector3ToLog(points[i], "Punkt");
    }   
    for(int j = 1; j <= segs; j++)
    {
        // PI/segs*2 = 360°
        // PI/segs = 180°
        DRMatrix rot = DRMatrix::rotationX((PI/segs*2)*j);
        for(int i = 0; i < iSegments; i++)
        {
            //glVertex3fv(points[i].transformNormal(rot));
            points[j*iSegments+i] = points[i].transformNormal(rot);//*DRRandom::rReal(1.02, 0.98f);
            color[j*iSegments+i] = DRColor((float)i/(float)iSegments, (float)j/(float)segs, 0.2f);
            //glVertex3fv(points[i+iSegments]);
        }
    }
    for(int j = 0; j < segs; j++)
    {
        for(int i = 0; i < iSegments; i++)
        {
            indices[i*2+(iSegments*2)*j] = iSegments+i+iSegments*j; 
            indices[i*2+1+(iSegments*2)*j] = i+iSegments*j;    
         }
    }
    
    for(int i = 0; i < 8; i++)
        DRLog.writeVector3ToLog(points[indices[i]]);
    
    int iterator =150;
    int m = 1;
    float max = 0.0f, min = 1.0f;
    for(int i = 0; i < iterator; i++)
    {
        DRVector3 n = DRRandom::rVector3(1.0f);
        DRPlane pl(n, n.length());
        //int m = rand() % 2;
        //if(!m) m =-1;
        m = -m;
        for(int j = 0; j < iSegments*(segs+2); j++)
        {
            float d = 1.0f - points[j].length();
            if(d != 1.0f && d > max) max = d;
            if(d < min) min = d;
            if(pl.dotCoords(points[j]) >= 1)
            {
                points[j] = points[j] * (1.0f+(float)m/1000.0f);
            }
            else
            {
                points[j] = points[j] * (1.0f-(float)m/1000.0f);
            }
        }
    }
    
    //einfärben
    DRLog.writeToLog("min: %f, max: %f", min, max);
    for(int j = 0; j < iSegments*(segs+2); j++)
    {
        float d = 1.0f - points[j].length();
        d *= -1.0f;
        if(j < 10)
            DRLog.writeToLog("d: %f", d);
        if(d < 0)
        {
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
    }


    //glEnd();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, points);
    glColorPointer(4, GL_FLOAT, 0, color);
    //glDrawArrays(GL_QUADS, 0, iSegments*(segs+2));    
    glDrawElements(GL_QUAD_STRIP, 2*iSegments+2*iSegments*(segs-1), GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
    
    glEndList();
    
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

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 0.5f, 0.1f);
    glBegin(GL_QUADS);
        glVertex3f( 50.0f, 0.0f, -50.0f);
        glVertex3f(-50.0f, 0.0f, -50.0f);
        glVertex3f(-50.0f, 0.0f,  50.0f);
        glVertex3f( 50.0f, 0.0f,  50.0f);
    glEnd();
    
    glTranslatef(0.0f, 2.0f, 0.0f);
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    glTranslatef(0.0f, -2.0f, 0.0f);
    
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
    glDisable(GL_LIGHTING);
    glPushMatrix();
    
    glTranslatef(0.0, 0.0f, -15.0f);
    //renderSphere(5.0f);
    
    static float sphereRotate = 0;
    glRotatef(sphereRotate, 0.4f, 0.4f, 0.4f);
    
    glCallList(sphereList);
    
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
