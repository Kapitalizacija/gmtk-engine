#version 460 core

layout (location = 0) in vec2 aVert;

layout (location = 0) out vec2 texCoords;

uniform mat4 projection;

uniform vec3 pos;
uniform vec2 scale;
uniform float rotation;

layout(std430, binding = 1) buffer ssbo_offsets {
    vec2 texOffsets[];
};

void main() {
    mat2 rotationMatrix = mat2(
        vec2(cos(rotation), -sin(rotation)),
        vec2(sin(rotation), cos(rotation))
    );

    vec2 offsetCum = vec2(0);
    for (int i = 0; i <  gl_InstanceID; i++) {
        if (texOffsets[i].y == 0) {
            offsetCum.x += 1;
        }
        offsetCum.x += texOffsets[i].y;
    }

    gl_Position = projection * vec4((pos.xy + offsetCum * scale) + aVert * scale * rotationMatrix, pos.z, 1.0);

    texCoords = vec2(0.5, 0.5) + aVert;

    if (texOffsets[gl_InstanceID] == vec2(0)) {
        texCoords = vec2(0);
        return;
    }

    if (texCoords.x == 0) {
        texCoords.x = texOffsets[gl_InstanceID].x;
    } else {
        texCoords.x = texOffsets[gl_InstanceID].y;
    }

    texCoords.y = 1 - texCoords.y;
    
}

