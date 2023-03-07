
#ifndef GQY_CORE_H
#define GQY_CORE_H

#include <memory>

#ifndef GQY_PLATFORM_WINDOWS
#error Monte carlo path tracing only support Windows
#endif // !GQY_PLATFORM_WINDOWS

#ifdef GQY_MONTE_CARLO_PATH_TRACING_DEBUG
#define GQY_MONTE_CARLO_PATH_TRACING_ENABLE_ASSERTS
#endif // GQY_MONTE_CARLO_PATH_TRACING_DEBUG

#ifdef GQY_MONTE_CARLO_PATH_TRACING_ENABLE_ASSERTS
#define GQY_MONTE_CARLO_PATH_TRACING_ASSERT(x, ...) { if (!x) { GQY_MONTE_CARLO_PATH_TRACING_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GQY_MONTE_CARLO_PATH_TRACING_ASSERT(x, ...) { if (!x) { GQY_MONTE_CARLO_PATH_TRACING_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#endif // GQY_MONTE_CARLO_PATH_TRACING_ENABLE_ASSERTS

#endif // !GQY_CORE_H
