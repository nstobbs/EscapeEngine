#ifndef CONFIG_H
#define CONFIG_H

#include <vulkan/vulkan.h>

// Thrid Party 
#include "../lib/stb_image.h"
#include "../lib/tiny_obj_loader.h"

//TODO UPDATE WITH GIT SUBMOBLUES 
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../lib/imgui/backends/imgui_impl_glfw.h"

/* If there is any errors
related to GLFW and Vulkan
this move this part back into
config.cpp*/
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <set>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <optional>
#include <algorithm>
#include <limits>
#include <fstream>

#include <chrono>

#endif
