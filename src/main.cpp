#include "config.hpp"
#include "controller/application.hpp"
#include "controller/scene.hpp"

int main()
{
    /* Create Scene */
    Scene SceneObject;

    /* Create Entity */
    Entity CardEntity = SceneObject.makeEntity();
    MeshComponent CardMeshComponent;
    CardMeshComponent.vertices = {-0.5f, -0.5f, 0.0f,  //0
                                   0.5f,  0.5f, 0.0f,  //1
                                   0.5f,  0.5f, 0.0f,  //2
                                  -0.5f,  0.5f, 0.0f}; //3
    CardMeshComponent.verticesCount = 4;
    CardMeshComponent.indices = {0, 1, 2, 2, 3, 0};
    CardMeshComponent.indicesCount =  6;
    SceneObject.setEntityMeshComponent(CardEntity, CardMeshComponent);

    TransformComponent CardTransformComponent;
    CardTransformComponent.position = glm::mat4(1.0f);
    SceneObject.setEntityTransformComponent(CardEntity, CardTransformComponent);

    ShaderComponent CardShaderComponent;
    CardShaderComponent.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    CardShaderComponent.fragmentSourcePath = "../../src/shaders/DefaultShaderFrag.spv";
    SceneObject.setEntityShaderComponent(CardEntity, CardShaderComponent);

    Entity BoxEntity = SceneObject.makeEntity();
    ShaderComponent BoxShaderComponent;
    BoxShaderComponent.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    BoxShaderComponent.fragmentSourcePath = "../../src/shaders/DefaultShaderFrag.spv";
    SceneObject.setEntityShaderComponent(BoxEntity, BoxShaderComponent);

    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
    Camera.up = glm::vec3(0.0f, 0.0f, 1.0f);
    Camera.position = glm::vec3(0.0f, 2.0f, -1.0f);
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