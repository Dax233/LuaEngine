#pragma once

#pragma region shlp
namespace hook_shlp {
	struct ProjectilesData {
		void* Plot;
		int Id;
		ProjectilesData(
			void* Plot = nullptr,
			int Id = 0)
			:Plot(Plot),Id(Id) {
		};
	};
	map<void*, ProjectilesData> ProjectilesList;

	SafetyHookInline g_hook_dtor;
	SafetyHookMid g_hook_ctor;
	static void Hook() {
		framework_logger->info("����Ͷ����shlp���ɺ����ٹ���");

		g_hook_ctor = safetyhook::create_mid(MH::Shlp::ctor,
			+[](SafetyHookContext& ctx) {
				int shlpid = ctx.r12;
				void* shlp = reinterpret_cast<void*>(ctx.rax);
				ProjectilesList[shlp] = ProjectilesData(shlp, shlpid);
			});

		g_hook_dtor = safetyhook::create_inline(MH::Shlp::dtor, reinterpret_cast<void*>(
			+[](void* shlp) {
				ProjectilesList.erase(shlp);
				return g_hook_dtor.call<int>(shlp);
			}));
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע��Ͷ����shlp��غ���");
		//ע��Ͷ�����ȡ����
		lua_register(L, "GetShlp", [](lua_State* pL) -> int
			{
				lua_newtable(pL);//����һ����񣬷���ջ��
				for (auto [Plot, shlpData] : ProjectilesList) {
					lua_pushinteger(pL, (long long)Plot);
					lua_newtable(pL);//ѹ������Ϣ��
					lua_pushstring(pL, "Id");//Id
					lua_pushinteger(pL, (long long)shlpData.Id);
					lua_settable(pL, -3);
					lua_settable(pL, -3);//����������
				}
				return 1;
			});
	}
}
#pragma endregion
