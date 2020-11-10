#pragma once

#define DLLATTRIB

#if defined SHARED_LIBRARY_EXPORT
#define DLLATTRIB __declspec(dllexport)
#elif defined SHARED_LIBRARY_IMPORT
#define DLLATTRIB __declspec(dllimport)
#endif
