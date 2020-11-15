#version 450
#extension GL_ARB_separate_shader_objects : enable

vec3 tonemap(vec3 x) {
  float a = 2.51;
  float b = 0.03;
  float c = 2.43;
  float d = 0.59;
  float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

struct LightComponent {
  vec3  position;
  vec3  direction;
  vec3  color;
  float size;
  uint  type;
};

layout(binding = 0) uniform UniformBufferData {
  mat4           model;
  mat4           view;
  mat4           projection;
  float          lightSourceCount;
  LightComponent lightSources[64];
}
ubo;

layout(binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec3 colorIn;
layout(location = 1) in vec2 textureCoordinatesIn;
layout(location = 2) in vec3 normalIn;
layout(location = 3) in vec3 positionIn;

layout(location = 0) out vec4 colorOut;

void main() {
  vec3 color = texture(textureSampler, textureCoordinatesIn).rgb * colorIn;

  vec3 ambientLight     = vec3(0.0);
  vec3 pointLight       = vec3(0.0);
  vec3 directionalLight = vec3(0.0);

  for (uint i = 0; i < floor(ubo.lightSourceCount); ++i) {
    LightComponent light = ubo.lightSources[i];

    if (light.type == 0)  // Directional
      directionalLight += max(0.0, pow(dot(normalIn, light.direction), 8.0)) * light.color;
    else if (light.type == 1)  // Point
      pointLight += mix(1.0, 0.0, pow(clamp(0.0, 1.0, distance(light.position, positionIn) / light.size), 0.2)) * light.color;
    else if (light.type == 2)  // Ambient
      ambientLight += light.color;
  }

  if (ubo.lightSourceCount > 0.0)
    color *= ambientLight + directionalLight + pointLight;

  color.rgb = pow(color.rgb, vec3(2.2));
  color.rgb = tonemap(color.rgb);
  color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
  colorOut  = vec4(color, 1.0);
}
