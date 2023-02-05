#pragma once

extern "C" void* _stdcall GetRBP160(void*);

#pragma region screen
namespace hook_screen {
	struct ScreenaData {
		float x;
		float y;
		ScreenaData(
			float x = 0,
			float y = 0)
			: x(x), y(y) {
		};
	};
	map<void*, ScreenaData> ScreenList;
	static void Hook() {
		framework_logger->info("������Ļ��������");
		MH_Initialize();
		HookLambda(MH::World::Screen,
			[](auto rcx) {
				/*
				void* Plot = nullptr;
				GetRBP160(&Plot);
				*/
				ScreenList[rcx] = ScreenaData(*offsetPtr<float>(rcx, 0xb0), *offsetPtr<float>(rcx, 0xb4));
				return original(rcx);
			});
		MH_ApplyQueued();
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע����Ļ��غ���");
		lua_register(L, "GetScreenPos", [](lua_State* pL) -> int
			{
				/*
				void* screenPtr = (void*)(long long)lua_tointeger(pL, 1);
				lua_pushnumber(pL, ScreenList[screenPtr].x);
				lua_pushnumber(pL, ScreenList[screenPtr].y);
				return 2;
				*/
				lua_newtable(pL);//����һ����񣬷���ջ��
				for (auto [e, data] : ScreenList) {
					if (e != nullptr) {
						lua_pushinteger(pL, (long long)e);//ѹ���ַ
						lua_newtable(pL);//ѹ������Ϣ��
						lua_pushstring(pL, "x");//����Id
						lua_pushinteger(pL, data.x);
						lua_settable(pL, -3);
						lua_pushstring(pL, "y");//����SubId
						lua_pushinteger(pL, data.y);
						lua_settable(pL, -3);
						lua_settable(pL, -3);//����������
					}
				}
				return 1;
			});
	}
}
#pragma endregion
