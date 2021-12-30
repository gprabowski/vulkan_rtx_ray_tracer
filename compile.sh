#!/bin/sh
$VULKAN_SDK/bin/glslc shaders/shader.vert -o spv/vert.spv
$VULKAN_SDK/bin/glslc shaders/shader.frag -o spv/frag.spv
