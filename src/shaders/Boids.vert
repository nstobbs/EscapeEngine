#version 450

struct Boid {
    vec3 pos;
    vec3 dir;
};

layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} scene;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(set = 2, binding = 0) readonly buffer BoidsSSBOIn
{
    Boid boidsIn[ ];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 0) out vec2 fragTexCoord; 

void main()
{
    Boid currentBoid = boidsIn[gl_InstanceIndex];
    mat4 boidTransform = mat4(1.0f);
    gl_Position = scene.proj * scene.view * object.model *  vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}