#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vulkan/vulkan.h>
#include <array>
#include <string>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

constexpr std::string_view MODEL_PATH = "assets/models/Windmill.obj";
constexpr std::string_view MODELS_FOLDER = "assets/models/";
constexpr std::string_view TEXTURE_PATH = "assets/textures/windmill.png";

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

constexpr std::array<const char *, 1> validationLayers = {"VK_LAYER_KHRONOS_validation"};

constexpr std::array<const char *, 2> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                          VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME};

constexpr std::array<const char *, 7> rtExtensions = {
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,

    // Additional, not directly mentioned in the tutorial
    VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,  // to be able to get buffer address
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,    // to be able to get buffer address
    VK_KHR_DEVICE_GROUP_EXTENSION_NAME,           //  to be able to pass pNext with Alloc Info Flags to
                                                  //  the createBuffer function (last arg)
    VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

#endif  // CONSTANTS_H
