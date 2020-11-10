#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec3 colorIn;
layout(location = 1) in vec2 textureCoordinatesIn;

layout(location = 0) out vec4 colorOut;

void main() {
  colorOut = texture(textureSampler, textureCoordinatesIn);  // vec4(textureCoordinatesIn, 0.0, 1.0);
}
