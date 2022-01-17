#include "extension_functions.h"

namespace ExtFun
{
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysKHR(VkDevice device, VkCommandBuffer commandBuffer,
                                             const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
                                             const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
                                             const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
                                             const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
                                             uint32_t width, uint32_t height, uint32_t depth)
{
    auto func = (PFN_vkCmdTraceRaysKHR)vkGetDeviceProcAddr(device, "vkCmdTraceRaysKHR");
    if (func != nullptr)
    {
        func(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable,
             pCallableShaderBindingTable, width, height, depth);
    }
}

VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureBuildSizesKHR(
    VkDevice device, VkAccelerationStructureBuildTypeKHR buildType,
    const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts,
    VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)
{
    auto func = (PFN_vkGetAccelerationStructureBuildSizesKHR)vkGetDeviceProcAddr(
        device, "vkGetAccelerationStructureBuildSizesKHR");
    if (func != nullptr)
    {
        func(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
    }
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureKHR(VkDevice device,
                                                                const VkAccelerationStructureCreateInfoKHR* pCreateInfo,
                                                                const VkAllocationCallbacks* pAllocator,
                                                                VkAccelerationStructureKHR* pAccelerationStructure)
{
    auto func = (PFN_vkCreateAccelerationStructureKHR)vkGetDeviceProcAddr(device, "vkCreateAccelerationStructureKHR");
    if (func != nullptr)
    {
        func(device, pCreateInfo, pAllocator, pAccelerationStructure);
    }
    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL
vkCmdBuildAccelerationStructuresKHR(VkDevice device, VkCommandBuffer commandBuffer, uint32_t infoCount,
                                    const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
                                    const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)
{
    auto func =
        (PFN_vkCmdBuildAccelerationStructuresKHR)vkGetDeviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");
    if (func != nullptr)
    {
        func(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
    }
}

VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
{
    auto func = (PFN_vkGetBufferDeviceAddress)vkGetDeviceProcAddr(device, "vkGetBufferDeviceAddress");
    if (func != nullptr)
    {
        return func(device, pInfo);
    }

    return VkDeviceAddress(nullptr);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureKHR(VkDevice device,
                                                             VkAccelerationStructureKHR accelerationStructure,
                                                             const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyAccelerationStructureKHR)vkGetDeviceProcAddr(device, "vkDestroyAccelerationStructureKHR");
    if (func != nullptr)
    {
        return func(device, accelerationStructure, pAllocator);
    }
}

VKAPI_ATTR VkDeviceAddress VKAPI_CALL
vkGetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)
{
    auto func = (PFN_vkGetAccelerationStructureDeviceAddressKHR)vkGetDeviceProcAddr(
        device, "vkGetAccelerationStructureDeviceAddressKHR");
    if (func != nullptr)
    {
        return func(device, pInfo);
    }

    return VkDeviceAddress(nullptr);
}

VkResult vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation,
                                        VkPipelineCache pipelineCache, uint32_t createInfoCount,
                                        const VkRayTracingPipelineCreateInfoKHR* pCreateInfos,
                                        const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    auto func = (PFN_vkCreateRayTracingPipelinesKHR)vkGetDeviceProcAddr(device, "vkCreateRayTracingPipelinesKHR");
    if (func != nullptr)
    {
        return func(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    }

    return VK_SUCCESS;
}

VkResult vkGetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup,
                                              uint32_t groupCount, size_t dataSize, void* pData)
{
    auto func =
        (PFN_vkGetRayTracingShaderGroupHandlesKHR)vkGetDeviceProcAddr(device, "vkGetRayTracingShaderGroupHandlesKHR");
    if (func != nullptr)
    {
        return func(device, pipeline, firstGroup, groupCount, dataSize, pData);
    }

    return VK_SUCCESS;
}

VkDeviceAddress vkGetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo)
{
    auto func = (PFN_vkGetBufferDeviceAddressKHR)vkGetDeviceProcAddr(device, "vkGetBufferDeviceAddressKHR");
    if (func != nullptr)
    {
        return func(device, pInfo);
    }

    return VkDeviceAddress(nullptr);
}

}  // namespace ExtFun
