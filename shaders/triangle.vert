#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferData {
  mat4 model;
  mat4 view;
  mat4 projection;
}
ubo;

layout(push_constant) uniform PushConstantData {
  mat4 model;
}
pc;

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 colorIn;
layout(location = 2) in vec2 textureCoordinatesIn;

layout(location = 0) out vec3 colorOut;
layout(location = 1) out vec2 textureCoordinatesOut;

void main() {
  gl_Position           = ubo.projection * ubo.view * ubo.model * vec4(positionIn, 1.0);
  colorOut              = colorIn;
  textureCoordinatesOut = textureCoordinatesIn;
}
