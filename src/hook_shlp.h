#pragma once

extern "C" void* _stdcall GetR12DPtr(void*);

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

	static void Hook() {
		framework_logger->info("����Ͷ����shlp���ɺ����ٹ���");
		MH_Initialize();

		HookLambda(MH::Shlp::dtor,
			[](auto rcx) {
				ProjectilesList.erase(rcx);
				return original(rcx);
			});
		HookLambda(MH::Shlp::ctor,
			[]() {
				void* ret = original();
				int shlpid = 0;
				GetR12DPtr(&shlpid);
				ProjectilesList[ret] = ProjectilesData(ret, shlpid);
				return ret;
			});
		MH_ApplyQueued();
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
