#version 460 core

layout (location = 0) in vec2 aPos;

uniform mat4 cam;

uniform vec2 pos;
uniform vec2 size;

void main() {
    gl_Position = cam * vec4(aPos * size + pos, 0.0, 1.0);
}