#include "extension_functions.h"

namespace ExtFun {
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}


VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysKHR(
        VkDevice device,
        VkCommandBuffer commandBuffer,
        const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
        const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
        uint32_t width,
        uint32_t height,
        uint32_t depth) {
  auto func = (PFN_vkCmdTraceRaysKHR)vkGetDeviceProcAddr(device, "vkCmdTraceRaysKHR");
  if (func != nullptr) {
    func(commandBuffer,
         pRaygenShaderBindingTable,
         pMissShaderBindingTable,
         pHitShaderBindingTable,
         pCallableShaderBindingTable,
         width,
         height,
         depth);
  }

}
} // namespace ExtFun
