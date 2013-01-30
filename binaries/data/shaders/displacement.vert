v2fConnector distanceVertex(a2vConnector a2v,

  uniform float4x4 modelViewProj,

  uniform float3 eyeCoord,

  uniform float3 lightCoord,

  uniform float invBumpDepth)

{



  v2fConnector v2f;



  // Project position into screen space



  // and pass through texture coordinate

  v2f.projCoord = mul(modelViewProj, float4 (a2v.objCoord, 1));

  v2f.texCoord = float3 (a2v.texCoord, 1);



  // Transform the eye vector into tangent space.

  // Adjust the slope in tangent space based on bump depth



  float3 eyeVec = eyeCoord - a2v.objCoord;

  float3 tanEyeVec;

  tanEyeVec.x = dot(a2v.objTangent, eyeVec);

  tanEyeVec.y = dot(a2v.objBinormal, eyeVec);

  tanEyeVec.z = -invBumpDepth * dot(a2v.objNormal, eyeVec);

  v2f.tanEyeVec = tanEyeVec;



  // Transform the light vector into tangent space.

  // We will use this later for tangent-space normal mapping



  float3 lightVec = lightCoord - a2v.objCoord;

  float3 tanLightVec;

  tanLightVec.x = dot(a2v.objTangent, lightVec);

  tanLightVec.y = dot(a2v.objBinormal, lightVec);

  tanLightVec.z = dot(a2v.objNormal, lightVec);

  v2f.tanLightVec = tanLightVec;



  return v2f;



}