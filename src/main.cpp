#include <iostream>
#include <memory>
#include <thread>

#include "GL_includes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

        RenderEngine<GLFWScr>             engine( 1024.f, 768.f, 45.f );

        engine.use_program( ShaderProgram( "assets/shaders/vert.glsl", 
                                           "assets/shaders/frag.glsl" ) );

        engine.look_at(
            0.f, 0.f, 3.f,
            0.f, 0.f, 0.f,
                      1.f
        );

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

        triangle->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );

        /*engine.aux().emplace_back( thread([&engine,&triangle]() {

            glm::mat4 & mat   = triangle->transform();
            bool      & term  =         engine.term();

            while( !term )
            {

                mat = glm::rotate( mat, 0.0001f, glm::vec3(1.f,1.f,1.f) );
            }

        } ) );*/

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}