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
    vec3 boidPos = currentBoid.pos;
    vec3 boidDir = currentBoid.dir;
    
    vec3 forward = normalize(boidDir);
    vec3 world_up = vec3(0.0f, 0.0f, 1.0f);

    vec3 right;
    right = normalize(cross(world_up, right));

    vec3 up = cross(forward, right);
    mat4 rotation = mat4(1.0f);

    rotation[0][0] = right.x;
    rotation[1][0] = right.y;
    rotation[2][0] = right.z;

    rotation[0][1] = up.x;
    rotation[1][1] = up.y;
    rotation[2][1] = up.z;

    rotation[0][2] = forward.x;
    rotation[1][2] = forward.y;
    rotation[2][2] = forward.z;

    mat4 translation = mat4(1.0f);
    translation[3][0] = boidPos.x;
    translation[3][1] = boidPos.y;
    translation[3][2] = boidPos.z;

    mat4 final = translation * rotation;

    gl_Position = scene.proj * scene.view * final * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}