#ifndef BUFFERPOLY_H
#define BUFFERPOLY_H

#include <iostream>

#include "GL_includes.h"
#include "glm/glm.hpp"

#include "Renderable.h"
#include "ShaderProgram.h"

class BufferPoly : public Renderable 
{

    GLuint  vaoID, buffs[2];
    GLsizei         indices;

public:

    enum IncludedAttribs {
        ATTRIB_VERT  = 0,
        ATTRIB_COLOR = 1,
        ATTRIB_NORM  = 2
    };

    template <class S>
    BufferPoly( S *attribs, int nAttr, GLuint *elems, int nElem ) : Renderable()
    {

        indices = nElem;

        glGenVertexArrays( 1, &vaoID );
        glBindVertexArray(     vaoID );

        glGenBuffers( 2,                          buffs );
        glBindBuffer( GL_ARRAY_BUFFER,         buffs[0] );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffs[1] );

        glBufferData( GL_ARRAY_BUFFER,         nAttr * sizeof(      S ), attribs, GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, nElem * sizeof( GLuint ),   elems, GL_STATIC_DRAW );
    }

    void register_attrib( 
        BufferPoly::IncludedAttribs   attr, 
        GLuint                        size, 
        GLenum                        type, 
        GLboolean                     norm, 
        GLsizei                     stride, 
        GLuint                         off )
    {

        glBindVertexArray(                     vaoID );
        glBindBuffer(      GL_ARRAY_BUFFER, buffs[0] );

        glEnableVertexAttribArray( (GLuint)attr );
        glVertexAttribPointer( (GLuint)attr, size, type, norm, stride, reinterpret_cast<void *>( off ) );

    }

    void render( ShaderProgram &p, glm::mat4 &m )
    {

        glBindVertexArray( vaoID );

        glm::mat4 transform = m * _local;

        glUniformMatrix4fv(            p.matID(), 1, GL_FALSE, &transform[0][0] );
        glDrawElements( GL_TRIANGLE_STRIP, indices, GL_UNSIGNED_INT, (void *) 0 );

    }
};

#endif