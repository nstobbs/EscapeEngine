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
    CardMeshComponent.vertices = {0.5f, 0.5f, 0.0f, //0
                                  -0.5f, 0.5f, 0.0f, //1
                                  -0.5f, -0.5f, 0.0f, //2
                                  0.5f, -0.5f, 0.0f}; //3
    
    CardMeshComponent.verticesCount = 6;
    CardMeshComponent.indices = {1, 0, 3, 3, 2, 1};
    SceneObject.setEntityMeshComponent(CardEntity, CardMeshComponent);

    TransformComponent CardTransformComponent;
    CardTransformComponent.position = glm::vec3(0.0f, 0.0f, 0.0f);
    SceneObject.setEntityTransformComponent(CardEntity, CardTransformComponent);

    ShaderComponent CardShaderComponent;
    CardShaderComponent.fragmentSourcePath = "../../src/shaders/DefaultShader.frag";
    CardShaderComponent.vertexSourcePath = "../../src/shaders/DefaultShader.vert";
    SceneObject.setEntityShaderComponent(CardEntity, CardShaderComponent);

    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
    Camera.up = glm::vec3(0.0f, 0.0f, 1.0f);
    Camera.position = glm::vec3(0.0f, 0.0f, 0.0f );
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