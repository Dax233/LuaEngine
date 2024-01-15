#pragma once

using namespace loader;

#define DllExport   __declspec( dllexport )

namespace LuaCore {
#pragma region Lua Handle
	struct LuaScriptData {
		lua_State* L;
		string name;
		string file;
		bool start;
		LuaScriptData(
			lua_State* L = nullptr,
			string name = "",
			string file = "",
			bool start = true
		) :L(L), name(name), file(file), start(start) { };
	};
	vector<string> LuaFiles;
	map<string, LuaScriptData> LuaScript;
	lua_State* Lc;
	bool MemoryLog = false;
#pragma endregion
	//����ʱ��
	DllExport extern time_t reloadTime = 0;
	//����ص�
	static int LuaErrorCallBack(lua_State* L)
	{
		lua_Debug debug = {};
		//���������ĵĵ��ò㼶
		int rank = 0;
		//����ȡlua�׳��Ĵ���ֱ����ȡ��
		while (lua_getstack(L, rank, &debug)) {
			rank++;
		}
		//�ж�ջ���Ƿ�Ϊ�ַ�������
		int type = lua_type(L, -1);
		if (type != 4)
			return 0;
		std::string error = lua_tostring(L, -1);
		std::string errorShort = debug.short_src;
		int errorLine = debug.currentline;
		std::string errorMsg = "LuaEngine Error:\n" + error;
		lua_logger->error(errorMsg);
		LOG(ERR) << errorMsg;
		//��������Ϣѹ��ջ
		lua_pushstring(L, errorMsg.c_str());
		return 1;
	}
	static int LuaErrorRecord(string error) {
		if (error != "error in error handling") {
			lua_logger->error(error);
			LOG(ERR) << error;
		}
		return 1;
	}
	//����Lua��Ϸ����
	static void loadEngine(lua_State* L) {
		engine_logger->info("��ʼ��Engine.lua��������");
		int err = 0;
		err = luaL_dofile(L, "Lua\\Engine.lua");
		if (err != 0)
		{
			int type = lua_type(L, -1);
			if (type == 4) {
				string error = lua_tostring(L, -1);
				LuaErrorRecord(error);
			}
		}
	}
	//����Lua�ű�
	static void Lua_Load(string path, vector<string>& files)
	{
		engine_logger->info("��ʼ����Lua�ű�����");
		using namespace std::filesystem;
		if (exists(path) && is_directory(path))
		{
			for (auto& fe : directory_iterator(path))
			{
				auto fp = fe.path();
				auto temp = fp.filename();
				if (fp.extension().string() == ".lua" && temp.stem().string() != "Engine") {
					LuaCore::LuaScript[temp.stem().string()] = LuaCore::LuaScriptData(
						luaL_newstate(),
						temp.stem().string(),
						fp.string());
					files.push_back(temp.stem().string());
				}
			}
		}
	}
	//����lua�ű�
	DllExport extern int Lua_Run(lua_State* L, string LuaFile)
	{
		int err = 0;
		//��������
		loadEngine(L);
		err = luaL_dofile(L, LuaCore::LuaScript[LuaFile].file.c_str());
		if (err != 0)
		{
			int type = lua_type(L, -1);
			if (type == 4) {
				string error = lua_tostring(L, -1);
				LuaErrorRecord(error);
			}
			engine_logger->warn("Lua�ű�{}�ļ����س��ִ���", LuaFile);
			return -1;
		}
		//���ô���ص�����
		lua_pushcfunction(L, LuaErrorCallBack);
		engine_logger->info("����{}�ļ���Lua����", LuaFile);
		return 1;
	}
	//����lua����
	DllExport extern void run(string func, lua_State* runL = nullptr) {
		if (runL != nullptr) {
			int err = 0;
			int callBack = lua_gettop(runL);
			lua_getglobal(runL, func.c_str());
			if (lua_type(runL, -1) == LUA_TFUNCTION)
			{
				err = lua_pcall(runL, 0, 0, callBack);
				if (err != 0)
				{
					string error = lua_tostring(runL, -1);
					LuaCore::LuaErrorRecord(error);
				}
			}
		}
		else {
			for (string file_name : LuaCore::LuaFiles) {
				if (LuaCore::LuaScript[file_name].start) {
					lua_State* L = LuaCore::LuaScript[file_name].L;
					int err = 0;
					int callBack = lua_gettop(L);
					lua_getglobal(L, func.c_str());
					if (lua_isfunction(L, -1))
					{
						err = lua_pcall(L, 0, 0, callBack);
						if (err != 0)
						{
							string error = lua_tostring(L, -1);
							LuaCore::LuaErrorRecord(error);
						}
					}
					else {
						lua_pop(L, 1);
					}
				}
			}
		}
	}
	//Ϊluaע���º���
	static void Lua_register(string funcName, int(*func)(lua_State* pL)) {
		for (string file_name : LuaCore::LuaFiles) {
			if (LuaCore::LuaScript[file_name].start) {
				lua_State* L = LuaCore::LuaScript[file_name].L;
				lua_register(L, funcName.c_str(), func);
			}
		}
	}
	DllExport extern vector<string> getLuaFils() {
		return LuaFiles;
	}
	DllExport extern map<string, LuaScriptData> getLuas() {
		return LuaScript;
	}
}

