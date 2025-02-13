#version 450

// uniforms
layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} sceneData;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord; 

void main()
{
    gl_Position = object.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}