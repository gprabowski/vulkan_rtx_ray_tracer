#version 460
#extension GL_EXT_ray_tracing : require

layout(location = 0) rayPayloadInEXT Payload {
	//TODO
    vec3 hitValue;
} payload;

void main() {
     payload.hitValue = vec3(0.6f, 0.6f, 0.8f);
}
