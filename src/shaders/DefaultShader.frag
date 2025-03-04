#version 450 core

layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} scene;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(set = 2, binding = 0) uniform sampler _sampler;
layout(set = 2, binding = 1) uniform texture2D textures[2];

layout(push_constant) uniform TextureIndexPush {
    int textureIndex;
}texIndex;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(sampler2D(textures[texIndex.textureIndex], _sampler), fragTexCoord);
}