#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <vector>
#include <initializer_list>
#include <memory>

#include "glm/glm.hpp"

#include "Renderable.h"
#include "ShaderProgram.h"

class RenderGroup : public Renderable 
{

protected:

    std::vector< std::shared_ptr<Renderable> > children;

public:

    RenderGroup() : Renderable() {};

    RenderGroup( std::initializer_list< std::shared_ptr<Renderable> > && );

    void add_child( std::shared_ptr<Renderable> & );

    void render( ShaderProgram &, glm::mat4 & );

};

#endif