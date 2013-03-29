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

/* TODO

* Tween class
* New BufferPoly constructor initialises VAO with already created Buffer.
* New BufferPoly constructor initialise from OBJ file.

*/

struct vert_attrib {

    static void layout( BufferPoly<vert_attrib> &b )
    {

        b.register_attrib( BufferPoly<vert_attrib>::ATTRIB_VERT, 3, 
                                                GL_FLOAT, GL_FALSE, 
                                             sizeof( vert_attrib ), 
                                                                 0 );

        b.register_attrib( BufferPoly<vert_attrib>::ATTRIB_COLOR, 3,
                                                 GL_FLOAT, GL_FALSE, 
                                              sizeof( vert_attrib ), 
                                              3 * sizeof( GLfloat ) );

    }

    GLfloat  vert[3];
    GLfloat color[3];

};

int main( int, char ** )
{

    try {

        RenderEngine<GLFWScr>     engine                ( 1024.f, 768.f, 45.f, 0.1f, 100.f );
        shared_ptr<ShaderProgram> prog   ( new ShaderProgram( "assets/shaders/vert.glsl", 
                                                              "assets/shaders/frag.glsl" ) );

        engine.use_program( prog );

        engine.look_at(
            4.f, 4.f, 3.f,
            0.f, 0.f, 0.f,
                      1.f
        );

        vert_attrib *tr0 = new vert_attrib[3] {

            { { -1.f, -1.f, 0.f }, { 1.f, 0.f, 0.f } },
            { {  1.f, -1.f, 0.f }, { 1.f, 0.f, 0.f } },
            { {  0.f,  1.f, 0.f }, { 1.f, 0.f, 0.f } }

        };

        vert_attrib *tr1 = new vert_attrib[3] {

            { { -1.f, -1.f, 0.f }, { 0.f, 1.f, 0.f } },
            { {  1.f, -1.f, 0.f }, { 0.f, 1.f, 0.f } },
            { {  0.f,  1.f, 0.f }, { 0.f, 1.f, 0.f } }

        };

        vert_attrib *tr2 = new vert_attrib[3] {

            { { -1.f, -1.f, 0.f }, { 0.f, 0.f, 1.f } },
            { {  1.f, -1.f, 0.f }, { 0.f, 0.f, 1.f } },
            { {  0.f,  1.f, 0.f }, { 0.f, 0.f, 1.f } }

        };


        GLuint *tre = new GLuint[3] { 0, 1, 2 };

        BufferPoly<vert_attrib> *t0 = new BufferPoly<vert_attrib>( tr0, 3, tre, 3 );
        BufferPoly<vert_attrib> *t1 = new BufferPoly<vert_attrib>( tr1, 3, tre, 3 );
        BufferPoly<vert_attrib> *t2 = new BufferPoly<vert_attrib>( tr2, 3, tre, 3 );
        BufferPoly<vert_attrib> *t3 = new BufferPoly<vert_attrib>(            *t0 );

        delete[] tr0; delete[] tr1; delete[] tr2; delete[] tre;

        shared_ptr<Renderable> spt0( t0 ), spt1( t1 ), spt2( t2 ), spt3( t3 );

        engine.thrd_req(); 

        engine.add_child( spt2 );
        engine.add_child( spt0 );
        engine.add_child( spt1 );
        engine.add_child( spt3 );

        engine.thrd_rel();

        t0->transform() = glm::scale( glm::mat4(1.f), glm::vec3( 1.f) );
        t1->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.8f) );
        t2->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );

        t0->transform() = glm::translate( t0->transform(), glm::vec3( 0.f, 0.f, 0.8f ) );
        t1->transform() = glm::translate( t1->transform(), glm::vec3( 0.f, 0.f, 0.5f ) );
        t2->transform() = glm::translate( t2->transform(), glm::vec3( 0.f, 0.f, 2.f ) );
        t3->transform() = glm::translate( t3->transform(), glm::vec3( 0.f, 0.f, 0.2f ) );

        auto rot = [](Renderable &r, const double &d )
        {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(10*d), glm::vec3(0.f, 0.f, 1.f ) );

        };


        auto revRot = [](Renderable &r, const double &d )
        {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(-10*d), glm::vec3(0.f, 0.f, 1.f ) );

        };

        t0->callback() =    rot;
        t1->callback() =    rot;
        t2->callback() = revRot;
        t3->callback() = revRot;

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}