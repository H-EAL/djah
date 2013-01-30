f2fConnector distanceFragment(v2fConnector v2f,

  uniform sampler2D colorTex,

  uniform sampler2D normalTex,

  uniform sampler3D distanceTex,

  uniform float3 normalizationFactor)

{

  f2fConnector f2f;



  // Normalize the offset vector in texture space.



  // The normalization factor ensures we are normalized with respect

  // to a distance which is defined in terms of pixels.

  float3 offset = normalize(v2f.tanEyeVec);

  offset *= normalizationFactor;



  float3 texCoord = v2f.texCoord;



  // March a ray



  for (int i = 0; i < NUM_ITERATIONS; i++) {

    float distance = f1tex3D(distanceTex, texCoord);

    texCoord += distance * offset;

  }



  // Compute derivatives of unperturbed texcoords.

  // This is because the offset texcoords will have discontinuities



  // which lead to incorrect filtering.

  float2 dx = ddx(v2f.texCoord.xy);

  float2 dy = ddy(v2f.texCoord.xy);



  // Do bump-mapped lighting in tangent space.

  // 'normalTex' stores tangent-space normals remapped



  // into the range [0, 1].

  float3 tanNormal = 2 * f3tex2D(normalTex, texCoord.xy, dx, dy) - 1;

  float3 tanLightVec = normalize(v2f.tanLightVec);

  float diffuse = dot(tanNormal, tanLightVec);



  // Multiply diffuse lighting by texture color



  f2f.COL.rgb = diffuse * f3tex2D(colorTex, texCoord.xy, dx, dy);

  f2f.COL.a = 1;



  return f2f;



}