#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;

void main()
{
    gl_Position = ubo.model * ubo.view * ubo.proj * vec4(inPosition, 1.0);
}