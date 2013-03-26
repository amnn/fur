#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <iostream>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <vector>

#include "GL_includes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderGroup.h"
#include "ShaderProgram.h"

template< class Scr >
class RenderEngine : public RenderGroup
{

    bool                         _term;
    std::mutex                 _access;
    std::vector< std::thread >    _aux;
    Scr                        _screen;
    ShaderProgram                _prog;
    glm::mat4                    _proj,
                                 _view;

public:

    RenderEngine
    ( 
      float  _w, 
      float  _h, 
      float fov, 
      float ncp =  0.1f, 
      float fcp = 100.f 
    ) 
    throw( char const * ) 
    : _term { false },
      _screen( (int)_w, (int)_h ),
      _proj { glm::perspective( fov, _w/_h, ncp, fcp ) },
      _view ( 1.f )
    {

        _local = _proj * _view;

        glewExperimental = true;
        if( glewInit() != GLEW_OK ) { throw( "Failed to Initialize GLEW!" ); }

    }

    ~RenderEngine()
    {
        _term = true;

        for( std::thread &th : _aux ) th.join();
    }

    std::vector< std::thread > & aux()  { return    _aux; }
    Scr                        & scr()  { return _screen; }
    bool                       & term() { return   _term; }

    void thrd_req()    {                _access.lock(  ); }
    void thrd_rel()    {                _access.unlock(); }

    void draw_loop()   {    _screen.display_link( this ); }

    void use_program( ShaderProgram &&p )
    {
        _prog = p;
        _prog.use();
    }

    void look_at
    (
      float cX, float cY, float cZ,
      float pX, float pY, float pZ,
      float                    dir
    )
    {

        _view = glm::lookAt( glm::vec3( cX,   cY,  cZ ),
                             glm::vec3( pX,   pY,  pZ ),
                             glm::vec3( 0.f, dir, 0.f ) );

        _local = _proj * _view;

    }

    void render()    { 

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
        for( auto c : children ) c->render( _prog, _local );

        _screen.swap(); 
    }

};

#endif