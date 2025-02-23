#ifndef ENTITY_FACTOR_H
#define ENTITY_FACTOR_H

#include "../config.hpp"
#include "entity.hpp"
#include "../controller/scene.hpp"
#include "../loaders/objLoader.hpp"

class EntityFactor
{
    public:
        EntityFactor(Scene* scene);
        Entity createGrid();
        Entity createOBJMesh(std::string objFilePath, std::vector<std::string> textureFilepaths);
    
    private:
        Scene* m_scene;
};

#endif