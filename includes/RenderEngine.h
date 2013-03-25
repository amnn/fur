#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <iostream>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <vector>

#include "GL_includes.h"
#include "glm/glm.hpp"

#include "RenderGroup.h"
#include "ShaderProgram.h"

template< class Scr >
class RenderEngine : public RenderGroup
{

    std::mutex                 _access;
    std::vector< std::thread >    _aux;
    Scr                         screen;
    ShaderProgram                _prog;

public:

    RenderEngine( float _w, float _h, float fov, 
                 float ncp = 0.1f, float fcp = 100.f ) 
    throw( char const * ) 
    : screen( (int)_w, (int)_h )
    {

        _local = glm::perspective( fov, _w / _h, ncp, fcp );

        glewExperimental = true;
        if( glewInit() != GLEW_OK ) { throw( "Failed to Initialize GLEW!" ); }

    }

    ~RenderEngine()
    {
        for( std::thread &th : _aux ) th.join();

    }

    std::vector< std::thread > & aux() { return   _aux; }
    Scr                        & scr() { return screen; }

    void thrd_req()  {                _access.lock(  ); }
    void thrd_rel()  {                _access.unlock(); }

    void draw_loop() {     screen.display_link( this ); }

    void use_program( ShaderProgram &&p )
    {
        _prog = p;
        _prog.use();
    }

    void render()    { 

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
        for( auto c : children ) c->render( _prog, _local );

        screen.swap(); 
    }

};

#endif