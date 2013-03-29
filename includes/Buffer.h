#ifndef BUFFER_H
#define BUFFER_H

#include "GL_includes.h"

class Buffer {

    GLuint _id, _target;

public:

    Buffer()                           {     glGenBuffers( 1, &_id ); }
    ~Buffer()                          {  glDeleteBuffers( 1, &_id ); }

    template<class S>
    Buffer( 
    
        GLenum     target, 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 
    
    ) : Buffer()     { _target = target; data( size, contents, usage ); }

    Buffer( Buffer &&that )            {    std::swap( _id, that._id ); }
    Buffer( Buffer & )                                        = delete;

    Buffer &operator=( Buffer &&that ) {    std::swap( _id, that._id ); 
                                                          return *this; }
    Buffer &operator=( Buffer & )                             = delete;

    void           bind()   const      {  glBindBuffer( _target, _id ); }
    const GLenum & target() const      { return                _target; }

    template<class S>
    void data( 
 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 

    ) const
    {
        bind();
        glBufferData( _target, size * sizeof( S ), contents, usage );
    }

};

#endif