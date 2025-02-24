#version 450 core

layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} scene;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(set = 2, binding = 0) uniform sampler _sampler;
layout(set = 2, binding = 1) uniform texture2D textures[1];

layout(push_constant) uniform TextureIndexPush {
    int textureIndex;
}texIndex;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main()
{
    /* Grid Rendering Plan
        Create two modulo blocks.
            One for the big boxes
            And one for the samller boxes

        Generate lines from these blocks.
         
     */
    float size = 1.0f;
    float u = modf(fragTexCoord.y, size) / size;
    float v = modf(fragTexCoord.y, size) / size;
    outColor = vec4(u, v, 0.0f, 1.0f);
    //outColor = texture(sampler2D(textures[texIndex.textureIndex], _sampler), fragTexCoord);
}