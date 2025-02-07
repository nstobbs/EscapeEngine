#ifndef SHADER_COMPONENT_H
#define SHADER_COMPONENT_H

#include "../config.cpp"
 
struct SceneUniformBuffer
{
    glm::mat4 view;
    glm::mat4 proj;
    // should manager the sampler too.
};

struct ShaderComponent
{
    /* Should be Set Before 
    Create the Graphics Pipeline*/
    std::string vertexSourcePath;
    std::string fragmentSourcePath;
    
    /* ID FOR BACKEND */
    uint32_t ID;
    std::vector<uint32_t> textureIDs;
};

#endif