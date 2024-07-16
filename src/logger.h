#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

static auto engine_logger = spdlog::basic_logger_mt("����", "logs/LuaEngine.log");
static auto framework_logger = spdlog::basic_logger_mt("���", "logs/LuaEngine.log");
static auto lua_logger = spdlog::basic_logger_mt("Lua", "logs/LuaEngine.log");
static auto error_logger = spdlog::basic_logger_mt("����", "logs/Error.log");
static auto memory_logger = spdlog::rotating_logger_mt("�ڴ�", "logs/Memory.log", 1048576, 2);