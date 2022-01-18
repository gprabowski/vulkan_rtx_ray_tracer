#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 emission;
};

struct Vertex
{
    vec3 pos;
    vec3 color;
    vec2 texCoord;
};

hitAttributeEXT vec2 attribs;

layout(location = 0) rayPayloadInEXT Payload {
	//TODO
    vec3 hitValue;
} payload;


layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 inv_view;
    mat4 proj;
    mat4 inv_proj;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform accelerationStructureEXT topLevelAS;

layout(binding = 4) buffer IndexBuffer { uint data[]; } indexBuffer;
layout(binding = 5) buffer VertexBuffer { Vertex data[]; } vertexBuffer;
layout(binding = 6) buffer MaterialIndexBuffer { uint data[]; } materialIndexBuffer;
layout(binding = 7) buffer MaterialBuffer { Material data[]; } materialBuffer;


void main() {
    ivec3 indices = ivec3(indexBuffer.data[3 * gl_PrimitiveID + 0], indexBuffer.data[3 * gl_PrimitiveID + 1], indexBuffer.data[3 * gl_PrimitiveID + 2]);

    Vertex v0 = vertexBuffer.data[indices.x];
    Vertex v1 = vertexBuffer.data[indices.y];
    Vertex v2 = vertexBuffer.data[indices.z];

    vec3 barycentric = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);
    vec2 texCoord = v0.texCoord * barycentric.x + v1.texCoord * barycentric.y + v2.texCoord * barycentric.z;

    payload.hitValue = texture(texSampler, texCoord).rgb;
}
