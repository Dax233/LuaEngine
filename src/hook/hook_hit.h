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
	static void Hook() {
		framework_logger->info("�����ܻ�������");
		MH_Initialize();
		HookLambda(MH::Player::HitPtr,
			[](auto rcx, auto rdx) {
				//�ܻ�ͳ�ƣ���������������߼����룬��������ִ��
				Hit.HitCount += 1;
				Hit.HitPlot = *((void**)(rdx + 0x8));
				//����趨���޵У�������rcxΪ0���������������Լ�ִ��
				if (Hit.Invulnerable) rcx = 0;

				auto ret = original(rcx, rdx);
				return ret;
			});
		MH_ApplyQueued();
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
