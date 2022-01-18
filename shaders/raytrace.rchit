#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_GOOGLE_include_directive : require

#include "ao_helpers.h"

#define AO_NUM 16

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
    int hitType;
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

    vec3 light_direction = normalize(vec3(-0.3f, 1.0f, -0.5f));
    vec3 cam = (ubo.inv_view * vec4(0.0f,0.0f,0.0f,1.0f)).xyz;

    vec3 pos = v0.pos*barycentric.x + v1.pos*barycentric.y + v2.pos*barycentric.z;
    pos = vec3(gl_ObjectToWorldEXT * vec4(pos, 1.0f));


    if (payload.hitType == 0) { //ray created in rgen shader
           uint  rayFlags = gl_RayFlagsOpaqueEXT;
           float tMin     = 0.001;
           float tMax     = 100.0;

           payload.hitType = 1;
           traceRayEXT(topLevelAS,     // acceleration structure
                       rayFlags,       // rayFlags
                       0xFF,           // cullMask
                       0,              // sbtRecordOffset
                       0,              // sbtRecordStride
                       0,              // missIndex
                       pos,            // ray origin
                       tMin,           // ray min range
                       light_direction,// ray direction
                       tMax,           // ray max range
                       0               // payload (location = 0)
           );
           vec2 texCoord = v0.texCoord * barycentric.x + v1.texCoord * barycentric.y + v2.texCoord * barycentric.z;
           vec3 color = payload.hitValue = texture(texSampler, texCoord).rgb;
           float intensity = 0.1f;

           if (payload.hitType == 0){//miss -> sun
               vec3 normal = v0.pos*barycentric.x + v1.pos*barycentric.y + v2.pos*barycentric.z;
               vec3 view = normalize(cam - pos);
               vec3 h = normalize(view + light_direction);
               intensity += 0.6f * clamp(dot(normal, light_direction), 0.0f, 1.0f);
               //TODO - for now some magic numbers, materials dana should be used
               //intensity += 0.5f * pow(clamp(dot(normal, h), 0.0f, 1.0f),16.0f);
               intensity += min(1.0f, intensity);
               payload.hitValue = color*intensity;


           } else if (payload.hitType == 1){
               vec2 texCoord = v0.texCoord * barycentric.x + v1.texCoord * barycentric.y + v2.texCoord * barycentric.z;
               payload.hitValue = texture(texSampler, texCoord).rgb*0.1;
           }

           // place for other effect such as AO

           // 1. Offset position with helper function
           vec3 normal = v0.pos*barycentric.x + v1.pos*barycentric.y + v2.pos*barycentric.z;
           vec3 pos_off = OffsetPositionAlongNormal(pos, normal);

           payload.hitType = 2;
           // maybe could be 0, we are already offsetting above
           tMin = 0.00001;
           // model dependent
           tMax = 10.0;

           for(int i = 0; i < 4; ++i){
           // generate direction
           vec3 dir = GetRandCosDir(normal);

           // trace
           traceRayEXT(
                    topLevelAS,
                    rayFlags,
                    0xFF,
                    0,
                    0,
                    0,
                    pos,
                    tMin,
                    dir,
                    tMax,
                    0
               );
           }

           //for mirrors probably wrap all this in something like if (is normal material) {do above} else {do mirror}


           payload.hitValue = color*intensity;
       } else if (payload.hitType == 1){
        // ray emitted above, shadow, we do nothing
    } else if(payload.hitType == 2)
    {
    } //TODO else if (payload.hitType == X){ something}

}
