#line 2 110

/** 
  * Reorthogonalize tangent space with Gram-Schmidt. Its kind of expensive to do in the fragment shader
  * but we do it because we want to eliminate tangent seam artifacts.
  * For faster approaches one could also use a vertex shader approach like in https://sotrh.github.io/learn-wgpu/intermediate/tutorial11-normals
  * (but this lead to seams in our implementation) or the industry standard mikktspace.
  */
mat3 calcWorldToTangentMatrix(vec3 wN, vec3 wT) {
    wN = normalize(wN);
    wT = normalize(wT - dot(wT, wN) * wN);
    vec3 wB = cross(wN, wT);

    mat3 tangentToWorld = mat3(wT, wB, wN);
    mat3 worldToTangent = transpose(tangentToWorld); // inverse is transpose for orthonormal matrices
    return worldToTangent;
}