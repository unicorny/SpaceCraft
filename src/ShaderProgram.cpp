#include "main.h"

ShaderProgram::ShaderProgram()
: mVertexShader(0), mFragmentShader(0), mProgram(0)
{
    glClampColorARB(GL_CLAMP_READ_COLOR, GL_FALSE);
}


ShaderProgram::~ShaderProgram()
{
    if(mVertexShader && mProgram)
        glDetachShader(mProgram, mVertexShader);
    if(mFragmentShader && mProgram)
        glDetachShader(mProgram, mFragmentShader);
    if(mProgram)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
    if(mVertexShader)
    {
        glDeleteShader(mVertexShader);
        mVertexShader = 0;
    }
    if(mFragmentShader)
    {
        glDeleteShader(mFragmentShader);
        mFragmentShader = 0;
    }
}

DRReturn ShaderProgram::init(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    const char *vertexShaderStrings[1];
    const char *fragmentShaderStrings[1];
     
    GLint vertexCompiled;
    GLint fragmentCompiled;
    GLint shadersLinked;
    char str[4096]; // For error messages from the GLSL compiler and linker
    memset(str, 0, 4096);

    // Create the vertex shader.
    mVertexShader = glCreateShader(GL_VERTEX_SHADER);

    unsigned char* vertexShaderAssembly = readShaderFile( vertexShaderFile );
    if(!vertexShaderAssembly)
    {
        DRLog.writeToLog("<font color='red'>Fehler:</font>couldn't open Vertex shader file: %s", vertexShaderFile);
        LOG_ERROR("Fehler by opening vertex-shader", DR_ERROR);
    }

    vertexShaderStrings[0] = (char*)vertexShaderAssembly;
    glShaderSource( mVertexShader, 1, vertexShaderStrings, NULL );
    glCompileShader( mVertexShader);
    free((void *)vertexShaderAssembly);

    glGetShaderiv( mVertexShader, GL_COMPILE_STATUS, &vertexCompiled );
    if(vertexCompiled  == GL_FALSE)
    {
        glGetShaderInfoLog(mVertexShader, sizeof(str), NULL, str);
        DRLog.writeToLog("<font color='red'>Fehler:</font>Vertex shader (%s) compile error: %s", vertexShaderFile, str);
    }
    if(DRGrafikError("ShaderProgram::init create Vertex Shader")) LOG_WARNING("Fehler bei shader init");
// -------------------------------------------------------------------------------------------------------------------------
    // Create the fragment shader.
    mFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    unsigned char* fragmentShaderAssembly = readShaderFile( fragmentShaderFile );
    if(!fragmentShaderAssembly)
    {
        DRLog.writeToLog("<font color='red'>Fehler:</font>couldn't open Fragment shader file: %s", vertexShaderFile);
        LOG_ERROR("Fehler by opening fragment-shader", DR_ERROR);
    }
    
    fragmentShaderStrings[0] = (char*)fragmentShaderAssembly;
    glShaderSource( mFragmentShader, 1, fragmentShaderStrings, NULL );
    glCompileShader( mFragmentShader );
    
    free((void *)fragmentShaderAssembly);

    glGetShaderiv( mFragmentShader, GL_COMPILE_STATUS, &fragmentCompiled );
    if(fragmentCompiled == GL_FALSE)
    {
        glGetShaderInfoLog( mFragmentShader, sizeof(str), NULL, str );
        DRLog.writeToLog("<font color='red'>Fehler:</font>Fragment shader (%s) compile error: %s", fragmentShaderFile, str);
    }
    if(DRGrafikError("ShaderProgram::init create fragment shader")) LOG_WARNING("Fehler bei shader init");    
//----------------------------------------------------------------------------------------------------
    // Create a program object and attach the two compiled shaders.
    mProgram = glCreateProgram();
    glAttachShader( mProgram, mVertexShader );
    glAttachShader( mProgram, mFragmentShader );

    // Link the program object and print out the info log.
    glLinkProgram( mProgram );
    glGetProgramiv( mProgram, GL_LINK_STATUS, &shadersLinked );
    if( shadersLinked == GL_FALSE )
    {
        int length = 0;
        glGetProgramInfoLog( mProgram, sizeof(str), &length, str );
        //printError("Program object linking error", str);
        if(length > 1023)
            DRLog.writeToLog(DRString(str));
        else
            DRLog.writeToLog("<font color='red'>Fehler:</font>Program object linking error:\n%s", str);
    }
    
    if(DRGrafikError("ShaderProgram::init create programm")) LOG_WARNING("Fehler bei shader init");
    
    return DR_OK;
}

void ShaderProgram::bind() const
{
    glUseProgram(mProgram);
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

unsigned char* ShaderProgram::readShaderFile(const char *filename) 
{
    DRFile file(filename, "rt");
    if(!file.isOpen())
        LOG_ERROR("cannot open file", NULL);
    
    int bytesinfile = file.getSize();
    unsigned char *buffer = (unsigned char*)malloc(bytesinfile+1);
    long unsigned int bytesread = 0;
    file.read(buffer, 1, bytesinfile, &bytesread);
    buffer[bytesread] = 0; // Terminate the string with 0
    
    return buffer;
}


