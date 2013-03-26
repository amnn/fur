#include <iostream>
#include <memory>
#include <thread>

#include "GL_includes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderable.h"
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

        vert_attrib *tr = new vert_attrib[3] {

            { { -1.f, -1.f, 0.f }, { 1.f, 0.f, 0.f} },
            { {  1.f, -1.f, 0.f }, { 1.f, 0.f, 0.f} },
            { {  0.f,  1.f, 0.f }, { 1.f, 0.f, 0.f} }

        };

        vert_attrib *sq = new vert_attrib[4] {
            { { -1.f, -1.f, 0.f }, { 0.f, 0.f, 1.f } },
            { {  1.f, -1.f, 0.f }, { 0.f, 0.f, 1.f } },
            { { -1.f,  1.f, 0.f }, { 0.f, 0.f, 1.f } },
            { {  1.f,  1.f, 0.f }, { 0.f, 0.f, 1.f } }
        };

        GLuint *tre = new GLuint[3]    { 0, 1, 2 },
               *sqe = new GLuint[4] { 0, 1, 2, 3 };

        BufferPoly *triangle = new BufferPoly( tr, 3, tre, 3 );
        BufferPoly *square   = new BufferPoly( sq, 4, sqe, 4 );

        delete[] tr; delete[] sq; delete[] tre; delete[] sqe;

        triangle->register_attrib( BufferPoly::ATTRIB_VERT, 3, 
                                           GL_FLOAT, GL_FALSE, 
                                        sizeof( vert_attrib ), 
                                                            0 );

        triangle->register_attrib( BufferPoly::ATTRIB_COLOR, 3,
                                            GL_FLOAT, GL_FALSE, 
                                         sizeof( vert_attrib ), 
                                         3 * sizeof( GLfloat ) );

        square->register_attrib(    BufferPoly::ATTRIB_VERT, 3, 
                                            GL_FLOAT, GL_FALSE, 
                                         sizeof( vert_attrib ), 
                                                             0 );

        square->register_attrib(   BufferPoly::ATTRIB_COLOR, 3,
                                            GL_FLOAT, GL_FALSE, 
                                         sizeof( vert_attrib ), 
                                         3 * sizeof( GLfloat ) );

        shared_ptr<Renderable> pTri( triangle ), pSqr( square );

        engine.thrd_req(); 

        engine.add_child( pTri );
        engine.add_child( pSqr );

        engine.thrd_rel();

        triangle->transform() = glm::scale( glm::mat4(1.f), glm::vec3( 0.5f ) );
        square->transform()   = glm::scale( glm::mat4(1.f), glm::vec3( 0.1f ) );
        square->transform()   = glm::translate( square->transform(), glm::vec3( 0.f, 0.f, 1.f ) );

        triangle->callback()  = [](Renderable &r, const double &d ) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>( 10*d ), glm::vec3(0.f, 0.f, 1.f) );

        };

        square->callback()    = [](Renderable &r, const double &d ) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>( -10*d), glm::vec3(0.f, 0.f, 1.f) );

        };

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}