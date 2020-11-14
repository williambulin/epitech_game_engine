#pragma once

#include "Extension/Language/Library.hpp"

#include "Vulkan/Image.hpp"
#include "Vulkan/Model.hpp"
#include "Vulkan/Renderer.hpp"

namespace Components {
  class Model;
}

class Components::Model final {
public:
  std::unique_ptr<Vulkan::Image> m_image{nullptr};
  std::unique_ptr<Vulkan::Model> m_model{nullptr};

public:
  DLLATTRIB explicit Model(Vulkan::Renderer &renderer, const std::string &modelPath, const std::string &texturePath) {
    m_image = renderer.createImage(texturePath);
    m_model = renderer.createModel(modelPath);
  }
};
