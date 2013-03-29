#ifndef BUFFERPOLY_H
#define BUFFERPOLY_H

#include <memory>

#include "GL_includes.h"
#include "glm/glm.hpp"

#include "Renderable.h"
#include "Buffer.h"

template <class S>
class BufferPoly : public Renderable 
{

    GLuint                      vaoID;
    GLsizei                   indices;
    std::shared_ptr< Buffer >  _aBuff,
                               _eBuff;

public:

    enum IncludedAttribs {
        ATTRIB_VERT  = 0,
        ATTRIB_COLOR = 1,
        ATTRIB_NORM  = 2
    };

    BufferPoly( S *attribs, int nAttr, GLuint *elems, int nElem ) 
    : Renderable()
    {

        indices = nElem;

        glGenVertexArrays( 1, &vaoID );
        glBindVertexArray(     vaoID );

        _aBuff = std::shared_ptr<Buffer>(new Buffer( GL_ARRAY_BUFFER,         nAttr, attribs, GL_STATIC_DRAW ) );
        _eBuff = std::shared_ptr<Buffer>(new Buffer( GL_ELEMENT_ARRAY_BUFFER, nElem,   elems, GL_STATIC_DRAW ) );

        S::layout( *this );

    }

    BufferPoly( std::shared_ptr<Buffer> &b, GLuint *elems, int nElem )
    {

        if( b->target() != GL_ARRAY_BUFFER ) throw( "Shared Buffer must have GL_ARRAY_BUFFER target!" );

        indices = nElem;

        glGenVertexArrays( 1, &vaoID );
        glBindVertexArray(     vaoID );

        _aBuff = b; _aBuff->bind();
        _eBuff = std::shared_ptr<Buffer>(new Buffer( GL_ELEMENT_ARRAY_BUFFER, nElem, elems, GL_STATIC_DRAW ) );

        S::layout( *this );
    }

    ~BufferPoly() { glDeleteVertexArrays( 1, &vaoID ); }

    BufferPoly( const BufferPoly<S> &that ) 
    : Renderable( that )
    {

        glGenVertexArrays( 1, &vaoID );
        glBindVertexArray(     vaoID );

        _aBuff  =  that._aBuff;
        _eBuff  =  that._eBuff;
        indices = that.indices;

        _aBuff->bind();
        _eBuff->bind();

        S::layout( *this );

    }

    BufferPoly<S> &operator= ( const BufferPoly<S> &that )
    {

        glGenVertexArrays( 1, &vaoID );
        glBindVertexArray(     vaoID );

        _aBuff  =  that._aBuff;
        _eBuff  =  that._eBuff;
        indices = that.indices; 

        _aBuff->bind();
        _eBuff->bind();

        S::layout( *this );

    }

    BufferPoly( BufferPoly &&that )
    {

        vaoID      =   that.vaoID;
        indices    = that.indices;
        _aBuff     =  that._aBuff;
        _eBuff     =  that._eBuff;

        that.vaoID =            0;

    }

    BufferPoly<S> &operator= ( BufferPoly &&that )
    {

        std::swap( vaoID,   that.vaoID );
        std::swap( _aBuff, that._aBuff );
        std::swap( _eBuff, that._eBuff );

        indices = that.indices;

        return *this;
    }

    void register_attrib( 

        BufferPoly::IncludedAttribs   attr, 
        GLuint                        size, 
        GLenum                        type, 
        GLboolean                     norm, 
        GLsizei                     stride, 
        GLuint                         off 

    ) const
    {

        glBindVertexArray( vaoID );
        _aBuff->bind(            );

        glEnableVertexAttribArray( (GLuint)attr );
        glVertexAttribPointer( (GLuint)attr, size, type, norm, stride, reinterpret_cast<void *>( off ) );

    }

    void render( const ShaderProgram &p, const glm::mat4 &m ) const
    {

        glBindVertexArray( vaoID );
        glm::mat4 transform = m * _local;

        glUniformMatrix4fv(            p.matID(), 1, GL_FALSE, &transform[0][0] );
        glDrawElements( GL_TRIANGLE_STRIP, indices, GL_UNSIGNED_INT, (void *) 0 );

    }
};

#endif