#pragma once

#define SCANNER_ASSERT(x, ...) { if(!(x)) { SCANNER_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SCANNER_CORE_ASSERT(x, ...) { if(!(x)) { SCANNER_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
