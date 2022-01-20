#ifndef ray_tracer_H
#define ray_tracer_H

#include <algorithm>
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include <thread>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <options.h>
#include <QApplication>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "constants.h"
#include "extension_functions.h"
#include "vertex.h"

/*
- scalars have to be aligned by N (= 4 bytes for 32bit floats)
- vec2 must be aligned by 2N (= 8 bytes)
- vec3 / vec4 must be aligned by 4N (= 16 bytes)
- nested structure must be aligned by the base alignment
of its members rounded up to a multiple of 16
- mat4 matrix must have the same alignment as vec4
 */
struct UniformBufferObject
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 inv_view;
    alignas(16) glm::mat4 proj;
    alignas(16) glm::mat4 inv_proj;
    alignas(16) glm::vec4 ao_opt;
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> computeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value(); }
};

struct Model
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<tinyobj::material_t> materials;
    std::vector<uint32_t> materials_indices;
};

struct Rt_model
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};

struct Camera
{
    glm::vec3 pos = {0.0f, -2.0f, -5.0f};
    glm::vec3 dir = {0.0f, 0.0f, 1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    bool mouse_pressed = false;
    float last_mouse_x, last_mouse_y;
    float rotation_x = 0.0f, rotation_y = 0.0f;
};

struct Material
{
    alignas(16) float ambient[3];
    alignas(16) float diffuse[3];
    alignas(16) float specular[3];
    alignas(16) float emission[3];
};

struct RayTracerApp
{
    // DISPATCHABLE OBJECTS ============================
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue computeQueue;
    VkQueue presentQueue;
    std::vector<VkCommandBuffer> commandBuffers;

    // NON DISPATCHABLE OBJECTS =========================
    GLFWwindow *window;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool graphicsCommandPool;
    VkCommandPool presentCommandPool;
    VkCommandPool computeCommandPool;
    // semaphores for signaling that an mage has been acquired and
    // is ready for rendering
    std::vector<VkSemaphore> imageAvailableSemaphores;
    // semaphores for signaling that an mage has finished
    // rendering and is ready for presentation
    std::vector<VkSemaphore> renderFinishedSemaphores;
    // to check if work on GPU for this one
    // has finished
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
    bool framebufferResized = false;

    static constexpr int RTShadersCount = 3;

    // unused, but may be useful if we will want to change architecture
    // std::vector<VkImageView> raytracedImagesViews;
    // std::vector<VkImage> raytracedImages;
    // std::vector<VkDeviceMemory> raytracedImagesMemory;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer vertexRTDataBuffer;
    VkDeviceMemory vertexRTDataBufferMemory;

    VkBuffer vertexRTBuffer;
    VkDeviceMemory vertexRTBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkBuffer indexRTDataBuffer;
    VkDeviceMemory indexRTDataBufferMemory;

    VkBuffer indexRTBuffer;
    VkDeviceMemory indexRTBufferMemory;

    VkBuffer blasBuffer;
    VkDeviceMemory blasBufferMemory;

    VkBuffer tlasBuffer;
    VkDeviceMemory tlasBufferMemory;

    VkBuffer tlasScratchBuffer;
    VkDeviceMemory tlasScratchBufferMemory;

    VkBuffer blasScratchBuffer;
    VkDeviceMemory blasScratchBufferMemory;

    VkBuffer materialIndexBuffer;
    VkDeviceMemory materialIndexBufferMemory;

    VkBuffer materialBuffer;
    VkDeviceMemory materialBufferMemory;

    VkBuffer shaderBindingTableBuffer;
    VkDeviceMemory shaderBindingTableBufferMemory;

    VkBuffer instancesBuffer;
    VkDeviceMemory instancesBufferMemory;

    VkAccelerationStructureKHR blas;
    VkAccelerationStructureKHR tlas;

    VkDeviceAddress vertexRTBufferAddress;
    VkDeviceAddress indexRTBufferAddress;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    uint32_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    Model model;
    Rt_model ray_model;
    Camera camera;
    std::thread opt;

    std::unique_ptr<QApplication> app;
    std::unique_ptr<Options> options;

    // member functions
    void run();
    void createVulkanInstance();
    void initVulkan();
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
                               uint32_t mipLevels);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &imageMemory);
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    VkFormat findDepthFormat();
    void createDepthResources();
    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    void createTextureImage();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void createTextureImageView();
    void createTextureSampler();
    void createRayTracedImages();
    void createDescriptorPool();
    void createUniformBuffers();
    void createDescriptorSetLayout();

    void createIndexBuffer();
    void createVertexBuffer();
    void createMaterialsBuffer();

    void createRTIndexBuffer();
    void createRTVertexBuffer();
    void createRTDataVertexBuffer();
    void createRTDataIndexBuffer();
    void createRT_BLAS();
    void createRT_TLAS();

    void recreateSwapChain();
    void createDescriptorSets();
    void createSyncObjects();
    void createEndBufferFence();
    void createCommandBuffers();
    void createCommandPools();
    void createRTCommandBuffers();
    void createFramebuffers();
    void createRenderPass();
    VkShaderModule createShaderModule(const std::vector<char> &code);
    void createGraphicsPipeline();
    void createRTPipeline();
    void createShaderBindingTable();
    void createImageViews();
    void createSwapChain();
    void createSurface();
    void createLogicalDevice();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkRTExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void updateUniformBuffers(uint32_t currentImage);
    void setupDebugMessenger();
    void processInputEvents();
    void mainLoop();
    void drawRasterFrame();
    void drawRTFrame();
    void cleanupSwapChain();
    void cleanup();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);

    // setup utils
    void initWindow(void *userPtr);
    std::vector<const char *> getRequiredExtensions();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool checkValidationLayerSupport();

    // vulkan utils
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool);
    void endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkQueue queue);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void copyBufferSync(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void copyHtoDSync(VkDeviceSize bufferSize, void *trData, VkBuffer dBuffer, VkDeviceMemory bufferMemory);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory,
                      VkMemoryAllocateFlags allocFlags = VK_MEMORY_ALLOCATE_FLAG_BITS_MAX_ENUM);
    // asset utils
    std::vector<char> readFile(const std::string &filename);
    void loadRTGeometry(Rt_model &m, std::string path);

    void loadModel(Model &m, Rt_model &rt_m);

    void loadVertex(Vertex &vertex, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices);
    void loadVertices(std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices);
    void loadIndex(uint32_t index, std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices);
    void loadIndices(std::vector<uint32_t> &indices, std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices);

    void loadPlane(glm::vec3 center, glm::vec3 rotation, float height, float width, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices, uint32_t materialId);
    std::vector<Vertex> getSphereVertices(glm::vec3 center, float radius, int n, int m, uint32_t materialId);
    std::vector<uint32_t> getSphereIndices(int n, int m, std::vector<Vertex> &vertices, std::map<Vertex, uint32_t> &uniqueVertices);
    void loadSphere(glm::vec3 center, float radius, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices, uint32_t materialId);

    void loadGeneratedShapes(Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices);
};

#endif  // ray_tracer_H
