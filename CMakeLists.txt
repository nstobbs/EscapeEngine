cmake_minimum_required(VERSION 3.5..3.8)

include(FetchContent)

project(ESC)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED CONFIG)
find_package(Vulkan REQUIRED FATAL_ERROR)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

add_executable(ESC src/main.cpp
					src/config.cpp
					src/config.hpp
					src/entity/entity.hpp
					src/controller/scene.cpp
					src/controller/scene.hpp
					src/controller/application.cpp
					src/controller/application.hpp
					src/backend/vulkanContext.hpp
					src/backend/vulkanInitialize.cpp
					src/backend/vulkanInitialize.hpp
					src/backend/vulkanDebug.cpp
					src/backend/vulkanDebug.hpp
					src/backend/vulkanSwapChain.cpp
					src/backend/vulkanSwapChain.hpp
					src/backend/vulkanShader.cpp
					src/backend/vulkanShader.hpp
					src/backend/vulkanUtilities.cpp
					src/backend/vulkanUtilities.hpp
					src/backend/vulkanBuffer.cpp
					src/backend/vulkanBuffer.hpp
					src/backend/vulkanTexture.cpp
					src/backend/vulkanTexture.hpp
					src/backend/glfwInitialize.cpp
					src/backend/glfwInitialize.hpp
					src/components/meshComponent.hpp
					src/components/shaderComponent.hpp
					src/components/transformComponent.hpp
					src/components/textureComponent.hpp
					src/components/cameraComponent.hpp
					src/systems/renderSystem.cpp
					src/systems/renderSystem.hpp
					src/systems/cameraSystem.cpp
					src/systems/cameraSystem.hpp
					src/loaders/objLoader.cpp
					src/loaders/objLoader.hpp
					lib/stb_image.h
					lib/tiny_obj_loader.h)
					
target_link_libraries(ESC PRIVATE glfw glm::glm Vulkan::Vulkan)