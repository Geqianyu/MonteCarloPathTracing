
#ifndef GQY_LOG_H
#define GQY_LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Monte_carlo_path_tracing
{
    class Log
    {
    public:
        Log() { };
        ~Log() { };

        static void init();

        inline static std::shared_ptr<spdlog::logger>& get_logger()
        {
            return m_logger;
        }

    private:
        static std::shared_ptr<spdlog::logger> m_logger;
    };
}

#define GQY_MONTE_CARLO_PATH_TRACING_ERROR(...)          ::Monte_carlo_path_tracing::Log::get_logger()->error(__VA_ARGS__)
#define GQY_MONTE_CARLO_PATH_TRACING_WARN(...)           ::Monte_carlo_path_tracing::Log::get_logger()->warn(__VA_ARGS__)
#define GQY_MONTE_CARLO_PATH_TRACING_INFO(...)           ::Monte_carlo_path_tracing::Log::get_logger()->info(__VA_ARGS__)
#define GQY_MONTE_CARLO_PATH_TRACING_TRACE(...)          ::Monte_carlo_path_tracing::Log::get_logger()->trace(__VA_ARGS__)

#endif // !GQY_LOG_H
