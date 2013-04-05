#include <iostream>
#include <memory>
#include <thread>

#include "GL_includes.h"

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderable.h"
#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "BufferPoly.h"
#include "Buffer.h"
#include "GLFWScr.h"

#define FOV_MAX   90.f
#define FOV       45.f
#define FOV_MIN   10.f
#define NCP       0.1f
#define FCP      100.f
#define SCR_W   1024.f
#define SCR_H    768.f
#define SCR_C_X 1024/2
#define SCR_C_Y  768/2
#define TRN_SPD  0.01f
#define ROT_SPD    1.f

using namespace std;

/* TODO

* Tween class
* New BufferPoly constructor initialise from file.

*/

template< BufferAttrib Attr >
struct vec3 {

    static void layout( Buffer<vec3> &b )
    {

        b.register_attrib(            Attr, 3, 
                           GL_FLOAT, GL_FALSE, 
                               sizeof( vec3 ), 
                                            0 );

    }

    GLfloat  vert[3];

};

void engine_callback( Renderable &, const double & );

int main( int, char ** )
{

    try {

        RenderEngine<GLFWScr>     engine                     ( SCR_W, SCR_H, FOV, NCP, FCP );
        shared_ptr<ShaderProgram> prog   ( new ShaderProgram( "assets/shaders/vert.glsl", 
                                                              "assets/shaders/frag.glsl" ) );

        engine.use_program( prog );

        auto cube = new vec3< ATTRIB_VERT >[8] {

            { {  1.f,  1.f,  1.f } },
            { {  1.f, -1.f,  1.f } },
            { { -1.f,  1.f,  1.f } },
            { { -1.f, -1.f,  1.f } },
            { { -1.f, -1.f, -1.f } },
            { {  1.f, -1.f, -1.f } },
            { {  1.f,  1.f, -1.f } },
            { { -1.f,  1.f, -1.f } },

        };

        auto col1 = new vec3< ATTRIB_COLOR >[8] {

            { { 1.f, 1.f, 1.f } },
            { { 1.f, 1.f, 0.f } },
            { { 1.f, 0.f, 1.f } },
            { { 1.f, 0.f, 0.f } },
            { { 0.f, 1.f, 1.f } },
            { { 0.f, 1.f, 0.f } },
            { { 0.f, 0.f, 1.f } },
            { { 0.f, 0.f, 0.f } }

        };

        auto col2 = new vec3< ATTRIB_COLOR >[8] {

            { { 1.f, 0.f, 0.f } },
            { { 0.f, 0.f, 1.f } },
            { { 0.f, 0.f, 1.f } },
            { { 1.f, 0.f, 0.f } },
            { { 0.f, 0.f, 1.f } },
            { { 1.f, 0.f, 0.f } },
            { { 0.f, 0.f, 1.f } },
            { { 1.f, 0.f, 0.f } }

        };

        auto elems = new GLuint[14] {0,1,2,3,4,1,5,6,4,7,2,6,0,1};

        auto v     = new Buffer<  vec3<ATTRIB_VERT> > (   GL_ARRAY_BUFFER, 8, 
                                                        cube, GL_STATIC_DRAW );

        auto c1    = new Buffer< vec3<ATTRIB_COLOR> > (   GL_ARRAY_BUFFER, 8, 
                                                        col1, GL_STATIC_DRAW );

        auto c2    = new Buffer< vec3<ATTRIB_COLOR> > (   GL_ARRAY_BUFFER, 8, 
                                                        col2, GL_STATIC_DRAW );

        auto e     = new Buffer< GLuint > (      GL_ELEMENT_ARRAY_BUFFER, 14, 
                                                       elems, GL_STATIC_DRAW );

        shared_ptr< Buffer<  vec3<ATTRIB_VERT> > > sp_v(   v );
        shared_ptr< Buffer< vec3<ATTRIB_COLOR> > > sp_c1( c1 );
        shared_ptr< Buffer< vec3<ATTRIB_COLOR> > > sp_c2( c2 );
        shared_ptr< Buffer<             GLuint > > sp_e(   e );

        auto bp_c0 = shared_ptr<BufferPoly>(new BufferPoly( sp_v, sp_e, 14 ) );
        auto bp_c1 = shared_ptr<BufferPoly>(new BufferPoly( sp_v, sp_e, 14 ) );
        bp_c0->add_array( sp_c1 ); bp_c1->add_array( sp_c2 );

        shared_ptr<Renderable> sp_ic0( new BufferPoly::Instance( bp_c0 ) );
        shared_ptr<Renderable> sp_ic1( new BufferPoly::Instance( bp_c1 ) );

        delete[] cube; delete[] elems;

        engine.add_child( sp_ic0 );
        engine.add_child( sp_ic1 );

        sp_ic0->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );
        sp_ic1->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );

        sp_ic0->transform() = glm::translate( sp_ic0->transform(), glm::vec3( -2.f, 0.f, 0.f ) );
        sp_ic1->transform() = glm::translate( sp_ic1->transform(), glm::vec3(  2.f, 0.f, 0.f ) );

        sp_ic1->callback()  = sp_ic0->callback() = [](Renderable &r, const double &d) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(30*d), glm::vec3(1.f, 1.f, 0.f) );

        };

        engine.callback() = &engine_callback;

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}

void engine_callback( Renderable &r, const double &d )
{

    static int   lastMW  =        glfwGetMouseWheel();
    static float fov     =                        FOV;
    static glm::vec3 eye           ( 0.f, 0.f, 10.f ), 
                     center        (            0.f ), 
                     up            ( 0.f, 1.f,  0.f ),
                     xAxis         ( 1.f, 0.f,  0.f );

    // Zoom on mouse wheel.
    int mw               =        glfwGetMouseWheel();
    fov                 +=                lastMW - mw;
    int x, y;               glfwGetMousePos( &x, &y );
    int dx               =                x - SCR_C_X,
        dy               =                y - SCR_C_Y;

    if      ( fov < FOV_MIN ) fov = FOV_MIN;
    else if ( fov > FOV_MAX ) fov = FOV_MAX;

    if ( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
    {
        // pan
        glm::mat4 trans = glm::translate( glm::mat4(1.f), glm::vec3(

            -dx * TRN_SPD,
             dy * TRN_SPD,
                      0.f

        ) );

        eye    = glm::vec3( trans * glm::vec4( eye,    1.f ) );
        center = glm::vec3( trans * glm::vec4( center, 1.f ) );


    } else {
        // rotate

        glm::mat4 xRot = glm::rotate( glm::mat4(1.f), ROT_SPD*dy, xAxis );
        up             = glm::vec3(      xRot * glm::vec4(    up, 0.f ) );
        glm::mat4 yRot = glm::rotate( glm::mat4(1.f), ROT_SPD*dx,    up );
        xAxis          = glm::vec3(      yRot * glm::vec4( xAxis, 0.f ) );

        eye            = center + glm::vec3( yRot * xRot * glm::vec4( eye - center, 0.f ) );

    }

    // Reset on middle mouse button click.
    if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS )
    {
        fov    =                         FOV;
        eye    = glm::vec3( 0.f, 0.f, 10.f );
        center = glm::vec3(            0.f );
        up     = glm::vec3( 0.f, 1.f,  0.f );
        xAxis  = glm::vec3( 1.f, 0.f,  0.f );
    }

    glm::mat4 proj = glm::perspective( fov, SCR_W/SCR_H, NCP, FCP ),
              view = glm::lookAt( eye, center, up );

    r.transform()  = proj * view;

    lastMW = mw; glfwSetMousePos( SCR_C_X, SCR_C_Y );

}