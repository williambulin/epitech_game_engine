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

// layout(push_constant) uniform LightSourceData {
//   vec3 position;
//   vec3 direction;
//   vec3 color;
// }
// pc;

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 colorIn;
layout(location = 2) in vec2 textureCoordinatesIn;
layout(location = 3) in vec3 normalIn;

layout(location = 0) out vec3 colorOut;
layout(location = 1) out vec2 textureCoordinatesOut;
layout(location = 2) out vec3 normalOut;

void main() {
  gl_Position           = ubo.projection * ubo.view * ubo.model * vec4(positionIn, 1.0);
  colorOut              = colorIn;
  textureCoordinatesOut = textureCoordinatesIn;
  normalOut             = normalize(mat3(ubo.model) * normalize(normalIn));
}
