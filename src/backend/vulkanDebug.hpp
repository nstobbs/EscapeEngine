#ifndef VULKAN_DEBUG_H
#define VULKAN_DEBUG_H

#include "../config.hpp"

/* Validation Layers Settings */
#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

bool checkValidationLayerSupport();

#endif