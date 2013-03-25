#ifndef DEBUGRENDERABLE_H
#define DEBUGRENDERABLE_H

#include "Renderable.h"

class DebugRenderable : public Renderable
{

    int id;

public:

    DebugRenderable( int _id ) : id { _id } {};

    void render()
    {
        std::cout << "DebugRenderable " << id << " Rendered!\n";
    }

};

#endif