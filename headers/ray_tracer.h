#ifndef ray_tracer_H
#define ray_tracer_H

#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include <algorithm>

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

#include "constants.h"
#include "extension_functions.h"
#include "vertex.h"
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


/*
- scalars have to be aligned by N (= 4 bytes for 32bit floats)
- vec2 must be aligned by 2N (= 8 bytes)
- vec3 / vec4 must be aligned by 4N (= 16 bytes)
- nested structure must be aligned by the base alignment
of its members rounded up to a multiple of 16
- mat4 matrix must have the same alignment as vec4
 */
struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct model {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
};

struct RayTracerApp {

  // DISPATCHABLE OBJECTS ============================
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
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
  VkCommandPool commandPool;
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
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
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
  model tutorial_model;

  // member functions
  void run();
  void createVulkanInstance();
  void initVulkan();
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout,
                             uint32_t mipLevels);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);
  void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
                   VkFormat format, VkImageTiling tiling,
                   VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                   VkImage &image, VkDeviceMemory &imageMemory);
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  bool hasStencilComponent(VkFormat format);
  VkFormat findDepthFormat();
  void createDepthResources();
  void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth,
                       int32_t texHeight, uint32_t mipLevels);
  void createTextureImage();
  VkImageView createImageView(VkImage image, VkFormat format,
                              VkImageAspectFlags aspectFlags,
                              uint32_t mipLevels);
  void createTextureImageView();
  void createTextureSampler();
  void createDescriptorPool();
  void createUniformBuffers();
  void createDescriptorSetLayout();
  void createIndexBuffer();
  void createVertexBuffer();
  void recreateSwapChain();
  void createDescriptorSets();
  void createSyncObjects();
  void createCommandBuffers();
  void createCommandPool();
  void createFramebuffers();
  void createRenderPass();
  VkShaderModule createShaderModule(const std::vector<char> &code);
  void createGraphicsPipeline();
  void createImageViews();
  void createSwapChain();
  void createSurface();
  void createLogicalDevice();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
  void pickPhysicalDevice();
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  void updateUniformBuffers(uint32_t currentImage);
  void setupDebugMessenger();
  void mainLoop();
  void drawFrame();
  void cleanupSwapChain();
  void cleanup();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData);

  // setup utils
  void initWindow(void *userPtr);
  std::vector<const char *> getRequiredExtensions();
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  bool checkValidationLayerSupport();

  // vulkan utils
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);
  // asset utils
  std::vector<char> readFile(const std::string &filename);
  void loadModel(model &m);

};

#endif // ray_tracer_H