#ifndef CONFIG_H
#define CONFIG_H

#include <vulkan/vulkan.h>

#include "../lib/stb_image.h"

/* If there is any errors
related to GLFW and Vulkan
this move this part back into
config.cpp*/
#define GLFW_INCLUDE_VULKAN
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
