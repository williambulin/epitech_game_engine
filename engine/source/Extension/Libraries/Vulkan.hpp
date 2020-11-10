#pragma once

#include "Extension/Language/Platform.hpp"

#pragma warning(push)
#pragma warning(disable : 4068)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#ifdef OS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>

#undef VK_MAKE_VERSION
#define VK_MAKE_VERSION(major, minor, patch) (((static_cast<std::uint32_t>(major)) << 22) | ((static_cast<std::uint32_t>(minor)) << 12) | (static_cast<std::uint32_t>(patch)))

#undef VK_NULL_HANDLE
#define VK_NULL_HANDLE nullptr

#pragma clang diagnostic pop
#pragma warning(pop)
