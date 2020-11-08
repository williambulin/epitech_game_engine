#pragma once

#if (defined(_WIN32) || defined(_WIN64))
#define OS_WINDOWS
#else
#define OS_LINUX
#define OS_OTHER
#endif
