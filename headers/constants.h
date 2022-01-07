#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <string>
#include <vulkan/vulkan.h>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

constexpr std::string_view MODEL_PATH = "assets/models/viking_room.obj";
constexpr std::string_view TEXTURE_PATH = "assets/textures/viking_room.png";

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

constexpr std::array<const char *, 1> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

constexpr std::array<const char *, 1> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

constexpr std::array<const char*, 3> rtExtensions = {
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

#endif // CONSTANTS_H
