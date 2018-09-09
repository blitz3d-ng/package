attribute highp vec3 bbVertexPosition;
attribute highp vec3 bbVertexNormal;
attribute highp vec4 bbVertexColor;

uniform lowp vec4 bbAmbientLight;
uniform mat4 bbProjMatrix;
uniform mat4 bbViewMatrix;
uniform mat4 bbWorldMatrix;

varying lowp vec4 bbColor;

void main() {
  mat4 bbModelViewMatrix = bbViewMatrix * bbWorldMatrix;
  mat4 bbModelViewProjectionMatrix = bbViewMatrix * bbWorldMatrix;

  // Calculate and normalize eye space normal
  vec3 ecNormal = vec3(bbModelViewMatrix * vec4(bbVertexNormal, 0.0));
  ecNormal = ecNormal / length(ecNormal);

  // Do light calculations
  float ecNormalDotLightDirection = max(0.0, dot(ecNormal, u_directionalLight.direction));
  float ecNormalDotLightHalfplane = max(0.0, dot(ecNormal, u_directionalLight.halfplane));

  // Diffuse light
  vec4 diffuseLight = ecNormalDotLightDirection * u_directionalLight.diffuseColor * u_material.diffuseFactor;

  // Specular light
  vec4 specularLight = vec4(0.0);
  if (ecNormalDotLightHalfplane > 0.0) {
      specularLight = pow(ecNormalDotLightHalfplane, u_material.shininess) * u_directionalLight.specularColor * u_material.specularFactor;
  }

  v_light = bbAmbientLight + diffuseLight + specularLight;
  gl_Position = u_mvpMatrix * bbVertexPosition;


  // mat4 bbModelViewProjectionMatrix=bbProjMatrix * bbViewMatrix * bbWorldMatrix;
  // bbColor = vec4(1.0, 1.0, 1.0, 1.0); // bbVertexColor;
  // gl_Position = bbModelViewProjectionMatrix * vec4(bbVertexPosition, 1.0);
}
