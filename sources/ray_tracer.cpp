#include "ray_tracer.h"

void RayTracerApp::run()
{
    initWindow(static_cast<void *>(this));
    initVulkan();
    mainLoop();
    cleanup();
}

void RayTracerApp::initVulkan()
{
    createVulkanInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPools();
    createDepthResources();
    createFramebuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();

    loadModel(tutorial_model);
    createVertexBuffer();
    createIndexBuffer();

    loadRTGeometry(ray_model, std::string(LOW_POLY_MODEL_PATH));
    createRTVertexBuffer();
    createRTIndexBuffer();

    createRT_BLAS();
    createRT_TLAS();

    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}

bool RayTracerApp::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat RayTracerApp::findDepthFormat()
{
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkSurfaceFormatKHR RayTracerApp::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR RayTracerApp::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RayTracerApp::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

QueueFamilyIndices RayTracerApp::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    // logic to find queue family indices
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
        {
            indices.graphicsFamily = i;
            indices.computeFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        ++i;
    }

    if (!indices.isComplete())
        throw std::runtime_error("queue families not found");

    return indices;
}

void RayTracerApp::updateUniformBuffers(uint32_t currentImage)
{
    // using push constants would be faster
    // static auto startTime = std::chrono::high_resolution_clock::now();
    // auto currentTime = std::chrono::high_resolution_clock::now();
    // float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.3));
    ubo.view = glm::mat4(1);
    ubo.view = glm::rotate(ubo.view, camera.rotation_x, {1, 0, 0});
    ubo.view = glm::rotate(ubo.view, camera.rotation_y, {0, 1, 0});
    ubo.view = glm::translate(ubo.view, camera.pos);
    camera.dir = glm::vec4(0, 0, 1, 0) * ubo.view;
    camera.up = glm::vec4(0, 1, 0, 0) * ubo.view;
    ubo.inv_view = glm::inverse(ubo.view);
    ubo.proj =
        glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
    // necessary as GLM was designed with openGL in mind
    // and Vulkan reverts the y coord
    ubo.proj[1][1] *= -1;

    void *data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void RayTracerApp::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (ExtFun::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger");
    }
}

void RayTracerApp::processInputEvents()
{
    if (glfwGetKey(window, GLFW_KEY_W))
        camera.pos += camera.dir * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_S))
        camera.pos -= camera.dir * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_D))
        camera.pos += glm::normalize(glm::cross(camera.dir, camera.up)) * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_A))
        camera.pos -= glm::normalize(glm::cross(camera.dir, camera.up)) * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        camera.pos -= glm::normalize(camera.up) * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        camera.pos += glm::normalize(camera.up) * 0.1f;
}

void RayTracerApp::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        processInputEvents();
        glfwPollEvents();
        drawRasterFrame();
    }

    vkDeviceWaitIdle(device);
}

void RayTracerApp::drawRasterFrame()
{
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    // 1. acquire and image from the swapchain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
                                            VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    // Check if a previous frame is using this image (i.e. there is
    // its fence to wait on
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    // UniformBufferObject
    updateUniformBuffers(imageIndex);
    // 2. execture the command buffer with that image as attachment in the
    //      framebuffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {// means that stage of writing to the color
                                         // will wait for the specified semaphore
                                         // each entry in waitStages has its own semaphore of
                                         // the same index
                                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    // signal those after the buffer has finished execution
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit render queue");
    }

    // 3. return the image to the swapchain for presentation

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
    {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RayTracerApp::cleanupSwapChain()
{
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkFreeCommandBuffers(device, graphicsCommandPool, static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);

    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

void RayTracerApp::cleanup()
{
    cleanupSwapChain();

    vkDestroySampler(device, textureSampler, nullptr);

    vkDestroyImageView(device, textureImageView, nullptr);

    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexRTBuffer, nullptr);
    vkFreeMemory(device, vertexRTBufferMemory, nullptr);

    vkDestroyBuffer(device, blasScratchBuffer, nullptr);
    vkFreeMemory(device, blasScratchBufferMemory, nullptr);

    vkDestroyBuffer(device, indexRTBuffer, nullptr);
    vkFreeMemory(device, indexRTBufferMemory, nullptr);

    vkDestroyBuffer(device, blasBuffer, nullptr);
    vkFreeMemory(device, blasBufferMemory, nullptr);

    ExtFun::vkDestroyAccelerationStructureKHR(device, blas, nullptr);

    VkBuffer scratchBuffer;
    VkDeviceMemory scratchBufferMemory;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, graphicsCommandPool, nullptr);
    vkDestroyCommandPool(device, computeCommandPool, nullptr);
    vkDestroyCommandPool(device, presentCommandPool, nullptr);

    // logical device
    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers)
    {
        ExtFun::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    // order is really important with the following
    // two calls
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void RayTracerApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL RayTracerApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                           const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                           void *pUserData)
{
    std::cerr << std::endl << std::endl;
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    std::cerr << std::endl << std::endl;

    return VK_FALSE;
}

std::vector<char> RayTracerApp::readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}
