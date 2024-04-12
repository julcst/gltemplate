#line 2 103
/**
 * Includes common functions and macros for debugging purposes
 */
 
/**
 * Uncomment following line to enable debug macros.
 * A uniform uDebug is required to be set to the index of the debug view
 * then you can specify different debug views by using the DEBUG_VIEW macro.
 */
//#define DEBUG
#ifdef DEBUG
#define RENDER_VIEW(c) if (uDebug == 0U) fragColor = c
#define DEBUG_VIEW(i, v) if (uDebug == uint(i)) fragColor = v
#else
#define RENDER_VIEW(c) fragColor = c
#define DEBUG_VIEW(i, v)
#endif

/**
 * Interprets angle as hue and converts it to RGB.
 */
vec3 angleToRGB(float angle) {
    float angle01 = angle / 6.283 + 0.5;
    return clamp(abs(mod(angle01 * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
}

vec3 checkerboard(vec2 uv) {
    vec2 p = floor(uv);
    return vec3(mod(p.x + p.y, 2.0));
}

vec3 checkerboard(vec2 uv, float steps) {
    vec2 p = floor(uv * steps);
    return vec3(mod(p.x + p.y, 2.0));
}

vec3 uvCheckerboard(vec2 uv, float steps) {
    return (checkerboard(uv, steps) * 0.5 + 0.5);
}

vec3 normalToRGB(vec3 normal) {
    return normal * 0.5 + 0.5;
}

vec3 normalToRGB(vec2 normal) {
    return vec3(normal * 0.5 + 0.5, 0.0);
}

vec3 boolToRGB(bool b) {
    return b ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
}

vec3 getColor(int i) {
    switch (i) {
        case 0: return vec3(0.0, 0.0, 1.0);
        case 1: return vec3(0.0, 1.0, 0.0);
        case 2: return vec3(1.0, 0.0, 0.0);
        case 3: return vec3(1.0, 1.0, 0.0);
        case 4: return vec3(0.0, 1.0, 1.0);
        case 5: return vec3(1.0, 0.0, 1.0);
        case 6: return vec3(1.0, 1.0, 1.0);
        case 7: return vec3(1.0, 0.5, 0.0);
        case 8: return vec3(0.0, 1.0, 0.5);
        case 9: return vec3(0.5, 0.0, 1.0);
        case 10: return vec3(1.0, 0.0, 0.5);
        case 11: return vec3(0.5, 1.0, 0.0);
        case 12: return vec3(0.0, 0.5, 1.0);
        default: return vec3(0.0);
    }
}

vec3 colorDebug(float x) {
    if (isnan(x)) return vec3(1.0, 0.0, 1.0);
    if (isinf(x)) return vec3(1.0, 0.0, 0.0);
    if (x <  0.0) return vec3(0.0, 0.0, 1.0);
    if (x >  1.0) return vec3(0.0, 1.0, 0.0);
    return vec3(x);
}

vec3 colorDebug(vec3 x) {
    if (any(isnan(x))) return vec3(1.0, 0.0, 1.0);
    if (any(isinf(x))) return vec3(1.0, 0.0, 0.0);
    if (any(lessThan(x, vec3(0.0)))) return vec3(0.0, 0.0, 1.0);
    if (any(greaterThan(x, vec3(1.0)))) return vec3(0.0, 1.0, 0.0);
    return x;
}

vec3 colorDebugEdges(float x) {
    if (isnan(x)) return vec3(1.0, 0.0, 1.0);
    if (isinf(x)) return vec3(1.0, 0.0, 0.0);
    if (x <  0.0) return vec3(0.0, 0.0, 1.0);
    if (x == 0.0) return vec3(0.0, 0.0, 0.1);
    if (x == 1.0) return vec3(0.9, 1.0, 0.9);
    if (x >  1.0) return vec3(0.0, 1.0, 0.0);
    return vec3(x);
}

vec3 colorDebugEdges(vec3 x) {
    if (any(isnan(x))) return vec3(1.0, 0.0, 1.0);
    if (any(isinf(x))) return vec3(1.0, 0.0, 0.0);
    if (any(lessThan(x, vec3(0.0)))) return vec3(0.0, 0.0, 1.0);
    if (any(greaterThan(x, vec3(1.0)))) return vec3(0.0, 1.0, 0.0);
    if (any(equal(x, vec3(1.0)))) return vec3(0.0, 0.0, 0.1);
    if (any(equal(x, vec3(0.0)))) return vec3(0.9, 1.0, 0.9);
    return x;
}

vec3 checkBarycentrics(vec4 bary) {
    if (any(lessThan(bary, vec4(0.0)))) return vec3(0.0, 0.0, 1.0);
    if (any(greaterThan(bary, vec4(1.0)))) return vec3(1.0, 0.0, 0.0);
    if (dot(bary, vec4(1.0)) > 1.0) return vec3(1.0, 0.0, 1.0);
    if (dot(bary, vec4(1.0)) < 1.0) return vec3(0.0, 1.0, 1.0);
    return vec3(0.0, 1.0, 0.0);
}
vec3 checkBarycentrics(vec3 bary) { return checkBarycentrics(vec4(bary, 0.0)); }
vec3 checkBarycentrics(vec2 bary) { return checkBarycentrics(vec4(bary, 0.0, 0.0)); }