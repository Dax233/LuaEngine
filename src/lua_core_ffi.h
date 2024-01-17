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
}
