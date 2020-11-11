#version 450
#extension GL_ARB_separate_shader_objects : enable

struct LightComponent {
  vec3  position;
  vec3  direction;
  vec3  color;
  float strength;
  uint  type;
};

layout(binding = 0) uniform UniformBufferData {
  mat4           model;
  mat4           view;
  mat4           projection;
  float          lightSourceCount;
  LightComponent lightSources[4];
}
ubo;

layout(binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec3 colorIn;
layout(location = 1) in vec2 textureCoordinatesIn;
layout(location = 2) in vec3 normalIn;

layout(location = 0) out vec4 colorOut;

void main() {
  vec3 color = texture(textureSampler, textureCoordinatesIn).rgb;

  vec3 directionalLight = vec3(0.0);
  for (uint i = 0; i < floor(ubo.lightSourceCount); ++i) {
    LightComponent light = ubo.lightSources[i];
    if (light.type == 0)  // Directional
      directionalLight += max(0.0, pow(dot(normalIn, light.direction), 8.0)) * light.color;
  }

  color *= directionalLight;

  // color *= max(0.0, dot(normalIn, ubo.lightSource[0].direction));
  colorOut = vec4(color, 1.0);  // vec4(textureCoordinatesIn, 0.0, 1.0);
  // colorOut = vec4(normalIn, 1.0);
}
