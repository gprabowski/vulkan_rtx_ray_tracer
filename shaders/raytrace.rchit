#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 emission;
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

layout(binding = 2) uniform accelerationStructureEXT topLevelAS;

layout(binding = 4) buffer IndexBuffer { uint data[]; } indexBuffer;
layout(binding = 5) buffer VertexBuffer { float data[]; } vertexBuffer;
layout(binding = 6) buffer MaterialIndexBuffer { uint data[]; } materialIndexBuffer;
layout(binding = 7) buffer MaterialBuffer { Material data[]; } materialBuffer;


void main() {


    payload.hitValue = vec3(0.0f, 0.0f, 0.0f);
}
