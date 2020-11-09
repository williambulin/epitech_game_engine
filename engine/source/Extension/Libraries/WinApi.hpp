#pragma once

#include "Extension/Language/Platform.hpp"

#ifdef OS_WINDOWS

#pragma warning(push)
#pragma warning(disable : 4068)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <Windows.h>

#pragma clang diagnostic pop
#pragma warning(pop)

#else
#error Operating System not supported
#endif
