#version 330 core

in vec2 texUV;
out vec3 color;

uniform sampler2D tex;

void main() {

    color = texture( tex, texUV ).rgb;

}
