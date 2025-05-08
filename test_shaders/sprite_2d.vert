#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec4 aRotation;
layout (location = 3) in vec3 aScale;

layout (location = 0) out vec2 texCoords;
layout (location = 1) out flat int instanceID;

void main() {
    gl_Position = vec4(aPos.xy + aVert, 0.0, 1.0);

    texCoords = vec2(0.5, 0.5) - aVert;
    instanceID = gl_InstanceID;
}