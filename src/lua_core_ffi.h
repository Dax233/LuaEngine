#pragma once
#include "lua_register.h"

/// <summary>
/// Ϊ FFI �����ṩ�ļ��ݲ�
/// </summary>
extern "C" {
	/// <summary>
	/// Ϊ���� lua_State ע��һ��ȫ�ֺ���
	/// </summary>
	DllExport void LuaCoreAddFunction(const char* name, lua_CFunction func) {
		engine_logger->info("adding external function " + (string)name);
		LuaExternalData::StatelessFunctions[name] = func;
	}

	/// <summary>
	/// Ϊ���� lua_State ע��һ���Զ���ص�����
	/// </summary>
	DllExport void LuaCoreAddStateProcessor(StatesProcessor processor) {
		engine_logger->info("adding external processor");
		LuaExternalData::AllStatesProcessor.push_back(processor);
	}

	/// <summary>
	/// ע�������Ϣ
	/// </summary>
	DllExport void RegComponent(const char* name, int state) {
		engine_logger->info(format("received register signal from {} (state = {})", name, state));
		// TODO

	}

	/// <summary>
	/// <para>�����־</para>
	/// 
	/// ��־�ȼ���
	/// <para>0 -> DEBUG</para>
	/// <para>1 -> INFO</para>
	/// <para>2 -> WARN</para>
	/// <para>3 -> ERROR</para>
	/// </summary>
	/// <param name="level">
	/// ��־�ȼ�
	/// </param>
	/// <param name="content">��־����</param>
	DllExport void Log(int level, const char* content) {
		switch (level)
		{
		case 0:
			engine_logger->debug(content);
			break;
		case 1:
			engine_logger->info(content);
			break;
		case 2:
			engine_logger->warn(content);
			break;
		case 3:
			engine_logger->error(content);
			break;
		default:
			engine_logger->error(content);
			break;
		}
	}
}