#include "ray_tracer.h"

void RayTracerApp::loadIndex(uint32_t index, std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices) {

    m.indices.push_back(uniqueVertices[vertices[index]]);
    rt_m.indices.push_back(uniqueVertices[vertices[index]]);
}

void RayTracerApp::loadIndices(std::vector<uint32_t> &indices, std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices) {

    for (uint32_t index : indices) {
        loadIndex(index, vertices, m, rt_m, uniqueVertices);
    }
}

void RayTracerApp::loadVertex(Vertex &vertex, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices) {

    if (uniqueVertices.count(vertex) == 0)
    {
        uniqueVertices[vertex] = static_cast<uint32_t>(m.vertices.size());
        m.vertices.push_back(vertex);
        rt_m.vertices.push_back(vertex.pos.x);
        rt_m.vertices.push_back(vertex.pos.y);
        rt_m.vertices.push_back(vertex.pos.z);
    }
}

void RayTracerApp::loadVertices(std::vector<Vertex> &vertices, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices) {

    for (Vertex &vertex : vertices) {
        loadVertex(vertex, m, rt_m, uniqueVertices);
    }
}

void RayTracerApp::loadPlane(glm::vec3 center, glm::vec3 rotation, float height, float width, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices, uint32_t materialId) {

    auto mtx = glm::mat4(1);
    mtx = glm::translate(mtx, center);
    mtx = glm::rotate(mtx, rotation.x, {1, 0, 0});
    mtx = glm::rotate(mtx, rotation.y, {0, 1, 0});
    mtx = glm::rotate(mtx, rotation.z, {0, 0, 1});

    glm::vec3 normal = glm::vec3(mtx * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    std::vector<Vertex> vertices = {
        { glm::vec3(mtx * glm::vec4(+width / 2, 0.0f, +height / 2, 1.0f)), normal, { 0.0f, 0.0f }, materialId },
        { glm::vec3(mtx * glm::vec4(+width / 2, 0.0f, -height / 2, 1.0f)), normal, { 0.0f, 0.0f }, materialId },
        { glm::vec3(mtx * glm::vec4(-width / 2, 0.0f, -height / 2, 1.0f)), normal, { 0.0f, 0.0f }, materialId },
        { glm::vec3(mtx * glm::vec4(-width / 2, 0.0f, +height / 2, 1.0f)), normal, { 0.0f, 0.0f }, materialId },
    };

    loadVertices(vertices, m, rt_m, uniqueVertices);

    std::vector<uint32_t> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    loadIndices(indices, vertices, m, rt_m, uniqueVertices);
}

std::vector<Vertex> RayTracerApp::getSphereVertices(glm::vec3 center, float radius, int n, int m, uint32_t materialId) {

    std::vector<Vertex> vertices;

    vertices.push_back({ {center.x, center.y + radius, center.z }, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f }, materialId });

    float alphaDelta = M_PI / n;
    float alpha = alphaDelta;
    for (int i = 0; i < n - 1; i++) {

        float betaDelta = 2.0f * M_PI / m;
        float beta = 0.0f;
        for (int j = 0; j < m; j++) {

            glm::vec3 position = { radius * sinf(alpha) * cosf(beta), radius * cosf(alpha), radius * sinf(alpha) * sinf(beta) };
            position += center;
            glm::vec3 normal = { sinf(alpha) * cosf(beta), cosf(alpha), sinf(alpha) * sinf(beta) };

            vertices.push_back({ position, normal, { 0.0f, 0.0f }, materialId });

            beta += betaDelta;
        }
        alpha += alphaDelta;
    }

    vertices.push_back({ {center.x, center.y - radius, center.z }, {0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f }, materialId });

    return vertices;
}

std::vector<uint32_t> RayTracerApp::getSphereIndices(int n, int m, std::vector<Vertex> &vertices, std::map<Vertex, uint32_t> &uniqueVertices) {

    int verticesCount = (n - 1) * m + 2;

    std::vector<uint32_t> indices;
    int i, j;

    for (j = 0; j < m - 1; j++) {
        indices.push_back(0);
        indices.push_back(j + 2);
        indices.push_back(j + 1);
    }
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(m);

    for (i = 0; i < n - 2; i++) {
        for (j = 0; j < m - 1; j++) {
            indices.push_back(i * m + j + 1);
            indices.push_back(i * m + j + 2);
            indices.push_back((i + 1) * m + j + 2);
            indices.push_back(i * m + j + 1);
            indices.push_back((i + 1) * m + j + 2);
            indices.push_back((i + 1) * m + j + 1);
        }
        indices.push_back(i * m + j + 1);
        indices.push_back(i * m + 1);
        indices.push_back((i + 1) * m + 1);
        indices.push_back(i * m + j + 1);
        indices.push_back((i + 1) * m + 1);
        indices.push_back((i + 1) * m + j + 1);
    }

    for (j = 0; j < m - 1; j++) {
        indices.push_back(i * m + j + 1);
        indices.push_back(i * m + j + 2);
        indices.push_back(verticesCount - 1);
    }
    indices.push_back((i + 1) * m);
    indices.push_back(i * m + 1);
    indices.push_back(verticesCount - 1);

    return indices;
}

void RayTracerApp::loadSphere(glm::vec3 center, float radius, Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices, uint32_t materialId) {

    int N = 20;
    int M = 20;

    std::vector<Vertex> vertices = getSphereVertices(center, radius, N, M, materialId);
    loadVertices(vertices, m, rt_m, uniqueVertices);

    std::vector<uint32_t> indices = getSphereIndices(N, M, vertices, uniqueVertices);
    loadIndices(indices, vertices, m, rt_m, uniqueVertices);
}

void RayTracerApp::loadGeneratedShapes(Model &m, Rt_model &rt_m, std::map<Vertex, uint32_t> &uniqueVertices) {

    //ground
    loadPlane({0.0f, -0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, 20.0f, 20.0f, m, rt_m, uniqueVertices, 0);

    //mirrors
    loadPlane({10.0f, 10.0f - 0.2f, 0.0f}, {0.0f, 0.0f, M_PI_2}, 20.0f, 20.0f, m, rt_m, uniqueVertices, 1);
    loadPlane({-10.0f, 10.0f - 0.2f, 0.0f}, {0.0f, 0.0f, -M_PI_2}, 20.0f, 20.0f, m, rt_m, uniqueVertices, 1);

    loadSphere({5.0f, 5.0f, 5.0f}, 3.0f, m, rt_m, uniqueVertices, 1);
    loadSphere({-7.0f, 5.0f, -7.0f}, 1.5f, m, rt_m, uniqueVertices, 1);
    loadSphere({7.0f, 1.5f, -7.0f}, 1.0f, m, rt_m, uniqueVertices, 1);
    loadSphere({-7.0f, 3.0f, 7.0f}, 1.5f, m, rt_m, uniqueVertices, 1);
    loadSphere({0.0f, 9.5f, 0.0f}, 1.0f, m, rt_m, uniqueVertices, 1);
}

void RayTracerApp::loadModel(Model &m, Rt_model &rt_m)
{
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = MODELS_FOLDER;
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(std::string(MODEL_PATH), config))
    {
        if (reader.Error().empty())
        {
            std::cerr << "OBJ READER: " << reader.Error();
        }
        exit(1);
    }

    const auto &materials = reader.GetMaterials();
    const auto &shapes = reader.GetShapes();
    const auto &attrib = reader.GetAttrib();

    m.vertices.clear();
    m.indices.clear();
    m.materials.clear();
    m.materials_indices.clear();
    rt_m.vertices.clear();
    rt_m.indices.clear();

    std::map<Vertex, uint32_t> uniqueVertices{};
    m.materials = materials;

    loadGeneratedShapes(m, rt_m, uniqueVertices);

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.pos = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                          attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                               // we have 0 at the top while obj 0 at
                               // the bottom
                               1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.normal = {attrib.normals[3 * index.normal_index], attrib.normals[3 * index.normal_index + 1],
                             attrib.normals[3 * index.normal_index + 2]};
            // vertex.normal = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(m.vertices.size());
                m.vertices.push_back(vertex);

                rt_m.vertices.push_back(vertex.pos.x);
                rt_m.vertices.push_back(vertex.pos.y);
                rt_m.vertices.push_back(vertex.pos.z);
            }

            m.indices.push_back(uniqueVertices[vertex]);
            rt_m.indices.push_back(uniqueVertices[vertex]);
        }

        for (const auto &material_idx : shape.mesh.material_ids)
            m.materials_indices.push_back(material_idx);
    }
    return;
}

VkFormat RayTracerApp::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                           VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

void RayTracerApp::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format,
                               VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                               VkImage &image, VkDeviceMemory &imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(device, image, imageMemory, 0);
}

void RayTracerApp::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight,
                                   uint32_t mipLevels)
{
    // Check if image format support linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
    {
        throw std::runtime_error(
            "texture image format does not support"
            "linear blitting!");
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(graphicsCommandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; ++i)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        // blit command will wait on this transition
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        if (mipWidth > 1)
            mipWidth /= 2;
        if (mipHeight > 1)
            mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                         nullptr, 0, nullptr, 1, &barrier);

    endSingleTimeCommands(graphicsCommandPool, commandBuffer, graphicsQueue);
}

void RayTracerApp::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                         VkImageLayout newLayout, uint32_t mipLevels)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(graphicsCommandPool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    // if using a barrier to transfer queue family ownership then
    // these two fields should be the indices of the queue families.
    // otherwise must be set to IGNORED
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (hasStencilComponent(format))
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    // which types of operations that involve
    // the resource must happen before the barrier
    // and which operations that involve the resource must wait on the barrier
    // barrier.srcAccessMask
    // barrier.dstAccessMask

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer,
                         sourceStage,       // which pipeline stages should happen before barrier
                         destinationStage,  // stages to wait
                         0,                 // per region or not
                         0, nullptr,        // memory barriers
                         0, nullptr,        // buffer memory barriers
                         1, &barrier);      // image memory barriers

    endSingleTimeCommands(graphicsCommandPool, commandBuffer, graphicsQueue);
}

void RayTracerApp::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(graphicsCommandPool);
    // which part of buffer will be copied to which part of image
    VkBufferImageCopy region{};
    region.bufferOffset = 0;

    // 0 means tightly packed in memory
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(graphicsCommandPool, commandBuffer, graphicsQueue);
}
