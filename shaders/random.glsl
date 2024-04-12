#line 2 105
/**
 * Hash functions based on the recommendations in following paper:
 * 
 * Mark Jarzynski and Marc Olano, Hash Functions for GPU Rendering, 
 * Journal of Computer Graphics Techniques (JCGT), vol. 9, no. 3, 21-38, 2020
 * Available online http://jcgt.org/published/0009/03/02/
 * Code at: https://www.shadertoy.com/view/XlGcRh
 *
 * These are useful for procedural effects like noise.
 */

#define MAX_UINT 0xffffffffU
#define MAX_INT  0x7fffffff

//////////////// Mapping N -> 1 /////////////////
// Uses a linear combination of the input with prime factors from the supplementary material of the paper
uint seed(uvec2 p) { return 19u * p.x + 47u * p.y + 101u; }
uint seed(uvec3 p) { return 19u * p.x + 47u * p.y + 101u * p.z + 131u; }
uint seed(uvec4 p) { return 19u * p.x + 47u * p.y + 101u * p.z + 131u * p.w + 173u; }

/////// Mapping uint -> float (range 0-1) ///////
float mapf(uint v) { return float(v) / float(MAX_UINT); }
vec2 mapf(uvec2 v) { return vec2(v) / float(MAX_UINT); }
vec3 mapf(uvec3 v) { return vec3(v) / float(MAX_UINT); }
vec4 mapf(uvec4 v) { return vec4(v) / float(MAX_UINT); }

///////////// Mapping float -> uint /////////////
uint mapu(float v) { return floatBitsToUint(v); }
uvec2 mapu(vec2 v) { return floatBitsToUint(v); }
uvec3 mapu(vec3 v) { return floatBitsToUint(v); }
uvec4 mapu(vec4 v) { return floatBitsToUint(v); }

////////////////// 1 -> 1 hash //////////////////

/**
 * Inigo Quilez, Integer Hash - III, 2017
 * (https://www.shadertoy.com/view/llGSzw)
 */
uint hash1u(uint v) {
    // integer hash copied from Hugo Elias
	v = (v << 13U) ^ v;
    v = v * (v * v * 15731U + 789221U) + 1376312589U;
    return v;
}
uint hash1u(float v) { return hash1u(mapu(v)); }
float hash1f(uint v) { return mapf(hash1u(v)); }
float hash1f(float v) { return mapf(hash1u(mapu(v))); }

////////////////// 2 -> 1 hash //////////////////

/**
 * Inigo Quilez, Integer Hash - III, 2017
 * (https://www.shadertoy.com/view/4tXyWN)
 */
uint hash1u(uvec2 v) {
    uvec2 q = 1103515245U * ( (v>>1U) ^ (v.yx   ) );
    uint  n = 1103515245U * ( (q.x  ) ^ (q.y>>3U) );
    return n;
}
uint hash1u(vec2 v) { return hash1u(mapu(v)); }
float hash1f(uvec2 v) { return mapf(hash1u(v)); }
float hash1f(vec2 v) { return mapf(hash1u(mapu(v))); }

////////////////// 2 -> 2 hash //////////////////

/**
 * pcg2d, from:
 * Mark Jarzynski and Marc Olano, Hash Functions for GPU Rendering, 
 * Journal of Computer Graphics Techniques (JCGT), vol. 9, no. 3, 21-38, 2020
 * Available online http://jcgt.org/published/0009/03/02/
 * Code at: https://www.shadertoy.com/view/XlGcRh
 */
uvec2 hash2u(uvec2 v) {
    v = v * 1664525u + 1013904223u;
    v.x += v.y * 1664525u; v.y += v.x * 1664525u;
    v ^= v >> 16u;
    v.x += v.y * 1664525u; v.y += v.x * 1664525u;
    v ^= v >> 16u;
    return v;
}
uvec2 hash2u(vec2 v) { return hash2u(mapu(v)); }
vec2 hash2f(uvec2 v) { return mapf(hash2u(v)); }
vec2 hash2f(vec2 v) { return mapf(hash2u(mapu(v))); }

////////////////// 3 -> 3 hash //////////////////

/**
 * pcg3d, from:
 * Mark Jarzynski and Marc Olano, Hash Functions for GPU Rendering, 
 * Journal of Computer Graphics Techniques (JCGT), vol. 9, no. 3, 21-38, 2020
 * Available online http://jcgt.org/published/0009/03/02/
 * Code at: https://www.shadertoy.com/view/XlGcRh
 */
uvec3 hash3u(uvec3 v) {
    v = v * 1664525u + 1013904223u;
    v.x += v.y * v.z; v.y += v.z * v.x; v.z += v.x * v.y;
    v ^= v >> 16u;
    v.x += v.y * v.z; v.y += v.z * v.x; v.z += v.x * v.y;
    return v;
}
uvec3 hash3u(vec3 v) { return hash3u(mapu(v)); }
vec3 hash3f(uvec3 v) { return mapf(hash3u(v)); }
vec3 hash3f(vec3 v) { return mapf(hash3u(mapu(v))); }

////////////////// 4 -> 4 hash //////////////////

/**
 * pcg4d, from:
 * Mark Jarzynski and Marc Olano, Hash Functions for GPU Rendering, 
 * Journal of Computer Graphics Techniques (JCGT), vol. 9, no. 3, 21-38, 2020
 * Available online http://jcgt.org/published/0009/03/02/
 * Code at: https://www.shadertoy.com/view/XlGcRh
 */
uvec4 hash4u(uvec4 v) {
    v = v * 1664525u + 1013904223u;
    v.x += v.y * v.w; v.y += v.z * v.x; v.z += v.x * v.y; v.w += v.y * v.z;
    v ^= v >> 16u;
    v.x += v.y * v.w; v.y += v.z * v.x; v.z += v.x * v.y; v.w += v.y * v.z;
    return v;
}
uvec4 hash4u(vec4 v) { return hash4u(mapu(v)); }
vec4 hash4f(uvec4 v) { return mapf(hash4u(v)); }
vec4 hash4f(vec4 v) { return mapf(hash4u(mapu(v))); }