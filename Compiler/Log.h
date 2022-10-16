#pragma once

#include <memory>

#include "Scanner/src/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Compiler {
    class Log {
    public:
        static void Init(std::string name);

        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}

#ifndef TIMETEST
    // Core log macros
#define CORE_TRACE(...)    Compiler::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     Compiler::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     Compiler::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    Compiler::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...)    Compiler::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#else
#define CORE_TRACE(...)
#define CORE_INFO(...)
#define CORE_WARN(...)
#define CORE_ERROR(...)
#define CORE_FATAL(...)
#endif





