#ifndef SHADER_COMPONENT_H
#define SHADER_COMPONENT_H

#include "../config.cpp"

struct ShaderComponent
{
    /* Should be Set Before 
    Create the Graphics Pipeline*/
    std::string vertexSourcePath;
    std::string fragmentSourcePath;
    
    /* ID FOR BACKEND */
    uint32_t ID;
};

#endif