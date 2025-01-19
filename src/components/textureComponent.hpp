#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "../config.cpp"

struct TextureComponent
{
    /* Should be Set Before 
    Create the Graphics Pipeline*/
    std::string texturePath;
    
    /* ID FOR BACKEND */
    uint32_t ID;
};

#endif