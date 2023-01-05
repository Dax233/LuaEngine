#pragma once
#include "lua_register.h"

using namespace loader;

namespace LuaCore {
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
		//��������Ϣѹ��ջ
		lua_pushstring(L, errorMsg.c_str());
		return 1;
	}
	static int LuaErrorRecord(string error) {
		LOG(ERR) << error;
		return 1;
	}
	//��ȡĿ¼�е��ļ�
	static void Lua_Load(string path, vector<string>& files)
	{
		using namespace std::filesystem;
		if (exists(path) && is_directory(path))
		{
			for (auto& fe : directory_iterator(path))
			{
				auto fp = fe.path();
				auto temp = fp.filename();
				if (fp.extension().string() == ".lua") {
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
		err = luaL_dofile(L, LuaHandle::LuaScript[LuaFile].file.c_str());
		if (err != 0)
		{
			int type = lua_type(L, -1);
			if (type == 4) {
				string error = lua_tostring(L, -1);
				LuaErrorRecord(error);
			}
			return -1;
		}
		//���ô���ص�����
		lua_pushcfunction(L, LuaErrorCallBack);
		LOG(INFO) << LuaFile;
		return 1;
	}
}

