#include <string>
#include <fstream>

#include "GL_includes.h"

#include "ShaderProgram.h"

void ShaderProgram::read( const char *loc, std::string &buff )
{
    std::ifstream in( loc, std::ios::in );

    if ( in )
    {

        in.seekg( 0, std::ios::end );
        buff.resize(    in.tellg() );
        in.seekg( 0, std::ios::beg );

        in.read( &buff[0], buff.size() );

        in.close();

        return;

    } else throw( "Could not read Shader!" );
}

void ShaderProgram::compile_shader( GLuint id, const char *src ) throw( const char * )
{

    glShaderSource(  id, 1, &src, NULL );
    glCompileShader( id );

    GLint res;

    glGetShaderiv( id, GL_COMPILE_STATUS, &res );

    if( res != GL_TRUE )
    {
        int logLength;

        glGetShaderiv(      id, GL_INFO_LOG_LENGTH, &logLength );

        char *msg = new char[logLength];

        glGetShaderInfoLog( id, logLength, NULL,           msg );

        throw( msg );

    }

}

ShaderProgram::ShaderProgram( const char * vs_file, const char * fs_file ) throw( char const * )
{
    GLuint vsID = glCreateShader(   GL_VERTEX_SHADER ),
           fsID = glCreateShader( GL_FRAGMENT_SHADER );

    std::string vsSrc, fsSrc;

    ShaderProgram::read( vs_file, vsSrc );
    ShaderProgram::read( fs_file, fsSrc );

    const char * vsSrcRaw = vsSrc.c_str();
    const char * fsSrcRaw = fsSrc.c_str();

    ShaderProgram::compile_shader( vsID, vsSrcRaw );
    ShaderProgram::compile_shader( fsID, fsSrcRaw );

    _id = glCreateProgram();

    glAttachShader( _id, vsID );
    glAttachShader( _id, fsID );
    glLinkProgram(        _id );

    glDeleteShader(      vsID );
    glDeleteShader(      fsID );

    GLint res;
    glGetProgramiv( _id, GL_LINK_STATUS, &res );

    if( res != GL_TRUE )
    {
        int logLength;

        glGetProgramiv(      _id, GL_INFO_LOG_LENGTH, &logLength );

        char *msg = new char[logLength];
        
        glGetProgramInfoLog( _id, logLength, NULL,           msg );
        
        throw( msg );

    }

    _matID = glGetUniformLocation( _id, "MVP" );

    if( _matID == -1 ) throw( "No MVP Uniform in shader!" );

}

ShaderProgram &ShaderProgram::operator=( ShaderProgram &p )
{

    _id    =    p.id();
    _matID = p.matID();

    return       *this;
}

ShaderProgram &ShaderProgram::operator=( ShaderProgram &&p )
{
    _id    =    p.id();
    _matID = p.matID();

    return       *this;
}

ShaderProgram::~ShaderProgram()           { if( _id ) glDeleteProgram( _id ); }

void ShaderProgram::use()           const {           glUseProgram(    _id ); }

const GLuint ShaderProgram::id()    const {                       return _id; }

const GLuint ShaderProgram::matID() const {                    return _matID; }