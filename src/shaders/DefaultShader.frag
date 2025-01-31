#version 450 core

layout(location = 0) in vec2 fragTexCoord;
layout(binding = 1) uniform sampler2D sampler;

layout(push_constant) uniform PER_OBJECT
{
    int textureIndex;
}texturePushConstant;

layout(location = 0) out vec4 outColor;

// update to add support for texture arrays. please, i beg

void main()
{
    outColor = texture(sampler2D(textures[texturePushConstant.textureIndex], samp), fragTexCoord);
};