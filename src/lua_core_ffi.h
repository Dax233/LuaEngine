#pragma once

/// <summary>
/// Ϊ FFI �����ṩ�ļ��ݲ�
/// </summary>
extern "C" {
	typedef int (*LuaCore_mapStatesCallback) (lua_State* L);

	/// <summary>
	/// Ϊ���� lua_State ע��һ��ȫ�ֺ���
	/// </summary>
	DllExport void LuaCore_regGlobalFunc(const char* name, lua_CFunction func) {
		for (auto& s : LuaCore::LuaScript) {
			LuaCore::LuaScriptData& data = s.second;
			if (data.L != nullptr) {
				lua_register(data.L, name, func);
			}
		}
	}

	/// <summary>
	/// Ϊ���� lua_State Ӧ��һ���Զ���ص�����
	/// </summary>
	DllExport void LuaCore_mapStates(LuaCore_mapStatesCallback callback) {
		for (auto& s : LuaCore::LuaScript) {
			LuaCore::LuaScriptData& data = s.second;
			if (data.L != nullptr) {
				callback(data.L);
			}
		}
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
