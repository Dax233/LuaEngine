#pragma once

#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "lua.h"
#include "logger.h"

namespace fs = std::filesystem;

typedef int (*InitializeFunc)(lua_State*);
typedef int (*DeInitializeFunc)(lua_State*);

namespace Extension {
	std::unordered_map<std::string, HMODULE> extensions;

	int load(const std::string& dllPath) {
		HMODULE hModule = LoadLibraryA(dllPath.c_str());
		if (!hModule) {
			framework_logger->error("������չ {} ʧ��", dllPath);
			return -1;
		}
		extensions.insert(std::pair(dllPath, hModule));

		return 0;
	}

	int init(HMODULE hModule, lua_State* luaState) {
		// ��ȡ����ָ��  
		InitializeFunc initFunc = reinterpret_cast<InitializeFunc>(GetProcAddress(hModule, "Init"));
		if (!initFunc) {
			framework_logger->error("�޷�����չ�л�ȡ��ʼ������");
			return -1;
		}

		// ���ó�ʼ������  
		int result = initFunc(luaState);
		if (result != 0) {
			framework_logger->error("��չ��ʼ��ʧ�� code={}", result);
			return result;
		}

		return 0;
	};

	int deinit(HMODULE hModule, lua_State* luaState) {
		// ��ȡ����ָ��  
		InitializeFunc deinitFunc = reinterpret_cast<DeInitializeFunc>(GetProcAddress(hModule, "Deinit"));
		if (!deinitFunc) {
			framework_logger->error("�޷�����չ�л�ȡ��������");
			return -1;
		}

		// ������������  
		int result = deinitFunc(luaState);
		if (result != 0) {
			framework_logger->error("��չ����ʧ�� code={}", result);
			return result;
		}

		return 0;
	};

	int loadAll(const std::string& dirPath) {
		if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
			return 0;
		}
		int success_count = 0;
		try {
			for (const auto& entry : fs::directory_iterator(dirPath)) {
				if (entry.is_regular_file() && entry.path().extension() == ".dll") {
					std::string dllPath = entry.path().string();

					if (load(dllPath)) {
						success_count++;
					};
				}
			}
		}
		catch (const fs::filesystem_error& e) {
			framework_logger->error("��ȡ��չ�б�ʧ�ܣ�{}", e.what());
			return -1;
		}

		return success_count;
	}

	int initAll(lua_State* luaState) {
		int success_count = 0;

		for (auto pair : extensions) {
			if (init(pair.second, luaState)) {
				success_count++;
			};
		}

		return success_count;
	};

	int deinitAll(lua_State* luaState) {
		int success_count = 0;

		for (auto pair : extensions) {
			if (deinit(pair.second, luaState)) {
				success_count++;
			};
		}

		return success_count;
	};
}

