#include <initializer_list>
#include <utility>
#include <memory>

#include "glm/glm.hpp"

#include "RenderGroup.h"

RenderGroup::RenderGroup( std::initializer_list< std::shared_ptr<Renderable> > &&_children )
: Renderable()
{

    children.reserve( _children.size() );

    for( auto c : _children ) this->add_child( c );

}

void RenderGroup::add_child( std::shared_ptr<Renderable> &pChild )
{
    children.emplace_back( pChild );
}

void RenderGroup::tick( const double &delta )
{

    for( auto c : children ) c->tick( delta );

}

void RenderGroup::render( ShaderProgram &p, glm::mat4 &m )
{
    glm::mat4 transform = m * _local;
    for( auto c : children ) c->render( p, transform );

}