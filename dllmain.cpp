#include <fstream>
#include <queue>
#include <functional>

#include <random>
#include <iostream>
#include <cmath>

#include <windows.h>
#include <Shlwapi.h>

#include <safetyhook.hpp>

#include "json/json.hpp"
#include "sol/sol.hpp"
#include "loader.h"
#include "ghidra_export.h"
#include "util.h"
#include <thread>

#include "game_utils.h"
#include "lua_core.h"
#include "lua_core_ffi.h"
#include "lua_register.h"
#include "lua_thread.h"

#include "hook_monster.h"
#include "hook_animals.h"
#include "hook_camera.h"
#include "hook_hit.h"
#include "hook_shlp.h"
#include "hook_frame.h"
#include "general.h"
#include <future>

using namespace loader;

SafetyHookInline g_hook_time;

#pragma region Basic data processing program
static void MassageCommand() {
	void* MassagePlot = *(undefined**)MH::World::Message;
	if (MassagePlot != nullptr) {
		string Massage = offsetPtr<char>(MassagePlot, 0xC0);
		string::size_type idx;
		//执行实时命令
		idx = Massage.find("luac:");
		if (idx != string::npos) {
			string command = Massage.substr(Massage.find("luac:") + 5);
			framework_logger->info("执行游戏内发出的Lua实时指令: {}", command);
			int err = 0;
			err = luaL_dostring(LuaCore::Lc, command.c_str());
			if (err != 0)
			{
				int type = lua_type(LuaCore::Lc, -1);
				if (type == 4) {
					string error = lua_tostring(LuaCore::Lc, -1);
					LuaCore::LuaErrorRecord(error);
				}
			}
			*offsetPtr<char>(MassagePlot, 0xC0) = *"";
		}
		//重载虚拟机
		idx = Massage.find("reload ");
		if (idx != string::npos) {
			string luae = Massage.substr(Massage.find("reload ") + 7);
			std::map<string, LuaCore::LuaScriptData>::iterator it;
			it = LuaCore::LuaScript.find(luae);
			if (it != LuaCore::LuaScript.end()) {
				framework_logger->info("重载脚本{}的Lua虚拟机", luae);
				// 注销hook函数
				hook_general::clearLuaHook(LuaCore::LuaScript[luae].L);
				lua_close(LuaCore::LuaScript[luae].L);
				LuaCore::LuaScript[luae].L = luaL_newstate();
				luaL_openlibs(LuaCore::LuaScript[luae].L);
				registerFunc(LuaCore::LuaScript[luae].L, luae);
				lua_thread::Registe(LuaCore::LuaScript[luae].L);
				hook_monster::Registe(LuaCore::LuaScript[luae].L);
				hook_animals::Registe(LuaCore::LuaScript[luae].L);
				hook_camera::Registe(LuaCore::LuaScript[luae].L);
				hook_hit::Registe(LuaCore::LuaScript[luae].L);
				hook_frame::Registe(LuaCore::LuaScript[luae].L);
				hook_shlp::Registe(LuaCore::LuaScript[luae].L);
				hook_general::Registe(LuaCore::LuaScript[luae].L);
				LuaCore::reloadTime = time(nullptr);;
				if (LuaCore::Lua_Run(LuaCore::LuaScript[luae].L, luae) != 1) {
					engine_logger->warn("脚本{}重载后出现异常，已停止该脚本继续运行", luae);
					LuaCore::LuaScript[luae].start = false;
				}
				else {
					engine_logger->info("脚本{}已完成重载操作，代码运行正常", luae);
					string message = "脚本" + luae + "已完成重载操作";
					MH::Chat::ShowGameMessage(*(undefined**)MH::Chat::MainPtr, (undefined*)&utils::string_To_UTF8(message)[0], -1, -1, 0);
					LuaCore::LuaScript[luae].start = true;
					LuaCore::run("on_init", LuaCore::LuaScript[luae].L);
				}
			}
			*offsetPtr<char>(MassagePlot, 0xC0) = *"";
		}
	}
}
//数据更新程序
static void updata() {
	//地图更新时清理数据
	void* TimePlot = utils::GetPlot(*(undefined**)MH::Player::PlayerBasePlot, { 0x50, 0x7D20 });
	if (TimePlot != nullptr && Chronoscope::NowTime > *offsetPtr<float>(TimePlot, 0xC24)) {
		framework_logger->info("游戏内发生场景变换，更新框架缓存数据");
		//清除计时器数据
		Chronoscope::ChronoscopeList.clear();
		//清除按键数据
		Keyboard::TempData::t_KeyCount.clear();
		Keyboard::TempData::t_KeyDown.clear();
		//清除Xbox手柄数据
		XboxPad::TempData::t_KeyCount.clear();
		XboxPad::TempData::t_KeyDown.clear();
		//钩子数据
		hook_frame::SpeedList.clear();
		engine_logger->info("运行脚本on_switch_scenes场景切换代码");
		LuaCore::run("on_switch_scenes");
	}
	//更新计时器
	Chronoscope::chronoscope();
	//更新手柄数据
	XboxPad::Updata();
	//执行玩家消息指令
	MassageCommand();
}
#pragma endregion

// 自定义的异常处理程序
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
	if (exceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
		exceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO) {
		// 在崩溃时记录日志
		LuaCore::logger.saveLogToFile();
	}
	// 继续崩溃处理流程
	return EXCEPTION_CONTINUE_SEARCH;
}

static void InitLogger() {
	engine_logger->set_level(spdlog::level::info);
	engine_logger->flush_on(spdlog::level::trace);
	framework_logger->set_level(spdlog::level::info);
	framework_logger->flush_on(spdlog::level::trace);
	lua_logger->set_level(spdlog::level::info);
	lua_logger->flush_on(spdlog::level::trace);
	error_logger->set_level(spdlog::level::info);
	error_logger->flush_on(spdlog::level::trace);
	memory_logger->set_level(spdlog::level::info);
	memory_logger->flush_on(spdlog::level::trace);

	SetUnhandledExceptionFilter(ExceptionHandler);
}

void LoadLuaFiles() {
	framework_logger->info("加载Lua文件");
	LuaCore::Lua_Load("Lua\\", LuaCore::LuaFiles);
	framework_logger->info("创建并运行实时脚本环境");
	LuaCore::Lc = luaL_newstate();
}

void ApplyDelayedHooks() {
	for (string file_name : LuaCore::LuaFiles) {
		if (LuaCore::LuaScript[file_name].start && LuaCore::LuaScript[file_name].L != nullptr) {
			lua_State* L = LuaCore::LuaScript[file_name].L;
			lua_thread::Registe(L);
			hook_monster::Registe(L);
			hook_animals::Registe(L);
			hook_camera::Registe(L);
			hook_hit::Registe(L);
			hook_frame::Registe(L);
			hook_shlp::Registe(L);
			hook_general::Registe(L);
		}
	}
	framework_logger->info("开始执行创建各功能钩子代码");
	hook_monster::Hook();
	hook_animals::Hook();
	hook_camera::Hook();
	hook_hit::Hook();
	hook_shlp::Hook();
	hook_frame::Hook();
}

void ApplyTimeHooks() {
	framework_logger->info("创建on_time钩子");
	g_hook_time = safetyhook::create_inline(MH::World::MapClockLocal, reinterpret_cast<void*>(
		+[](float* clock, float clock2) {
		// 更新基础数据
		updata();

		// 运行 Lua 虚拟机
		LuaCore::run("on_time");
		return g_hook_time.call<int>(clock, clock2);
		}));
}

void RegFuncs() {
	luaL_openlibs(LuaCore::Lc);
	registerFunc(LuaCore::Lc, "内置");

	for (string file_name : LuaCore::LuaFiles) {
		if (LuaCore::LuaScript[file_name].start && LuaCore::LuaScript[file_name].L != nullptr) {
			lua_State* L = LuaCore::LuaScript[file_name].L;
			luaL_openlibs(L);
			engine_logger->info("为 {} 脚本注册引擎初始化函数", file_name);
			registerFunc(L, file_name);
		}
	}
}

void RunLuaFiles() {
	for (string file_name : LuaCore::LuaFiles) {
		lua_State* L = LuaCore::LuaScript[file_name].L;
		if (LuaCore::Lua_Run(L, file_name) != 1) {
			engine_logger->warn("脚本{}运行过程中出现异常，已停止该脚本继续运行", file_name);
			LuaCore::LuaScript[file_name].start = false;
		}
	}
}

void RunOnInit() {
	engine_logger->info("运行脚本on_init初始化代码");
	LuaCore::run("on_init");
}

__declspec(dllexport) extern bool Load()
{
	LoadLuaFiles();
	ApplyDelayedHooks();
	RegFuncs();
	RunLuaFiles();
	RunOnInit();
	ApplyTimeHooks();

	return true;
}

/// <summary>
/// 添加 Dll 扫描目录
/// </summary>
/// <returns></returns>
BOOL AddDllSearchPath()
{
	TCHAR currentPath[MAX_PATH];
	TCHAR luaLibPath[MAX_PATH];

	// 获取进程文件路径
	DWORD dwResult = GetModuleFileName(NULL, currentPath, MAX_PATH);
	if (dwResult == 0 || dwResult == MAX_PATH)
	{
		return FALSE;
	}
	// 去掉文件名部分
	PathRemoveFileSpec(currentPath);
	// 添加子目录
	PathCombine(luaLibPath, currentPath, TEXT("Lua\\lib"));
	// 添加DLL搜索路径
	if (!SetDllDirectory(luaLibPath))
	{
		return FALSE;
	}
	return TRUE;
}

void mainThread() {
	// 先延迟2秒加载，以便其他组件注册完毕
	// TODO: 更高效的注册方法
	LoadLuaFiles();
	ApplyDelayedHooks();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	RegFuncs();
	RunLuaFiles();
	RunOnInit();
	ApplyTimeHooks();
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		if (!AddDllSearchPath()) { LOG(ERR) << "LuaEngine 错误：添加动态库扫描目录失败，已忽略。"; };
		InitLogger();
		// TODO: 在新线程中处理组件注册消息，以及完成模块初始化
		std::thread tmain(mainThread);
		tmain.detach();
		break;
	}
	}
	return TRUE;
}
