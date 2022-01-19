#include "vertex.h"

bool Vertex::operator==(const Vertex &other) const
{
    return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
}

bool Vertex::operator<(const Vertex &other) const
{
    std::array<float, 8> v1 = {pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, texCoord.x, texCoord.y};
    std::array<float, 8> v2 = {other.pos.x,    other.pos.y,    other.pos.z,      other.normal.x,
                               other.normal.y, other.normal.z, other.texCoord.x, other.texCoord.y};
    for (int i = 0; i < 8; i++)
    {
        if (v2[i] > v1[i])
            return false;
        if (v2[i] < v1[i])
            return true;
    }
    return false;
}

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}

Vertex::Vertex(const glm::vec3 &pos, const glm::vec3 &normal, const glm::vec2 &texCoord, const uint32_t materialId)
    : pos(pos), normal(normal), texCoord(texCoord), materialId(materialId) {}

Vertex::Vertex()
    : pos({0, 0, 0}), normal(0, 0, 1), texCoord(0, 0), materialId(0) {}
