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
    
    std::vector<std::string> cityTextureFilepaths;
    cityTextureFilepaths.push_back("../../textures/citytokio.png");
    Entity city = Factor.createOBJMesh("../../models/citytokio.obj", cityTextureFilepaths);
    

    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.direction = glm::vec3(-0.5f, -0.5f, -0.5f);
    Camera.position = glm::vec3(1.75f, 1.75f, 1.75f);
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