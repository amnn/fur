#include "GL_includes.h"

class Buffer {

    GLuint _id;

public:

    Buffer()                           {     glGenBuffers( 1, &_id ); }
    ~Buffer()                          {  glDeleteBuffers( 1, &_id ); }

    template<class S>
    Buffer( 
    
        GLenum     target, 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 
    
    ) : Buffer()             { data( target, size, contents, usage ); }

    Buffer( Buffer &&that )            {  std::swap( _id, that._id ); }
    Buffer( Buffer & )                                      = delete;

    Buffer &operator=( Buffer &&that ) {  std::swap( _id, that._id ); 
                                                        return *this; }
    Buffer &operator=( Buffer & )                           = delete;

    void bind( GLenum target )         { glBindBuffer( target, _id ); }

    template<class S>
    void data( 

        GLenum     target, 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 

    )
    {
        bind( target );
        glBufferData( target, size * sizeof( S ), contents, usage );
    }

};