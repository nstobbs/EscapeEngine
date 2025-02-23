#include "config.hpp"
#include "controller/application.hpp"
#include "controller/scene.hpp"
#include "entity/entityFactor.hpp"

int main()
{
    /* Create Scene */
    Scene SceneObject;

    /* Create Entity */
    EntityFactor Factor(&SceneObject);
    Entity Grid = Factor.createGrid();

    std::vector<std::string> vikingRoomTextureFilepaths;
    vikingRoomTextureFilepaths.push_back("../../textures/viking_room.png");
    Entity vikingRoom = Factor.createOBJMesh("../../models/viking_room.obj", vikingRoomTextureFilepaths);
    
    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.front = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera.up = glm::vec3(0.0f, 0.0f, -1.0f); // -1 is upwards in vulkan
    Camera.position = glm::vec3(1.5f, 3.0f, 2.0f);
    SceneObject.setActiveCamera(Camera);

    /* Create Application */
    Application App(SceneObject);

    /* Run Application */
    try
    {
        App.run();
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}