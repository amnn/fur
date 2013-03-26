#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "glm/glm.hpp"

#include "ShaderProgram.h"

class Renderable 
{

public:

    typedef void (*tick_callback)(Renderable &,const double &);

    Renderable() : _local( 1.0 ) {};

    glm::mat4     & transform()      { return       _local; }

    tick_callback & callback()       { return          _cb; }

    void tick( const double &delta ) { _cb( *this, delta ); }

    virtual void render( ShaderProgram &, glm::mat4 & ) = 0;


protected:

    glm::mat4     _local;
    tick_callback    _cb;

};

#endif