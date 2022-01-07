#ifndef EXTENSIONFUNCTIONS_H
#define EXTENSIONFUNCTIONS_H
#include <vulkan/vulkan.h>

namespace ExtFun {
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator);

VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysKHR(
        VkCommandBuffer commandBuffer,
        const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
        uint32_t width,
        uint32_t height,
        uint32_t depth);

} // namespace ExtFun

#endif // EXTENSIONFUNCTIONS_H
