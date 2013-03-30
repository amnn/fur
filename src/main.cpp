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

        RenderEngine<GLFWScr>     engine                     ( SCR_W, SCR_H, FOV, NCP, FCP );
        shared_ptr<ShaderProgram> prog   ( new ShaderProgram( "assets/shaders/vert.glsl", 
                                                              "assets/shaders/frag.glsl" ) );

        engine.use_program( prog );

        auto cube = new vert_attrib[8] {

            { {  1.f,  1.f,  1.f }, { 1.f, 1.f, 1.f } },
            { {  1.f, -1.f,  1.f }, { 1.f, 1.f, 0.f } },
            { { -1.f,  1.f,  1.f }, { 1.f, 0.f, 1.f } },
            { { -1.f, -1.f,  1.f }, { 1.f, 0.f, 0.f } },
            { { -1.f, -1.f, -1.f }, { 0.f, 1.f, 1.f } },
            { {  1.f, -1.f, -1.f }, { 0.f, 1.f, 0.f } },
            { {  1.f,  1.f, -1.f }, { 0.f, 0.f, 1.f } },
            { { -1.f,  1.f, -1.f }, { 0.f, 0.f, 0.f } },

        };

        auto elems = new GLuint[14] {0,1,2,3,4,1,5,6,4,7,2,6,0,1};

        auto c0    = new BufferPoly<vert_attrib>( cube, 8, elems, 14 );
        
        shared_ptr<Renderable> spc0( c0 );
        
        delete[] cube; delete[] elems;

        engine.add_child( spc0 );

        spc0->transform() = glm::scale( glm::mat4(1.f), glm::vec3( 0.5f ) );
        /*spc0->callback()  = [](Renderable &r, const double &d) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(30*d), glm::vec3(1.f, 1.f, 0.f) );

        };*/

        engine.callback() = [](Renderable &r, const double &d) {

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

        };

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}