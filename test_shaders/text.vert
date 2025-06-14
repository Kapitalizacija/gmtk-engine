#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 0) out vec2 texCoords;

uniform mat4 projection;

uniform vec3 pos;
uniform vec2 scale;
uniform float rotation;
uniform int instanceCount;

layout(std430, binding = 1) buffer ssbo_offsets {
    float texOffsets[];
};

const int CHAR_DATA_SIZE = 7;

void main() {
    if (texOffsets[CHAR_DATA_SIZE * gl_InstanceID + 0] == texOffsets[CHAR_DATA_SIZE * gl_InstanceID + 2]) {
        gl_Position = vec4(-2, -2, 0.0, 1.0);
        return;
    }

    mat2 rotationMatrix = mat2(
        vec2(cos(rotation), -sin(rotation)),
        vec2(sin(rotation), cos(rotation))
    );

    vec2 offsetCum = vec2(0);
    for (int i = 0; i < gl_InstanceID; i++) {
        offsetCum.x += texOffsets[i * CHAR_DATA_SIZE + 4];
    }


    offsetCum.x /= float(instanceCount);

    vec2 adjustedScale = scale;
    adjustedScale.x /= float(instanceCount);

    gl_Position = projection * vec4((pos.xy + offsetCum * scale) + (aVert + vec2(texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 5],
     texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 6])) * adjustedScale, pos.z, 1.0);

    if (texOffsets[gl_InstanceID] == vec4(0)) {
        texCoords = vec2(0);
        return;
    }
    
    texCoords.x = texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 0];
    texCoords.y = texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 1];

    if (aVert.x > 0) {
        texCoords.x = texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 2];
    } 
    
    if (aVert.y < 0){
        texCoords.y = texOffsets[gl_InstanceID * CHAR_DATA_SIZE + 3];
    }
    
}

