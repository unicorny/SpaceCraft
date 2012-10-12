#include "ShaderProgram.h"


Shader::Shader(DHASH id/* = 0*/)
: mID(id), mShaderID(0)
{
    
}

Shader::~Shader()
{
    glDeleteShader(mShaderID);
    mShaderID = 0;
}

DRReturn Shader::init(const char* shaderFile, GLenum shaderType)
{
    const char *shaderStrings[1];
     
    GLint compiled;
    char str[4096]; // For error messages from the GLSL compiler and linker
    memset(str, 0, 4096);

    // Create the shader.
    mShaderID = glCreateShader(shaderType);

    unsigned char* shaderAssembly = readShaderFile( shaderFile );
    if(!shaderAssembly)
    {
        DREngineLog.writeToLog("<font color='red'>Fehler:</font>couldn't open shader file: %s", shaderFile);
        LOG_ERROR("Fehler by opening shader", DR_ERROR);
    }

    shaderStrings[0] = (char*)shaderAssembly;
    glShaderSource( mShaderID, 1, shaderStrings, NULL );
    glCompileShader( mShaderID);
    free((void *)shaderAssembly);

    glGetShaderiv( mShaderID, GL_COMPILE_STATUS, &compiled );
    if(compiled  == GL_FALSE)
    {
        int length = 0;
        glGetShaderInfoLog(mShaderID, sizeof(str), &length, str);
        if(length > 1023)
            DREngineLog.writeToLog(DRString(str));
        else
            DREngineLog.writeToLog("<font color='red'>Fehler:</font>shader (%s) compile error: %s", shaderFile, str);
    }
    if(DRGrafikError("Shader::init create Shader")) LOG_WARNING("Fehler bei shader init");
    
    return DR_OK;
}

unsigned char* Shader::readShaderFile(const char *filename) 
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

// ********************************************************************

ShaderProgram::ShaderProgram(DHASH id/* = 0*/)
: mId(id), mVertexShader(), mFragmentShader(), mProgram(0)
{
    glClampColorARB(GL_CLAMP_READ_COLOR, GL_FALSE);
}


ShaderProgram::~ShaderProgram()
{
    if(mProgram)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

DRReturn ShaderProgram::init(ShaderPtr vertexShader, ShaderPtr fragmentShader)
{
    GLint shadersLinked;
    char str[8192]; // For error messages from the GLSL compiler and linker
    memset(str, 0, 8192);
    
    if(!vertexShader || !fragmentShader) LOG_ERROR("Zero-Pointer", DR_ZERO_POINTER);
    
    mVertexShader = vertexShader;
    mFragmentShader = fragmentShader;
    
       // Create a program object and attach the two compiled shaders.
    mProgram = glCreateProgram();
    glAttachShader( mProgram, *mVertexShader );
    glAttachShader( mProgram, *mFragmentShader );

    // Link the program object and print out the info log.
    glLinkProgram( mProgram );
    glGetProgramiv( mProgram, GL_LINK_STATUS, &shadersLinked );
    if( shadersLinked == GL_FALSE )
    {
        int length = 0;
        glGetProgramInfoLog( mProgram, sizeof(str), &length, str );
        //printError("Program object linking error", str);
        if(length > 1023)
            DREngineLog.writeToLog(DRString(str));
        else
            DREngineLog.writeToLog("<font color='red'>Fehler:</font>Program object linking error:\n%s", str);
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



void ShaderProgram::setUniform1f(const char* name, GLfloat data)
{
	int loc = glGetUniformLocation(mProgram, name);
	glUniform1f(loc, data);
	DRGrafikError("ShaderProgram::setUniform1f");
}
void ShaderProgram::setUniform1i(const char* name, GLint data)
{
    int loc = glGetUniformLocation(mProgram, name);
	glUniform1i(loc, data);
	DRGrafikError("ShaderProgram::setUniform1i");    
}
void ShaderProgram::setUniform3fv(const char* name, const DRVector3& data)
{
	int loc = glGetUniformLocation(mProgram, name);
	glUniform3fv(loc, 1, static_cast<const GLfloat*>(data));
	DRGrafikError("ShaderProgram::setUniform3fv");
}

void ShaderProgram::setUniform2fv(const char* name, const DRVector2& data)
{
    int loc = glGetUniformLocation(mProgram, name);
	glUniform2fv(loc, 1, static_cast<const GLfloat*>(data));
	DRGrafikError("ShaderProgram::setUniform2fv");
}

void ShaderProgram::setUniform2iv(const char* name, const DRVector2i& data)
{
    int loc = glGetUniformLocation(mProgram, name);
	glUniform2iv(loc, 1, static_cast<const GLint*>(data));
	DRGrafikError("ShaderProgram::setUniform2iv");
}

void ShaderProgram::setUniformMatrix(const char* name, const DRMatrix& matrix, bool transpose /* = false*/)
{
	int loc = glGetUniformLocation(mProgram, name);
	glUniformMatrix4fv(loc, 1, transpose, static_cast<const GLfloat*>(matrix));
	if(DRGrafikError("ShaderProgramm::setUniformMatrix"))
    {
        DREngineLog.writeToLog("[ShaderProgram::setUniformMatrix] name: %s, transpose: %d", name, transpose);
        DREngineLog.writeMatrixToLog(matrix, "[ShaderProgram::setUniformMatrix] matrix");
    }
}

DRMatrix ShaderProgram::getUniformMatrix(const char* name)
{
	DRMatrix ret;
	int loc = glGetUniformLocation(mProgram, name);
	glGetUniformfv(mProgram, loc, ret);
	DRGrafikError("ShaderProgramm::getUniformMatrix");
	return ret;
}
