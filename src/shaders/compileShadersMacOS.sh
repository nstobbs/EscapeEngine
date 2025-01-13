#!/bin/bash
/Users/nathanstobbs/VulkanSDK/1.2.176.1/macOS/bin/glslc DefaultShader.vert -o DefaultShaderVert.spv
/Users/nathanstobbs/VulkanSDK/1.2.176.1/macOS/bin/glslc DefaultShader.frag -o DefaultShaderFrag.spv
read -p "Press any key to continue..."