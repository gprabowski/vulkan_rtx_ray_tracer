#ifndef VERTEX_H
#define VERTEX_H
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 normal;
    alignas(8) glm::vec2 texCoord;

    bool operator==(const Vertex &other) const;
    bool operator<(const Vertex &other) const;
    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

namespace std
{
template <>
struct hash<Vertex>
{
    size_t operator()(Vertex const &vertex) const
    {
        return hash<glm::vec3>()(vertex.pos) ^ hash<glm::vec3>()(vertex.normal) ^ hash<glm::vec2>()(vertex.texCoord);
    }
};
}  // namespace std

#endif  // VECTOR_H
