#ifndef GLFWSCR_H
#define GLFWSCR_H

#include "RenderEngine.h"

class GLFWScr {
    
public:

    GLFWScr( int , int ) throw( char const * );
    ~GLFWScr();

    void display_link( RenderEngine<GLFWScr> * );

    void swap();

};

#endif