#include "config.hpp"
#include "controller/application.hpp"
#include "controller/scene.hpp"

#include "loaders/objLoader.hpp" 

int main()
{
    /* Create Scene */
    Scene SceneObject;

    /* Create Entity */
    Entity vikingRoomEntity = SceneObject.makeEntity();
    MeshComponent vikingRoomMesh;
    TransformComponent vikingRoomTransform;
    ShaderComponent vikingRoomShader;
    TextureComponent vikingRoomTexture;

    vikingRoomMesh.filepath = "../../models/viking_room.obj";
    objLoader(vikingRoomMesh);
    vikingRoomTransform.position = glm::mat4(1.0f);
    vikingRoomShader.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    vikingRoomShader.fragmentSourcePath = "../../src/shaders/DefaultShaderFrag.spv";
    vikingRoomTexture.texturePath = "../../textures/viking_room.png";

    SceneObject.setEntityMeshComponent(vikingRoomEntity, vikingRoomMesh);
    SceneObject.setEntityTransformComponent(vikingRoomEntity, vikingRoomTransform);
    SceneObject.setEntityShaderComponent(vikingRoomEntity, vikingRoomShader);
    SceneObject.pushEntityTextureComponent(vikingRoomEntity, vikingRoomTexture);
    
    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.front = glm::vec3(0.5f, -0.5f, 0.0f);
    Camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    Camera.position = glm::vec3(0.5f, 0.5f, 0.0f);
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