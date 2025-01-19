#define STB_IMAGE_IMPLEMENTATION

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
    // not really a vertex here, but each float. So each thrid float would be one whole vertex.
    CardMeshComponent.vertices = {-0.5f, -0.5f, 0.0f,  //0
                                   0.5f,  -0.5f, 0.0f,  //1
                                   0.5f,  0.5f, 0.0f,  //2
                                  -0.5f,  0.5f, 0.0f}; //3
    CardMeshComponent.verticesCount = 4; // I don't dont the issue with vertices being single floats matters cause of this
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

    TextureComponent CardTextureComponent;
    CardTextureComponent.texturePath = "../../textures/testTexture.png";

    /* Test 2 Objects */
    Entity Card2Entity = SceneObject.makeEntity();
    MeshComponent Card2MeshComponent;
    Card2MeshComponent.vertices = {-0.25f, -0.25f, 0.0f,  //0
                                   0.75f,  -0.25f, 0.0f,  //1
                                   0.75f,  0.75f, 0.0f,  //2
                                  -0.25f,  0.75f, 0.0f}; //3
    Card2MeshComponent.verticesCount = 4;
    Card2MeshComponent.indices = {0, 1, 2, 2, 3, 0};
    Card2MeshComponent.indicesCount = 6;
    SceneObject.setEntityMeshComponent(Card2Entity, Card2MeshComponent);
    TransformComponent Card2TransformComponent;
    Card2TransformComponent.position = glm::mat4(1.0f);
    SceneObject.setEntityTransformComponent(Card2Entity, Card2TransformComponent);
    ShaderComponent Card2ShaderComponent;
    Card2ShaderComponent.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    Card2ShaderComponent.fragmentSourcePath = "../../src/shaders/testFrag.spv";
    SceneObject.setEntityShaderComponent(Card2Entity, Card2ShaderComponent);

    /* Test 3 Objects */
    Entity Card3Entity = SceneObject.makeEntity();
    MeshComponent Card3MeshComponent;
    Card3MeshComponent.vertices = {0.0f, 0.0f, 0.0f,  //0
                                   1.0f,  0.0f, 0.0f,  //1
                                   1.0f,  1.0f, 0.0f,  //2
                                  0.0f,  1.0f, 0.0f}; //3
    Card3MeshComponent.verticesCount = 4;
    Card3MeshComponent.indices = {0, 1, 2, 2, 3, 0};
    Card3MeshComponent.indicesCount = 6;
    SceneObject.setEntityMeshComponent(Card3Entity, Card3MeshComponent);
    TransformComponent Card3TransformComponent;
    Card3TransformComponent.position = glm::mat4(1.0f);
    SceneObject.setEntityTransformComponent(Card3Entity, Card3TransformComponent);
    ShaderComponent Card3ShaderComponent;
    Card3ShaderComponent.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    Card3ShaderComponent.fragmentSourcePath = "../../src/shaders/test2Frag.spv";
    SceneObject.setEntityShaderComponent(Card3Entity, Card3ShaderComponent);

    /* Create Camera */
    CameraComponent Camera;
    Camera.focalLength = 45.0f;
    Camera.front = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera.up = glm::vec3(0.0f, 0.0f, 1.0f);
    Camera.position = glm::vec3(-1.0f, 0.0f, 1.0f);
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