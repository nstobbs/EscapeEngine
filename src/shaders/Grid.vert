#version 450

layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} scene;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord; 
layout(location = 1) out mat4 cameraPos;
layout(location = 2) out mat4 outPosition;

void main()
{
    gl_Position = scene.proj * scene.view * object.model *  vec4(inPosition, 1.0);
    
    
    fragTexCoord = inTexCoord;
    cameraPos = view;
    outPosition = scene.proj * scene.view * object.model *  vec4(inPosition, 1.0);
}