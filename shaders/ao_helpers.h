#ifndef AO_HELPERS_H
#define AO_HELPERS_H

//-----------------------------------------------------------------------------
// offsetPositionAlongNormal shifts a point on a triangle surface so that a
// ray bouncing off the surface with tMin = 0.0 is no longer treated as
// intersecting the surface it originated from.
//
// This code uses an improved technique by Carsten Wächter and
// Nikolaus Binder from "A Fast and Robust Method for Avoiding
// Self-Intersection" from Ray Tracing Gems (verion 1.7, 2020).
// The normal can be negated if one wants the ray to pass through
// the surface instead.
vec3 OffsetPositionAlongNormal(vec3 worldPosition, vec3 normal)
{
  // Convert the normal to an integer offset.
  const float int_scale = 256.0f;
  const ivec3 of_i      = ivec3(int_scale * normal);

  // Offset each component of worldPosition using its binary representation.
  // Handle the sign bits correctly.
  const vec3 p_i = vec3(  //
      intBitsToFloat(floatBitsToInt(worldPosition.x) + ((worldPosition.x < 0) ? -of_i.x : of_i.x)),
      intBitsToFloat(floatBitsToInt(worldPosition.y) + ((worldPosition.y < 0) ? -of_i.y : of_i.y)),
      intBitsToFloat(floatBitsToInt(worldPosition.z) + ((worldPosition.z < 0) ? -of_i.z : of_i.z)));

  // Use a floating-point offset instead for points near (0,0,0), the origin.
  const float origin     = 1.0f / 32.0f;
  const float floatScale = 1.0f / 65536.0f;
  return vec3(  //
      abs(worldPosition.x) < origin ? worldPosition.x + floatScale * normal.x : p_i.x,
      abs(worldPosition.y) < origin ? worldPosition.y + floatScale * normal.y : p_i.y,
      abs(worldPosition.z) < origin ? worldPosition.z + floatScale * normal.z : p_i.z);
}

//-----------------------------------------------------------------------------
// Implementation of GetRandDir

uint rngState = gl_LaunchIDEXT.x * 2000 + gl_LaunchIDEXT.y;

// Steps the random number generator and returns a floating-point value between
// 0 and 1 inclusive.
float StepAndOutputRNGFloat()
{
  // Condensed version of pcg_output_rxs_m_xs_32_32, with simple conversion to floating-point [0,1].
  rngState  = rngState * 747796405 + 1;
  uint word = ((rngState >> ((rngState >> 28) + 4)) ^ rngState) * 277803737;
  word      = (word >> 22) ^ word;
  return float(word) / 4294967295.0f;
}

// Gets a randomly chosen cosine-weighted direction within the unit hemisphere
// defined by the surface normal.
vec3 GetRandCosDir(vec3 norm)
{
  // To generate a cosine-weighted normal, generate a random point on a sphere:
  float theta      = 6.2831853 * StepAndOutputRNGFloat();  // Random in [0, 2pi]
  float z          = 2 * StepAndOutputRNGFloat() - 1.0;    // Random in [-1, 1]
  float r          = sqrt(1.0 - z * z);
  vec3  ptOnSphere = vec3(r * cos(theta), r * sin(theta), z);
  // Then add the normal to it and normalize to make it cosine-weighted on a
  // hemisphere:
  return normalize(ptOnSphere + norm);
}

//-----------------------------------------------------------------------------

#endif // AO_HELPERS_H
