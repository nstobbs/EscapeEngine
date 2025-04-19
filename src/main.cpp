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

    /*
    std::vector<std::string> vikingRoomTextureFilepaths;
    vikingRoomTextureFilepaths.push_back("../../textures/viking_room.png");
    Entity vikingRoom = Factor.createOBJMesh("../../models/viking_room.obj", vikingRoomTextureFilepaths);
    
    
    std::vector<std::string> cityTextureFilepaths;
    cityTextureFilepaths.push_back("../../textures/citytokio.png");
    Entity city = Factor.createOBJMesh("../../models/citytokio.obj", cityTextureFilepaths);
    */

    /* Boids Testing */
    Entity boids = SceneObject.makeEntity();

    TransformComponent boidsTransform;
    boidsTransform.position = glm::mat4(1.0f);

    ShaderComponent boidsShaders;
    boidsShaders.fragmentSourcePath = "../../src/shaders/BoidsFrag.spv";
    boidsShaders.vertexSourcePath = "../../src/shaders/BoidsVert.spv";

    MeshComponent boidsMesh;
    boidsMesh.filepath = "../../models/paperplane.obj";
    objLoader(boidsMesh);
    
    TextureComponent boidsTexture;
    boidsTexture.texturePath = "../../textures/paper.png";

    BoidsComponent boidsSim;
    boidsSim.boidsCount = 100;

    SceneObject.setEntityTransformComponent(boids, boidsTransform);
    SceneObject.setEntityShaderComponent(boids, boidsShaders);
    SceneObject.setEntityMeshComponent(boids, boidsMesh);
    SceneObject.pushEntityTextureComponent(boids, boidsTexture);
    SceneObject.setEntityBoidsComponent(boids, boidsSim);

    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.direction = glm::vec3(-0.5f, -0.5f, -0.5f);
    Camera.position = glm::vec3(4.0f, 3.5f, 4.5f);
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