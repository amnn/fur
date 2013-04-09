#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec4 vPos;
layout(location = 3) in vec2   uv;

out vec2 texUV;

void main() {
    gl_Position = MVP * vPos;
    texUV       =         uv;
}