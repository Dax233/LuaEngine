#pragma once
#include "lua_register.h"

using namespace loader;

namespace LuaCore {
	//���ƿ���
	bool luaframe = false;
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
		std::string errorMsg = "error:" + error + ",errorShort:" + errorShort
			+ ",line:" + std::to_string(errorLine);
		LOG(ERR) << errorMsg;
		//��������Ϣѹ��ջ
		lua_pushstring(L, errorMsg.c_str());
		return 1;
	}
	static int LuaErrorRecord(string error) {
		if(error != "error in error handling")
		LOG(ERR) << error;
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
					LuaHandle::LuaScript[temp.stem().string()] = LuaHandle::LuaScriptData(
						luaL_newstate(),
						temp.stem().string(),
						fp.string());
					files.push_back(temp.stem().string());
				}
			}
		}
	}
	//����lua�ű�
	static int Lua_Run(lua_State* L, string LuaFile)
	{
		int err = 0;
		//��������
		loadEngine(L);
		err = luaL_dofile(L, LuaHandle::LuaScript[LuaFile].file.c_str());
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
	static void run(string func) {
		for (string file_name : LuaHandle::LuaFiles) {
			if (LuaHandle::LuaScript[file_name].start) {
				lua_State* L = LuaHandle::LuaScript[file_name].L;
				int err = 0;
				int callBack = lua_gettop(L);
				lua_getglobal(L, func.c_str());
				if (lua_type(L, -1) == LUA_TFUNCTION)
				{
					err = lua_pcall(L, 0, 0, callBack);
					if (err != 0)
					{
						string error = lua_tostring(L, -1);
						LuaCore::LuaErrorRecord(error);
					}
				}
			}
		}
	}
	//Ϊluaע���º���
	static void Lua_register(string funcName, int(*func)(lua_State* pL)) {
		for (string file_name : LuaHandle::LuaFiles) {
			if (LuaHandle::LuaScript[file_name].start) {
				lua_State* L = LuaHandle::LuaScript[file_name].L;
				lua_register(L, funcName.c_str(), func);
			}
		}
	}
}

