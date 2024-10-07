#pragma once

#pragma region frame
namespace hook_frame {
	map<void*, float> SpeedList;
	static void Hook() {
		framework_logger->info("��������֡���ʵ��ڹ���");
		MH_Initialize();
		HookLambda(MH::World::ActionFrameSpeed,
			[](auto entity) {
				for (auto [ptr, addSpeed] : SpeedList) {
					if (entity == ptr) {
						//0xa4�����ò���
						//1422373E1 -> 0x145224688 + 0x94 �����ǲ�֡������������޷��޸ģ���Ӧλ�õĹ��Ӳ���¼����ֻ�����б�֡�����ܣ�������
						float now_speed = *(float*)((long long)*(long long*)(0x1451238C8) + (*(int*)((long long)entity + 0x10) * 0xf8) + 0xa0);
						if (now_speed + addSpeed >= 0) {
							*(float*)((long long)*(long long*)(0x1451238C8) + (*(int*)((long long)entity + 0x10) * 0xf8) + 0xa0) = now_speed + addSpeed;
							//*(float*)((long long)*(long long*)(0x145224688) + 0x94) = now_speed + addSpeed;
							//*(float*)((long long)*(long long*)(0x145183e00) + (*(int*)((long long)utils::GetPlot(*(undefined**)MH::Player::PlayerBasePlot, { 0x50 }) + 0x10) * 0xf8) + 0xa4) = now_speed + addSpeed;
						}
						else {
							*(float*)((long long)*(long long*)(0x1451238C8) + (*(int*)((long long)entity + 0x10) * 0xf8) + 0xa0) = 0;
							//*(float*)((long long)*(long long*)(0x145224688) + 0x94) = 0;
							//*(float*)((long long)*(long long*)(0x145183e00) + (*(int*)((long long)utils::GetPlot(*(undefined**)MH::Player::PlayerBasePlot, { 0x50 }) + 0x10) * 0xf8) + 0xa4) = 0;
						}
					}
				}
			return original(entity);
			});
		MH_ApplyQueued();
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע�ᶯ��֡������غ���");
		lua_register(L, "AddFrameSpeed", [](lua_State* pL) -> int
			{
				SpeedList[(void*)(long long)lua_tointeger(pL, 1)] = (float)lua_tonumber(pL, 2);
				return 0;
			});
		lua_register(L, "GetAddFrameSpeed", [](lua_State* pL) -> int
			{
				lua_pushnumber(pL, SpeedList[(void*)(long long)lua_tointeger(pL, 1)]);
				return 1;
			});
	}
}
#pragma endregion
