#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "glm/glm.hpp"

#include "ShaderProgram.h"

class Renderable 
{

protected: 
    glm::mat4 _local;

public:

    Renderable() : _local( 1.0 ) {};

    virtual void render(ShaderProgram &, glm::mat4 &) = 0;
};

#endif