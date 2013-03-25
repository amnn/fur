#include <iostream>
#include <memory>
#include <thread>

#include "GL_includes.h"

#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "BufferPoly.h"
#include "GLFWScr.h"

using namespace std;

struct vert_attrib {

    GLfloat vert[3];
    GLfloat color[3];

};

int main( int, char ** )
{

    try {

        RenderEngine<GLFWScr>             engine( 1024, 768 );

        engine.use_program( ShaderProgram( "assets/shaders/vert.glsl", 
                                           "assets/shaders/frag.glsl" ) );

        vert_attrib *vs = new vert_attrib[3] {

            { { -1.f, -1.f, 0.f }, { 1.f, 0.f, 0.f} },
            { {  1.f, -1.f, 0.f }, { 0.f, 1.f, 0.f} },
            { {  0.f,  1.f, 0.f }, { 0.f, 0.f, 1.f} }

        };

        GLuint *el = new GLuint[3] { 0, 1, 2 };

        BufferPoly *triangle = new BufferPoly( vs, 3, el, 3 );

        delete[] vs; delete[] el;

        triangle->register_attrib( BufferPoly::ATTRIB_VERT, 3, 
                                           GL_FLOAT, GL_FALSE, 
                                        sizeof( vert_attrib ), 
                                                            0 );

        triangle->register_attrib( BufferPoly::ATTRIB_COLOR, 3,
                                            GL_FLOAT, GL_FALSE, 
                                         sizeof( vert_attrib ), 
                                         3 * sizeof( GLfloat ) );

        shared_ptr<Renderable> pTri( triangle );

        engine.thrd_req(); engine.add_child( pTri ); engine.thrd_rel();

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}