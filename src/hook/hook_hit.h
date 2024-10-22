#pragma once

#pragma region hit
namespace hook_hit {
	struct HitData {
		void* HitPlot;
		bool Invulnerable;
		int HitCount;
		HitData(
			void* HitPlot = nullptr,
			bool Invulnerable = false,
			int HitCount = 0)
			:HitPlot(HitPlot), Invulnerable(Invulnerable), HitCount(HitCount) {
		};
	};
	HitData Hit;
	SafetyHookMid g_hook_HitPtr;
	static void Hook() {
		framework_logger->info("�����ܻ�������");

		g_hook_HitPtr = safetyhook::create_mid(MH::Player::HitPtr,
			+[](SafetyHookContext& ctx) {
				Hit.HitCount += 1;
				Hit.HitPlot = *((void**)(ctx.rdx + 0x8));
				//����趨���޵У�������rcxΪ0���������������Լ�ִ��
				if (Hit.Invulnerable) ctx.rcx = 0;
			});
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע���ܻ���غ���");
		lua_register(L, "Invulnerable", [](lua_State* pL) -> int
			{
				Hit.Invulnerable = (bool)lua_toboolean(pL, 1);
				return 0;
			});
		lua_register(L, "GetHitCount", [](lua_State* pL) -> int
			{
				lua_pushinteger(pL, Hit.HitCount);
				return 1;
			});
		lua_register(L, "ResetHitCount", [](lua_State* pL) -> int
			{
				Hit.HitCount = 0;
				return 0;
			});
	}
}
#pragma endregion
