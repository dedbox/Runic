#pragma once

#ifdef RUNIC_PLATFORM_LINUX

#ifdef RUNIC_BUILD_LIB
#define RUNIC_API __attribute__((visibility("default")))
#else
#define RUNIC_API
#endif

#else

#error Runic only supports Linux!

#endif // RUNIC_PLATFORM_LINUX
